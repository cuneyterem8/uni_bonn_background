# Biomedical Data Science


Most of the exercises include heavily practical coding and some theoretical math


## Content

- Short introduction to Bioinformatics and Biomedicine
- Statistical Basics: Probability distributions and Bayesian inference, statistical hypothesis testing, linear models, logistic regression, Principal Component Analysis
- Clustering
- Hidden Markov Models
- Principles of Supervised Machine Learning
- Elastic Net
- Basics of deep learning


### Python example from task

```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA

df = pd.read_csv('golub.csv')
df = df.T
print(df.shape)
#print(df.head())
X = df.iloc[1:, :]
print(X.shape)

df2 = pd.read_csv('golub.cl.csv')
df2 = df2.iloc[:, 1:]
y = df2['x']
print(y.shape)
#print(y.head())
```

```python
pca = PCA()
pca.fit(X)
print(pca.explained_variance_ratio_)

cum_sum = np.cumsum(pca.explained_variance_ratio_)
plt.plot(cum_sum, color='red')
plt.axvline(x=np.where(cum_sum>=0.95)[0][0])
plt.xlabel('num of components')
plt.ylabel('cumulative variance ratio');
```
<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/biomedical_data_science_hw_only/output1.png?raw=true" width="60%" height="60%">
> output 1

```python
pca = PCA()
pca_2 = pca.fit_transform(X)
pca_2 = pd.DataFrame(data=pca_2[:, :2], columns=['PC1', 'PC2'])
pca_2 = pd.concat([pca_2, y], axis=1)
print(pca_2.head(), '\n')
plt.scatter(pca_2.iloc[:, 0][y==0], pca_2.iloc[:, 1][y==0], color='blue')
plt.scatter(pca_2.iloc[:, 0][y==1], pca_2.iloc[:, 1][y==1], color='red')
plt.xlabel('PC1')
plt.ylabel('PC2')
plt.show()
```
<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/biomedical_data_science_hw_only/output2.png?raw=true" width="60%" height="60%">
> output 2

```python
from sklearn.manifold import TSNE
tsne = TSNE(n_components=2, verbose=1, perplexity=40, n_iter=300)
tsne_results = tsne.fit_transform(X)
plt.scatter(tsne_results[:, 0], tsne_results[:, 1], color=['blue' if y==0 else␣
↪'red' for y in y])
plt.xlabel('TSNE1')
plt.ylabel('TSNE2')
plt.show()
```

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/biomedical_data_science_hw_only/output3.png?raw=true" width="60%" height="60%">
> output 3

Example from exercise
