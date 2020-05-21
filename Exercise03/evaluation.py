

import torch

classes = ('plane', 'car', 'bird', 'cat',
           'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

def check_accuracy(loader, model, device):   
    num_correct = 0
    num_samples = 0
    ## TODO: Set model to evaluation mode.
    model.eval()
    softmax = torch.nn.Softmax(dim=1)

    with torch.no_grad():
        for inputs, labels in loader:
            inputs = inputs.to(device=device)  # move to device, e.g. GPU
            labels = labels.to(device=device)
            
            ## TODO: Pass inputs through the model. 
            outputs = model(inputs)
#             print(outputs)
            
            ## TODO: Collect the prediction classes according to probability values. 
            scores =  softmax(outputs)
#             print(scores)
            _, preds = torch.max(scores, 1)
            
            num_correct += (preds == labels).sum()
            num_samples += preds.size(0)
        acc = float(num_correct) / num_samples
        print('Got %d / %d correct (%.2f)' % (num_correct, num_samples, 100 * acc))


def check_accuracy_perclass(loader, model, device):
    class_correct = list(0. for i in range(10))
    class_total = list(0. for i in range(10))
    ## TODO: Set model to evaluation mode.
    model.eval()

    with torch.no_grad():
        for inputs, labels in loader:

            inputs = inputs.to(device=device)  # move to device, e.g. GPU
            labels = labels.to(device=device) 

            ## TODO: Pass inputs through the model. 
            outputs = model(inputs)
            
            ## TODO: Collect the prediction classes according to probability values. 
            _, preds = torch.max(outputs, 1)
            
            c = (preds == labels).squeeze()
            for i in range(4):
                label = labels[i]
                class_correct[label] += c[i].item()
                class_total[label] += 1


    for i in range(10):
        print('Accuracy of %5s : %2d %%' % (
            classes[i], 100 * class_correct[i] / class_total[i]))

