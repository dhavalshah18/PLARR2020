import torch.nn as nn
import torch.nn.functional as F

## TODO: Define the convolutional neural network

class Net(nn.Module):
    def __init__(self, NUM_CLASSES=10):
        super(Net, self).__init__()
        self.num_classes = NUM_CLASSES

        # ***** TODO: Define the network layers *****
        self.conv1 = nn.Conv2d(in_channels=3, out_channels=32, kernel_size=5,
                               stride=1, padding=3)
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3,
                               stride=1, padding=2)
        self.conv3 = nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3,
                               stride=1, padding=2)
        self.conv4 = nn.Conv2d(in_channels=64, out_channels=128, kernel_size=3,
                               stride=1, padding=1)
        
        self.pool = nn.MaxPool2d(kernel_size=3, stride=2)

        self.fc1 = nn.Linear(in_features=2048, out_features=384)
        self.fc2 = nn.Linear(in_features=384, out_features=self.num_classes)

        self.relu = nn.ReLU(inplace=True)

#         self.softmax = nn.Softmax(dim=1)
    
    def forward(self, x):
        
         # ***** TODO: Implement the forward pass *****
        x = self.pool(self.relu(self.conv1(x)))
        x = self.pool(self.relu(self.conv2(x)))
        x = self.pool(self.relu(self.conv3(x)))
        x = self.relu(self.conv4(x))


        x = self.relu(self.fc1(x.view(-1, 2048)))
        x = self.fc2(x)

#         x = self.softmax(x)
        
        return x
