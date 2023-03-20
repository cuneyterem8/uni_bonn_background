# -*- coding: utf-8 -*-
"""
Created on Fri Nov 27 22:26:44 2020

@author: bilge
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

import torch
import torch as th
import torch.nn as nn
from torch.autograd import Variable

import torchvision
import torchvision.transforms as transforms
from torch.utils.data import Dataset, Subset, TensorDataset, DataLoader
from sklearn.metrics import confusion_matrix

    
class Network(nn.Module):
    def __init__(self, hidden_layers):
        super().__init__()
        inp = nn.Linear(
            in_features = pow(28,2), 
            out_features = hidden_layers[0]
        )
        hiddens = [
            nn.Linear(
                in_features = hidden_layers[idx], 
                out_features = hidden_layers[idx+1]
            )
            for idx in range(len(hidden_layers) - 1)
        ]
        last = nn.Linear(
            in_features = hidden_layers[-1], 
            out_features = 10
        )
        self.layers = nn.ModuleList([
            inp,
            *hiddens,
            last
        ])
        self.softmax = nn.Softmax(dim=1)
        
    def forward(self, x):
        for layer in self.layers:
            x = layer(x)
        x = self.softmax(x)
        return x
    
    
def output_label(label):
    output_mapping = {
                 0: "T-shirt/Top",
                 1: "Trouser",
                 2: "Pullover",
                 3: "Dress",
                 4: "Coat", 
                 5: "Sandal", 
                 6: "Shirt",
                 7: "Sneaker",
                 8: "Bag",
                 9: "Ankle Boot"
    }
    return output_mapping[label]


def separate_dataset_by_label(dataset, class_idx):
    # Get all targets
    targets = dataset.targets
    # Create target_indices
    target_indices = np.arange(len(targets))
    
    # Get indices to keep from train split
    idx_to_keep = targets[target_indices]!=class_idx
    
    # Only keep your desired classes
    target_indices = target_indices[idx_to_keep]
    train_dataset = Subset(dataset, target_indices)
    return train_dataset

def separate_dataset_by_labels(dataset):
    return [
        separate_dataset_by_label(dataset, idx)
        for idx in range(10)    
    ]

def sort_dataset_by_label(dataset):
    data = dataset.data
    targets = dataset.targets

    data_sorted = torch.zeros_like(data)
    targets_sorted = torch.zeros_like(targets)
    
    ordered_indices = torch.argsort(targets)
    
    data_sorted[ordered_indices] = data
    targets_sorted[ordered_indices] = targets
    
    return TensorDataset(data_sorted.float(), targets_sorted)


def accuracy(predictions, labels):
    classes = torch.argmax(predictions, dim=1)
    return torch.mean((classes == labels).float())

def train(model, dataloader, optimizer):
    avg_loss = 0.0
    accuracies = []
    losses = []
    for i, (images, label) in enumerate(dataloader):
        optimizer.zero_grad()
        prediction = model(
            images.reshape(images.shape[0], pow(28,2))
        )
        loss = nn.CrossEntropyLoss()(prediction,label)
        loss.backward()
        optimizer.step()
        with th.no_grad():
            acc = accuracy(prediction, label)
            lss = loss.numpy()
            losses.append(lss)
            accuracies.append(acc)
            avg_loss += lss/len(dataloader)
            if i % 100 == 0:
                print(f"Train step {i} | Loss {lss} | Accuracy {acc}")
    return avg_loss, losses, accuracies

def train_on_data(dataloader, epoch, msg_prefix="", model=None, optimizer=None):
    if not model:
        model = Network((50, 20))
    if not optimizer:    
        optimizer = th.optim.SGD(model.parameters(), lr = 0.01)
    epoch_accuracies = []
    for ep in range(epoch):
        print(f"{msg_prefix}Epoch: {ep+1}/{epoch}")
        avg_loss, losses, accuracies = train(
            model,
            dataloader,
            optimizer
        )
        epoch_accuracies.append(accuracies[-1])
        print(f"Avg. loss: {avg_loss} | Accuracy: {epoch_accuracies[-1]}")
    return epoch_accuracies


def plot_accuracies(sep_class_accs, sorted_accs, shuffled_accs):
    fig, ax = plt.subplots()
    ax.set(
        xlabel='epoch', 
        ylabel='accuracy',
        title='Accuracy comparison'
    )
    ax.grid()

    # Data for plotting
    epochs = list(range(1, len(sorted_accs)+1))
    ax.plot(
        epochs, 
        [
            sep_class_accs[idx]
            for idx in range(0, len(sep_class_accs), 2)
        ],
        label='class separated'
    )
    ax.plot(epochs, sorted_accs, label='sorted')
    ax.plot(epochs, shuffled_accs, label='shuffled')
    ax.legend()
    
    fig.savefig("accuracy.png")
    plt.show()

if __name__ == '__main__':
    
    train_set = torchvision.datasets.FashionMNIST(
        "./data", 
        download=True, 
        transform=transforms.Compose([transforms.ToTensor()])
    )
    # test_set = torchvision.datasets.FashionMNIST(
    #     "./data", 
    #     download=True, 
    #     train=False, 
    #     transform=transforms.Compose([transforms.ToTensor()])
    # )
    class_datasets = separate_dataset_by_labels(train_set)
    sorted_dataset = sort_dataset_by_label(train_set)
    
    
    # Loaders
    class_train_loaders = [
        DataLoader(dataset, batch_size=64, shuffle=False)
        for dataset in class_datasets
    ]
    sorted_train_loader = DataLoader(sorted_dataset, batch_size=64, shuffle=False)
    shuffled_train_loader = DataLoader(train_set, batch_size=64, shuffle=True)
        
    # Separate class trainings
    separate_class_epoch = 5
    sep_class_accs = [] # to keep class training accuracies
    model = Network((50, 20))
    optimizer = th.optim.SGD(model.parameters(), lr = 0.01)
    for idx, loader in enumerate(class_train_loaders):
        accs = train_on_data(
            loader,
            separate_class_epoch,
            msg_prefix=f"Class {idx} - {output_label(idx)} | ",
            model=model,
            optimizer=optimizer
        )
        sep_class_accs += accs
            
    # Sorted by labels training
    sorted_epoch = 25
    sorted_accs = train_on_data(
        sorted_train_loader,
        sorted_epoch,
        msg_prefix="Sorted Dataset | "
    )

    # Shuffled training
    shuffled_epoch = 25
    shuffled_accs = train_on_data(
        shuffled_train_loader,
        shuffled_epoch,
        msg_prefix="Shuffled Dataset | "
    )
    
    plot_accuracies(sep_class_accs, sorted_accs, shuffled_accs)
    
# ACCURACY COMPARISON
# The shuffled training performed better than the other two as expected.
# Shuffling the data ensures that each data point creates an "independent"     
# change on the model, without being biased by the same points before them.
# In the other cases, Both are acting like sorted datasets. They show    
# plateaus on the graph because each group of data points belonging to one class
# are being trained back to back, which creates a bias.
# Also, shuffling makes it less likely to converge to a global minimum,
# and it generalizes better.    
    