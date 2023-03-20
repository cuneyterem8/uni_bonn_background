#!/usr/bin/env python3
from argparse import ArgumentParser
from importlib import import_module
from itertools import count
import os

import h5py
import json
import numpy as np
import tensorflow as tf
from tensorflow.python.tools import freeze_graph

from aggregators import AGGREGATORS
import common
from PIL import Image

parser = ArgumentParser(description='Embed a dataset using a trained network.')

# Required

parser.add_argument(
    '--experiment_root', required=True,
    help='Location used to store checkpoints and dumped data.')

parser.add_argument(
    '--dataset', required=True,
    help='Path to the dataset csv file to be embedded.')

# Optional

parser.add_argument(
    '--image_root', type=common.readable_directory,
    help='Path that will be pre-pended to the filenames in the train_set csv.')

parser.add_argument(
    '--checkpoint', default=None,
    help='Name of checkpoint file of the trained network within the experiment '
         'root. Uses the last checkpoint if not provided.')

parser.add_argument(
    '--loading_threads', default=8, type=common.positive_int,
    help='Number of threads used for parallel data loading.')

parser.add_argument(
    '--batch_size', default=1, type=common.positive_int,
    help='Batch size used during evaluation, adapt based on available memory.')

parser.add_argument(
    '--filename', default=None,
    help='Name of the tflite-file')

parser.add_argument(
    '--flip_augment', action='store_true', default=False,
    help='When this flag is provided, flip augmentation is performed.')

parser.add_argument(
    '--crop_augment', choices=['center', 'avgpool', 'five'], default=None,
    help='When this flag is provided, crop augmentation is performed.'
         '`avgpool` means the full image at the precrop size is used and '
         'the augmentation is performed by the average pooling. `center` means'
         'only the center crop is used and `five` means the four corner and '
         'center crops are used. When not provided, by default the image is '
         'resized to network input size.')

parser.add_argument(
    '--aggregator', choices=AGGREGATORS.keys(), default=None,
    help='The type of aggregation used to combine the different embeddings '
         'after augmentation.')

parser.add_argument(
    '--quiet', action='store_true', default=False,
    help='Don\'t be so verbose.')

_RGB_MEAN = np.array([123.68, 116.78, 103.94])

def flip_augment(image, fid, pid):
    """ Returns both the original and the horizontal flip of an image. """
    images = tf.stack([image, tf.reverse(image, [1])])
    return images, tf.stack([fid]*2), tf.stack([pid]*2)


def five_crops(image, crop_size):
    """ Returns the central and four corner crops of `crop_size` from `image`. """
    image_size = tf.shape(image)[:2]
    crop_margin = tf.subtract(image_size, crop_size)
    assert_size = tf.assert_non_negative(
        crop_margin, message='Crop size must be smaller or equal to the image size.')
    with tf.control_dependencies([assert_size]):
        top_left = tf.floor_div(crop_margin, 2)
        bottom_right = tf.add(top_left, crop_size)
    center       = image[top_left[0]:bottom_right[0], top_left[1]:bottom_right[1]]
    top_left     = image[:-crop_margin[0], :-crop_margin[1]]
    top_right    = image[:-crop_margin[0], crop_margin[1]:]
    bottom_left  = image[crop_margin[0]:, :-crop_margin[1]]
    bottom_right = image[crop_margin[0]:, crop_margin[1]:]
    return center, top_left, top_right, bottom_left, bottom_right


def main():
    # Verify that parameters are set correctly.
    args = parser.parse_args()

    # Possibly auto-generate the output filename.
    if args.filename is None:
        basename = os.path.basename(args.dataset)
        args.filename = os.path.splitext(basename)[0] + '.tflite'
    args.filename = os.path.join(args.experiment_root, args.filename)

    # Load the args from the original experiment.
    args_file = os.path.join(args.experiment_root, 'args.json')

    if os.path.isfile(args_file):
        if not args.quiet:
            print('Loading args from {}.'.format(args_file))
        with open(args_file, 'r') as f:
            args_resumed = json.load(f)

        # Add arguments from training.
        for key, value in args_resumed.items():
            args.__dict__.setdefault(key, value)

        args.image_root = args.image_root or args_resumed['image_root']
    else:
        raise IOError('`args.json` could not be found in: {}'.format(args_file))

    # Load the data from the CSV file.
    _, data_fids = common.load_dataset(args.dataset, args.image_root)

    net_input_size = (args.net_input_height, args.net_input_width)
    pre_crop_size = (args.pre_crop_height, args.pre_crop_width)

    # Setup a tf Dataset containing all images.
    dataset = tf.data.Dataset.from_tensor_slices(data_fids)
    print(type(dataset))

    # Convert filenames to actual image tensors.
    dataset = dataset.map(
        lambda fid: common.fid_to_image(
            fid, tf.constant('dummy'), image_root=args.image_root,
            image_size=pre_crop_size if args.crop_augment else net_input_size),
        num_parallel_calls=args.loading_threads)
    
    print('Dataset TYPE')
    print(dataset)

    # Augment the data if specified by the arguments.
    # `modifiers` is a list of strings that keeps track of which augmentations
    # have been applied, so that a human can understand it later on.
    modifiers = ['original']

    # Group it back into PK batches.
    dataset = dataset.batch(args.batch_size)

    # Overlap producing and consuming.
    dataset = dataset.prefetch(1)

    #images, _, _ = dataset.make_one_shot_iterator().get_next()
    image = tf.placeholder(tf.float32, shape=[1, *net_input_size, 3], name='normalized_input_image')

    # Create the model and an embedding head.
    model = import_module('nets.' + args.model_name)
    head = import_module('heads.' + args.head_name)

    endpoints, body_prefix = model.endpoints(image, is_training=False)
    with tf.name_scope('head'):
        endpoints = head.head(endpoints, args.embedding_dim, is_training=False)
        
    #print(endpoints)
    
    # Initialize the network/load the checkpoint.
    if args.checkpoint is None:
        checkpoint = tf.train.latest_checkpoint(args.experiment_root)
    else:
        checkpoint = os.path.join(args.experiment_root, args.checkpoint)
    if not args.quiet:
        print('Restoring from checkpoint: {}'.format(checkpoint))
        
    loader = tf.train.Saver()
    input_saver_def = loader.as_saver_def()
    
    frozen_graph_def = freeze_graph.freeze_graph_with_def_protos(
        input_graph_def=tf.get_default_graph().as_graph_def(),
        input_saver_def=input_saver_def,
        input_checkpoint=checkpoint,
        output_node_names=endpoints['emb'].name[:-2],
        restore_op_name='',
        filename_tensor_name='',
        clear_devices=True,
        output_graph='',
        initializer_nodes='')
    
    with tf.gfile.GFile('frozen_graph.pb', 'wb') as f:
        f.write(frozen_graph_def.SerializeToString())

    #img = Image.open().convert('RGB')
    print('Dataset TYPE2')
    print(dataset)

    def representative_dataset_gen():
        from PIL import Image
        _RGB_MEAN = np.array([123.68, 116.78, 103.94], dtype = np.float32)
        
        for i, fid in enumerate(data_fids):
            print('quantization image {:04d} / {}\r'.format(i, len(data_fids)), end="")
            if args.image_root is not None:
                fid = os.path.join(args.image_root, fid)
            img = Image.open(fid) # open image from disk
            img_resized = img.resize((net_input_size[1], net_input_size[0]), Image.BILINEAR) # resize image
            img_np = np.asarray(img_resized).astype(np.float32) # convert to numpy array 
#            img_np -= _RGB_MEAN # substract image_net mean
            input_np = img_np[np.newaxis]
            yield [input_np]

#        def representative_dataset_gen():
#            input_shape = (1, *net_input_size, 3)
#            for i in range(10):
#            # Get sample input data as a numpy array in a method of your choosing.
#                dummy_image = 255 * np.random.rand(*input_shape) - _RGB_MEAN
#                print('calibration image {}'.format(i))
#                yield [dummy_image.astype(np.float32)]
        
    #args.input: input tensor name, #args.output: output tensor names, separated by ','

    converter = tf.compat.v1.lite.TFLiteConverter.from_frozen_graph('frozen_graph.pb', [image.name[:-2]], [endpoints['emb'].name[:-2]])
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset_gen
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.uint8
    converter.inference_output_type = tf.uint8
    print('converting...')
    tflite_quant_model = converter.convert()

    with open(args.filename, "wb") as f:
        f.write(tflite_quant_model)
        
    print('quantized tflite model written to: {}'.format(args.filename))

if __name__ == '__main__':
    main()
