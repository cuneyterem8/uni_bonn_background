import torch
import numpy

class Dataset(torch.utils.data.Dataset):
    def __init__(self, X, y):
        self.t_X_train = X
        self.t_y_train = y

        d_y_train = torch.tensor([0.])
        inps = torch.tensor([0.])
        index_y_labels = []
        x = self.t_y_train
        for i in range(len(self.t_y_train)):
            if (x[i].item() == 0.0) or (x[i].item() == 1.0):
                index_y_labels.append(i)
                inps = x[i]
                inps = inps.unsqueeze(0)
                d_y_train = torch.cat((d_y_train, inps))
        self.t_y_train = d_y_train

        X_train = self.t_X_train.numpy()
        d_X_train = X_train
        for i in numpy.arange(len(X_train)-1, -1, -1):
            if all(i != a for a in index_y_labels):
                d_X_train = numpy.delete(d_X_train, i, axis=0)
        nd_X_train = d_X_train
        d_X_train = torch.from_numpy(d_X_train)
        self.t_X_train = d_X_train

        mean = 0.
        for data in d_X_train:
            mean = data.mean()
        print("mean: " + str(mean))
        print("std: " + str(torch.sqrt((data**2).mean() - mean**2)))

    def __getitem__(self, index):
        data = self.t_X_train[index]
        return data, index

    def __len__(self):
        return len(self.t_X_train)