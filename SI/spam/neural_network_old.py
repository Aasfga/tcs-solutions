#!/usr/bin/env python
import sys
import numpy as np
from numpy import array
from numpy import exp

np.seterr(over='ignore')


def read_data(path):
    file = open(path, 'r')
    lines = file.readlines()
    return list(map(lambda x: array([float(n) for n in x.split()]), lines))


def get_weights(l_sizes):
    size = len(l_sizes) - 1
    layer_weights = []
    for i in range(size):
        x = l_sizes[i]
        y = l_sizes[i + 1]
        layer_weights.append(np.zeros((x, y)))
    return array(layer_weights)


def sigmoid(x):
    return 1.0 / (1.0 + exp(-x))


def sigmoid_derivative(x):
    return x * (1 - x)


class NeuralNetwork:
    def __init__(self, epochs, l_rate, h_layers, train_set, test_set):
        self.epochs = epochs
        self.l_rate = l_rate
        self.train_set = train_set
        self.test_set = test_set
        self.weights = get_weights([57] + h_layers + [1])

    def feed_forward(self, x):
        layers = [array([x])]
        for weight in self.weights:
            layers.append(sigmoid(np.dot(layers[-1], weight)))
        return layers

    def backpropagation(self, layers, y):
        losses = [2 * (layers[-1] - y)]
        errors = []
        derivative = []
        for i in reversed(range(len(self.weights))):
            layer = layers[i + 1]
            weight = self.weights[i]
            errors.append(losses[-1] * sigmoid_derivative(layer))
            losses.append(np.dot(errors[-1], weight.T))
        e_size = len(errors)
        for i in range(e_size):
            derivative.append(layers[i].T.dot(errors[::-1][i]))
        return array(derivative)

    def update(self, vector):
        x, y = vector[:-1], vector[-1]
        layers = self.feed_forward(x)
        der = self.backpropagation(layers, y)
        self.weights -= self.l_rate * der

    def train(self, log_enabled=False, log_interval=10):
        for epoch in range(self.epochs):
            if log_enabled and epoch % log_interval == 0:
                print(f"Epoch {epoch}: acc={self.accuracy()}, error={self.error()}")
            for vector in self.train_set:
                self.update(vector)
        return

    def check(self, x):
        return int(0.5 + self.output(x))

    def output(self, x):
        layers = self.feed_forward(x)
        return layers[-1][0][0]

    def accuracy(self):
        count = 0
        for vector in self.test_set:
            x, y = vector[:-1], vector[-1]
            output = self.check(x)
            if y == output:
                count += 1
        return count / len(self.test_set)

    def error(self):
        error = 0
        for vector in self.test_set:
            x, y = vector[:-1], vector[-1]
            error += (self.output(x) - y) ** 2
        return error / len(self.test_set)


def main_function():
    train, test = read_data(sys.argv[1]), read_data(sys.argv[2])
    neural = NeuralNetwork(epochs=20, l_rate=1, h_layers=[4],
                           train_set=train, test_set=test)
    neural.train(log_enabled=True, log_interval=4)
    for vector in test:
        x, y = vector[:-1], vector[-1]
        print(f"{y}  {neural.output(x)}")


main_function()


