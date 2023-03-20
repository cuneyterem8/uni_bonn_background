#!/usr/bin/env python3
from argparse import ArgumentParser, FileType
from importlib import import_module
from itertools import count
import os

import h5py
import json
import numpy as np
from sklearn.metrics import average_precision_score
import tensorflow as tf

import common
import loss
import csv


parser = ArgumentParser(description='Evaluate a ReID embedding.')

parser.add_argument(
    '--excluder', required=True, choices=('market1501', 'diagonal','duke'),
    help='Excluder function to mask certain matches. Especially for multi-'
         'camera datasets, one often excludes pictures of the query person from'
         ' the gallery if it is taken from the same camera. The `diagonal`'
         ' excluder should be used if this is *not* required.')

parser.add_argument(
    '--query_dataset', required=True,
    help='Path to the query dataset csv file.')

parser.add_argument(
    '--query_embeddings', required=True,
    help='Path to the h5 file containing the query embeddings.')

parser.add_argument(
    '--gallery_dataset', required=True,
    help='Path to the gallery dataset csv file.')

parser.add_argument(
    '--gallery_embeddings', required=True,
    help='Path to the h5 file containing the gallery embeddings.')

parser.add_argument(
    '--metric', required=True, choices=loss.cdist.supported_metrics,
    help='Which metric to use for the distance between embeddings.')

parser.add_argument(
    '--filename', type=FileType('w'),
    help='Optional name of the json file to store the results in.')

parser.add_argument(
    '--batch_size', default=256, type=common.positive_int,
    help='Batch size used during evaluation, adapt based on your memory usage.')

parser.add_argument(
    '--use_market_ap', action='store_true', default=False,
    help='When this flag is provided, the average precision is computed exactly'
         ' as done by the Market-1501 evaluation script, rather than the '
         'default scikit-learn implementation that gives slightly different'
         'scores.')


def average_precision_score_market(y_true, y_score):
    """ Compute average precision (AP) from prediction scores.

    This is a replacement for the scikit-learn version which, while likely more
    correct does not follow the same protocol as used in the default Market-1501
    evaluation that first introduced this score to the person ReID field.

    Args:
        y_true (array): The binary labels for all data points.
        y_score (array): The predicted scores for each samples for all data
            points.

    Raises:
        ValueError if the length of the labels and scores do not match.

    Returns:
        A float representing the average precision given the predictions.
    """

    if len(y_true) != len(y_score):
        raise ValueError('The length of the labels and predictions must match '
                         'got lengths y_true:{} and y_score:{}'.format(
                            len(y_true), len(y_score)))

    # Mergesort is used since it is a stable sorting algorithm. This is
    # important to compute consistent and correct scores.
    y_true_sorted = y_true[np.argsort(-y_score, kind='mergesort')]

    tp = np.cumsum(y_true_sorted)
    total_true = np.sum(y_true_sorted)
    recall = tp / total_true
    recall = np.insert(recall, 0, 0.)
    precision = tp / np.arange(1, len(tp) + 1)
    precision = np.insert(precision, 0, 1.)
    ap = np.sum(np.diff(recall) * ((precision[1:] + precision[:-1]) / 2))

    return ap


def main():
    # Verify that parameters are set correctly.
    args = parser.parse_args()

    # Load the query and gallery data from the CSV files.
    query_pids, query_fids = common.load_dataset(args.query_dataset, None)
    gallery_pids, gallery_fids = common.load_dataset(args.gallery_dataset, None)
    query_pids = [1491, 1491, 1491]
    query_fids = ['query/1491_c1s6_024271_00.jpg', 'query/1491_c3s3_073219_00.jpg', 'query/1491_c5s3_073637_00.jpg']

    # Load the two datasets fully into memory.
    with h5py.File(args.query_embeddings, 'r') as f_query:
        query_embs = np.array(f_query['emb'])
    with h5py.File(args.gallery_embeddings, 'r') as f_gallery:
        gallery_embs = np.array(f_gallery['emb'])

    # Just a quick sanity check that both have the same embedding dimension!
    query_dim = query_embs.shape[1]
    gallery_dim = gallery_embs.shape[1]
    if query_dim != gallery_dim:
        raise ValueError('Shape mismatch between query ({}) and gallery ({}) '
                         'dimension'.format(query_dim, gallery_dim))
    query_emb_id1491 = query_embs[0:3, :]
    gallery_emb_id1491 = gallery_embs[0:3,:]
    gallery_emb_id1497 = gallery_embs[3:6, :]
    gallery_emb_id1501 = gallery_embs[6:, :]

    dist_1491_1491 = np.linalg.norm(gallery_emb_id1491 - query_emb_id1491)
    dist_1491_1497 = np.linalg.norm(gallery_emb_id1497 - query_emb_id1491)
    dist_1491_1501 = np.linalg.norm(gallery_emb_id1501 - query_emb_id1491)

    print('DISTANCES')
    print(dist_1491_1491)
    print(dist_1491_1497)
    print(dist_1491_1501)

    # Setup the dataset specific matching function
    excluder = import_module('excluders.' + args.excluder).Excluder(gallery_fids)

    # We go through the queries in batches, but we always need the whole gallery
    batch_pids, batch_fids, batch_embs = tf.data.Dataset.from_tensor_slices(
        (query_pids, query_fids, query_emb_id1491)
    ).batch(args.batch_size).make_one_shot_iterator().get_next()

    batch_distances = loss.cdist(batch_embs, gallery_embs, metric=args.metric)
    # Check if we should use Market-1501 specific average precision computation.
    if args.use_market_ap:
        average_precision = average_precision_score_market
    else:
        average_precision = average_precision_score

    # Loop over the query embeddings and compute their APs and the CMC curve.
    aps = []
    cmc = np.zeros(len(gallery_pids), dtype=np.int32)
    with tf.Session() as sess:
        for start_idx in count(step=args.batch_size):
            try:
                # Compute distance to all gallery embeddings
                distances, pids, fids = sess.run([
                    batch_distances, batch_pids, batch_fids])
                print('\rEvaluating batch {}-{}/{}'.format(
                        start_idx, start_idx + len(fids), len(query_fids)),
                      flush=True, end='')
            except tf.errors.OutOfRangeError:
                print()  # Done!
                break

            # Convert the array of objects back to array of strings
            pids, fids = np.array(pids, '|U'), np.array(fids, '|U')

            # Compute the pid matches
            pid_matches = gallery_pids[None] == pids[:,None]

            # Get a mask indicating True for those gallery entries that should
            # be ignored for whatever reason (same camera, junk, ...) and
            # exclude those in a way that doesn't affect CMC and mAP.
            mask = excluder(fids)
            distances[mask] = np.inf
            pid_matches[mask] = False

            print('DISTANCES-2')
            print(distances)

            # Keep track of statistics. Invert distances to scores using any
            # arbitrary inversion, as long as it's monotonic and well-behaved,
            # it won't change anything.
            scores = 1 / (1 + distances)
            for i in range(len(distances)):
                ap = average_precision(pid_matches[i], scores[i])

                if np.isnan(ap):
                    print()
                    print("WARNING: encountered an AP of NaN!")
                    print("This usually means a person only appears once.")
                    print("In this case, it's because of {}.".format(fids[i]))
                    print("I'm excluding this person from eval and carrying on.")
                    print()
                    continue

                aps.append(ap)
                # Find the first true match and increment the cmc data from there on.
                k = np.where(pid_matches[i, np.argsort(distances[i])])[0][0]
                cmc[k:] += 1

    # Compute the actual cmc and mAP values
    cmc = cmc / len(query_pids)
    mean_ap = np.mean(aps)

    # Save important data
    if args.filename is not None:
        json.dump({'mAP': mean_ap, 'CMC': list(cmc), 'aps': list(aps)}, args.filename)

    # Print out a short summary.
    print('mAP: {:.2%} | top-1: {:.2%} top-2: {:.2%} | top-5: {:.2%} | top-9: {:.2%}'.format(
        mean_ap, cmc[0], cmc[1], cmc[4], cmc[8]))

if __name__ == '__main__':
    main()
