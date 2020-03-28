import numpy as np
from numpy import array
from numpy import exp

np.seterr(over='ignore')


def standarize(data):
    x = data[:, :-1]
    y = data[:, [-1]]
    x = (x - np.mean(x, axis=0)) / np.std(x, axis=0)
    return np.append(x, y, axis=1)


def read_data(path):
    file = open(path, 'r')
    lines = file.readlines()
    return array([[float(n) for n in l.split()] for l in lines])


def zero_weight(x, y):
    return np.zeros((x, y))


def one_weight(x, y):
    return np.ones((x, y))


def random_weight(x, y):
    return 2 * np.random.rand(x, y) - 1.0


def get_weights(layers, table_generator):
    size = len(layers) - 1
    return array([table_generator(layers[i], layers[i + 1]) for i in range(size)])


def sigmoid(x):
    return 1.0 / (1.0 + exp(-x))


def derivative(x):
    return x * (1.0 - x)


class NeuralNetwork:
    def __init__(self, input_size, h_layers, output_size, train_set, test_set, table_generator):
        self.train_set = train_set
        self.test_set = test_set
        self.weights = get_weights([input_size] + h_layers + [output_size], table_generator)

    def feed_forward(self, x):
        layers = [array([x])]
        for weight in self.weights:
            layers.append(sigmoid(np.dot(layers[-1], weight)))
        return layers

    def backpropagation(self, layers, y):
        losses = [2 * (layers[-1] - y)]
        errors = []
        for i in reversed(range(len(self.weights))):
            layer = layers[i + 1]
            weight = self.weights[i]
            errors.append(losses[-1] * derivative(layer))
            losses.append(np.dot(errors[-1], weight.T))
        return array([layers[i].T.dot(errors[::-1][i]) for i in range(len(errors))])

    def do_epoch(self, l_rate):
        for vector in self.train_set:
            x, y = vector[:-1], vector[-1]
            layers = self.feed_forward(x)
            gradient = self.backpropagation(layers, y)
            self.weights -= l_rate * gradient
        return

    def train(self, epochs=10, l_rate=1.0, log_enabled=False, log_interval=1):
        for epoch in range(epochs):
            if log_enabled and epoch % log_interval == 0:
                print(f"Epoch {epoch}: acc={self.accuracy()}, error={self.error()} l_rate: {l_rate}")
            self.do_epoch(l_rate)
            l_rate *= 0.95
        return

    def output(self, x):
        layers = self.feed_forward(x)
        return layers[-1].item()

    def classify(self, x):
        return int(self.output(x) + 0.5)

    def error(self):
        error = 0
        for vector in self.test_set:
            x, y = vector[:-1], vector[-1]
            error += (self.output(x) - y) ** 2
        return error / len(self.test_set)

    def accuracy(self):
        correct = 0
        for vector in self.test_set:
            x, y = vector[:-1], vector[-1]
            if y == self.classify(x):
                correct += 1
        return correct / len(self.test_set)


def main_function():
    train = array([[1, 0, 0, 0],
                   [1, 0, 1, 1],
                   [1, 1, 0, 1],
                   [1, 1, 1, 0]])
    test = train
    # train, test = read_data(sys.argv[1]), read_data(sys.argv[2])
    # train, test = standarize(train), standarize(test)
    neural = NeuralNetwork(input_size=3, h_layers=[2, 2], output_size=1, train_set=train, test_set=test,
                           table_generator=random_weight)
    neural.train(epochs=1000, log_enabled=True, log_interval=4, l_rate=1)

    for vector in test:
        x, _ = vector[:-1], vector[-1]
        print(neural.classify(x))


main_function()
