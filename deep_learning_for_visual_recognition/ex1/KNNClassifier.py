from collections import Counter
import torch

class KNNClassifier:

    def __init__(self, k, X, y):
        self.k = k
        self.X_train = X
        self.y_train = y

    def forward(self, X):
        y_pred = torch.tensor([self.calculate_distance(x) for x in X])
        return y_pred

    def calculate_distance(self, x):
        distances = torch.tensor([torch.sqrt(torch.sum((x - x_train)**2)) for x_train in self.X_train])
        k_n_labels = [self.y_train[i] for i in torch.argsort(distances)[:self.k]]  
        nearest = Counter(k_n_labels).most_common(1)
        return nearest[0][0]
	

