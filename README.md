# Computer Science Master in University of Bonn

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/diplomas/uni_bonn.jpg?raw=true" height="80%">

The University of Bonn is one of the largest research-based universities in the world. The THE ranking is 89 and the QS ranking is 201 in the world in 2023. 

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/diplomas/desk.JPEG?raw=true" height="80%">

Being the best university in Germany in the field of mathematics, Uni of Bonn has also adjusted its computer science department to have a solid mathematical foundation. For this reason, many courses are built on combining heavy mathematical theory and practical coding skills.

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/diplomas/bonn_diploma.JPEG?raw=true" width="80%" height="70%">

## Lectures, Thesis, Labs, Seminars

I took a total of 120 credits with a total of 14 courses and thesis+seminar from three different fields. All lecture details can be seen in files here with a summary of the content of each one of them. The transcript and diploma supplement can be seen [here](https://github.com/cuneyterem8/uni_bonn_background/tree/main/diplomas)

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/diplomas/bonn_transcript_1.JPEG?raw=true" width="80%" height="70%">

intelligence systems: [artificial life](https://github.com/cuneyterem8/uni_bonn_background/tree/main/artificial_life), [machine learning](https://github.com/cuneyterem8/uni_bonn_background/tree/main/machine_learning), [neural networks](https://github.com/cuneyterem8/uni_bonn_background/tree/main/neural_networks), [robot learning](https://github.com/cuneyterem8/uni_bonn_background/tree/main/robot_learning), [cognitive robotics](https://github.com/cuneyterem8/uni_bonn_background/tree/main/cognitive_robotics), [cognitive robotics lab](https://github.com/cuneyterem8/uni_bonn_background/tree/main/cognitive_robotics_lab), [humanoid robots lab](https://github.com/cuneyterem8/uni_bonn_background/tree/main/humanoid_robots_lab), [humanoid robots seminar](https://github.com/cuneyterem8/uni_bonn_background/tree/main/humanoid_robots_seminar)

computer-vision-audio: [deep learning for visual recognition](https://github.com/cuneyterem8/uni_bonn_background/tree/main/deep_learning_for_visual_recognition), [audio signal processing](https://github.com/cuneyterem8/uni_bonn_background/tree/main/audio_signal_processing)

information systems: [thesis in usable security and privacy](https://github.com/cuneyterem8/uni_bonn_background/tree/main/thesis_usable_security_privacy), [usable security and privacy](https://github.com/cuneyterem8/uni_bonn_background/tree/main/usable_security_privacy), [it security](https://github.com/cuneyterem8/uni_bonn_background/tree/main/it_security), [sensor data fusion](https://github.com/cuneyterem8/uni_bonn_background/tree/main/sensor_data_fusion), [mobile communication](https://github.com/cuneyterem8/uni_bonn_background/tree/main/mobile_communication)

only exercises (not exams due to lack of preparation): [biomedical data science](https://github.com/cuneyterem8/uni_bonn_background/tree/main/biomedical_data_science_hw_only), [humanoid robots](https://github.com/cuneyterem8/uni_bonn_background/tree/main/humanoid_robots_hw_only), [foundation of graphics](https://github.com/cuneyterem8/uni_bonn_background/tree/main/foundation_of_graphics_hw_only), [network security](https://github.com/cuneyterem8/uni_bonn_background/tree/main/network_security_hw_only)

### Implementation example from Deep learning class

```python
import torch
import numpy as np
from tqdm import tqdm 
from time import sleep 
import matplotlib.pyplot as plt

from torchvision import datasets
import torchvision.transforms as transforms
import torch.nn as nn
import torch.optim as optim 
import torch.nn.functional as F
```
```python
train_dataset = datasets.CIFAR10(root = "cifar/", train = True, download = True, transform = transforms.ToTensor())
test_dataset = datasets.CIFAR10(root = "cifar/", train = False, download = True, transform = transforms.ToTensor())

train_loader = torch.utils.data.DataLoader(train_dataset, batch_size = 128, shuffle = True, pin_memory=True)
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size = 128, shuffle = True, pin_memory=True)
```
```python
class Network(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(32*32*3, 512)
        self.bn = nn.BatchNorm1d(512, affine=True)
        self.fc2 = nn.Linear(512, 512)
        self.fc3 = nn.Linear(512, 10)
        
    def forward(self, x):
        x = x.reshape(-1, 32*32*3)
        x = F.relu(self.bn(self.fc1(x)))
        x = F.relu(self.bn(self.fc2(x)))
        x = self.fc3(x)
        return x
    
    def forward_test(self, x):
        x = x.reshape(-1, 32*32*3)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x
```
```python
def train(model, optimizer, dataloader, batch:bool):
    for x, y in dataloader:
        x, y = x.to('cuda:0'), y.to('cuda:0')
        optimizer.zero_grad()
        if batch == True:
            prediction = model.forward(x)
        else: 
            prediction = model.forward_test(x)
        loss = nn.CrossEntropyLoss()
        output_loss = loss(prediction, y.to(torch.long))
        output_loss.backward()
        optimizer.step()
    return output_loss

def accuracy(model, dataloader):
    hits = 0
    with torch.no_grad():
        for x, y in dataloader:
            x, y = x.to('cuda:0'), y.to('cuda:0')
            prediction = model.forward_test(x)
            prediction = torch.argmax(prediction, dim=1)
            hits += (prediction == y).count_nonzero()
    acc = hits / len(dataloader.dataset)
    return acc
```
```python
def batch_norm(model_factory, optimizer_factory, dataloader, epochs):
    losses = torch.zeros(epochs)
    accuracies = torch.zeros(epochs)
    model = model_factory.to('cuda:0')
    optimizer = optimizer_factory
    for epoch in tqdm(range(epochs)):
        losses[epoch] = train(model, optimizer, dataloader, True)
        accuracies[epoch] = accuracy(model, test_loader)
        sleep(0.1)
    return losses, accuracies

def baseline_nobatch(model_factory, optimizer_factory, dataloader, epochs):
    losses = torch.zeros(epochs)
    accuracies = torch.zeros(epochs)
    model = model_factory.to('cuda:0')
    optimizer = optimizer_factory
    for epoch in tqdm(range(epochs)):
        losses[epoch] = train(model, optimizer, dataloader, False)
        accuracies[epoch] = accuracy(model, test_loader)
        sleep(0.1)
    return losses, accuracies
```
```python
model = Network()
optimizer  = optim.Adam(model.parameters(), lr= 0.005, betas= (0.9, 0.95))
batch_losses, batch_accuracies = batch_norm(model, optimizer, train_loader, 25)

model1 = Network()
optimizer1  = optim.Adam(model1.parameters(), lr= 0.005, betas= (0.9, 0.95))
baseline_losses, baseline_accuracies = baseline_nobatch(model1, optimizer1, train_loader, 25)

def plotting_losses(batch, base):
    plt.plot(batch.detach(), 'r', label="With Batch Norm")
    plt.plot(base.detach(), 'k', label="Withouot Batch Norm")
    plt.legend()
    plt.show()
	
def plotting_accuracies(batch, base):
    plt.plot(batch, 'r', label= "With batch norm")
    plt.plot(base, 'k', label= "without batch norm")
    plt.legend()
    plt.show()

plotting_losses(batch_losses[:5], baseline_losses[:5])
plotting_accuracies(batch_accuracies, baseline_accuracies)
```

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/deep_learning_for_visual_recognition/output1.png?raw=true" width="60%" height="60%">

> output 1

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/deep_learning_for_visual_recognition/output2.png?raw=true" width="60%" height="60%">

> output 2


### Theory example from Deep learning class

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/deep_learning_for_visual_recognition/image1.png?raw=true" width="60%" height="60%">

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/deep_learning_for_visual_recognition/image2.png?raw=true" width="60%" height="60%">

> Example from exercise

