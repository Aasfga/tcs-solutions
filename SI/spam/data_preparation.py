from random import shuffle


def read_raw_data():
    file = 'raw_data.in'
    raw_data = open(file, 'r').read().splitlines()
    data = []
    for row in raw_data:
        data.append([float(x) for x in row.split()])
    return data


def split_data(data, test_size=0.1):
    is_spam = list(filter(lambda x: x[-1] == 1, data))
    not_spam = list(filter(lambda x: x[-1] == 0, data))
    shuffle(is_spam)
    shuffle(not_spam)
    size_all = len(is_spam) + len(not_spam)
    proportion = len(is_spam) / size_all
    size_test = int(test_size * size_all)
    size_spam = int(proportion * size_test)
    size_not = int((1 - proportion) * size_test)
    test = is_spam[:size_spam] + not_spam[:size_not]
    train = is_spam[size_spam:] + not_spam[size_not:]
    shuffle(test)
    shuffle(train)
    return test, train


def save_data(data, path):
    with open(path, 'w') as file:
        file.write("\n".join([" ".join([str(y) for y in x]) for x in data]))


def main():
    test, train = split_data(read_raw_data())
    save_data(test, 'test.in')
    save_data(train, 'train.in')


main()
