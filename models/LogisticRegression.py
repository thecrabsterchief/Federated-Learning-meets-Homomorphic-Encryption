import torch

class LogisticRegression(torch.nn.Module):
    def __init__(self, num_features):
        # init super class of LogisticRegression
        super(LogisticRegression, self).__init__()
        
        # create "linear neural network"
        input_dim  = num_features
        output_dim = 1
        self.linear = torch.nn.Linear(input_dim, output_dim)

        # initialize Weights and Bias
        self.linear.weight.detach().zero_()
        self.linear.bias.detach().zero_()

    def forward(self, x):
        return torch.sigmoid(self.linear(x))