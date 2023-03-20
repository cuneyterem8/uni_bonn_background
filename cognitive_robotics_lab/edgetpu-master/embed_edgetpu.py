import argparse
from edgetpu.basic.basic_engine import BasicEngine
from PIL import Image

import numpy as np
import time
import h5py
import csv
import os

_RGB_MEAN = np.array([123.68, 116.78, 103.94], dtype = np.float32)
_IMG_MIN = -123.68
_IMG_MAX = 255 - 103.94
_IMG_RANGE = _IMG_MAX - _IMG_MIN

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--model', help='Path of the re-id model.', required=True)
    parser.add_argument(
        '--dataset', help='Path to the dataset csv file to be embedded.', required=True)
    parser.add_argument(
        '--image_root', help='Path that will be pre-pended to the filenames in the train_set csv.')
    parser.add_argument(
        '--filename', help='Name of the HDF5 file in which to store the embeddings.')

    args = parser.parse_args()

    # Possibly auto-generate the output filename.
    if args.filename is None:
        basename = os.path.basename(args.dataset)
        args.filename = os.path.splitext(basename)[0] + '_embeddings.h5'

    # Initialize engine.
    en = BasicEngine(args.model)

    output_tensors_sizes = en.get_all_output_tensors_sizes()
    if output_tensors_sizes.size != 1:
        raise ValueError( 'Re-ID Model should have one output tensor! This model has {}.'.format(en.num_outputs))

    input_tensor_shape = en.get_input_tensor_shape()
    print("loaded model from {}. input shape: {}, embedding dim: {}".format(args.model, input_tensor_shape, output_tensors_sizes[0]))
    
    modifiers = ['original']

    with open(args.dataset, newline='') as dataset_file, h5py.File(args.filename, 'w') as f_out:
        data_reader = csv.reader(dataset_file, delimiter=',')
        data_list = list(data_reader)
        emb_storage = np.zeros((len(data_list), output_tensors_sizes[0]), np.float32)
        print('computing embeddings for {} images'.format(emb_storage.shape[0]))
        
        for i, row in enumerate(data_list):
            pid, filename = row
            if args.image_root is not None:
                filename = os.path.join(args.image_root, filename)
            print('ID: {}, file: {}'.format(pid, filename))
            img = Image.open(filename)
            img_resized = img.resize((input_tensor_shape[2], input_tensor_shape[1]), Image.BILINEAR)
            img_np = np.asarray(img_resized).astype(np.float32)
            
            if img_np.shape != tuple(input_tensor_shape[1:]):
                print('ERROR: image size {} is not equal to required input tensor shape {}'.format(img_np.shape, tuple(input_tensor_shape[1:])))
                continue
            
            #normalize with image_net mean and re-quantize to 0...255 uint8 range
            #img_normalized = ((img_np - _RGB_MEAN) - _IMG_MIN) * 255. / _IMG_RANGE
            
            ### run inference ##
            #_, emb = en.run_inference(img_normalized.astype(np.uint8).flatten())
            _, emb = en.run_inference(img_np.astype(np.uint8).flatten())
            emb_storage[i] = emb
            
        # Store the final embeddings.
        emb_dataset = f_out.create_dataset('emb', data=emb_storage)
        
        # Store information about the produced augmentation and in case no crop
        # augmentation was used, if the images are resized or avg pooled.
        f_out.create_dataset('augmentation_types', data=np.asarray(modifiers, dtype='|S'))

if __name__ == '__main__':
  main()
