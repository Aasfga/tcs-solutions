#!/usr/bin/env python


import os
import sys
from pprint import pprint
from collections import Counter
import random
import math
import time

random.seed(8537)


# In[786]:


# In[787]:


def read_reviews(directory):
    reviews = []
    files = os.listdir(directory)
    for file in files:
        path = os.path.join(directory, file)
        review = open(path, 'r').readlines()
        review = ''.join([line.strip() for line in review]).split(' ')
        reviews.append(review)
    return reviews, files

def get_reviews():
    pos_reviews, _ = read_reviews(sys.argv[1])
    neg_reviews, _ = read_reviews(sys.argv[2])
    test_reviews, files = read_reviews(sys.argv[3])
    return pos_reviews, neg_reviews, (test_reviews, files)


# In[788]:


def read_prebase(path):
    return [x.strip() for x in open(path, 'r', encoding='ISO-8859-1').readlines()]

def get_prebase():
    if len(sys.argv) == 5:
        pos_words_all = read_vbase('pos_words')
        neg_words_all = read_vbase('neg_words')
    else:
        pos_words_all = {'amazing',  'amusing',  'appreciate',  'beautiful',  'beauty',  'believable',  'best',  'better',  'brilliant',  'charming',  'classic',  'clear',  'clearly',  'clever',  'convincing',  'cool',  'decent',  'easy',  'effective',  'enjoy',  'enjoyable',  'enjoyed',  'enough',  'entertaining',  'excellent',  'exciting',  'fairly',  'famous',  'fans',  'fantastic',  'fast',  'favorite',  'fine',  'fresh',  'fun',  'genuine',  'good',  'grand',  'great',  'greatest',  'happy',  'hero',  'hilarious',  'humor',  'important',  'impressive',  'incredible',  'incredibly',  'intelligent',  'interesting',  'lead',  'leading',  'leads',  'led',  'like',  'liked',  'love',  'loved',  'master',  'masterpiece',  'memorable',  'modern',  'nice',  'nicely',  'outstanding',  'perfect',  'perfectly',  'popular',  'powerful',  'pretty',  'proves',  'pure',  'ready',  'realistic',  'recommend',  'rich',  'right',  'romantic',  'silent',  'smart',  'solid',  'strong',  'stunning',  'success',  'successful',  'superb',  'supporting',  'sweet',  'talent',  'talented',  'terrific',  'top',  'well',  'witty',  'wonder',  'wonderful',  'wonderfully',  'work',  'works',  'worth'}
        neg_words_all = {'annoying',  'attack',  'awful',  'bad',  'badly',  'bizarre',  'blame',  'bland',  'bored',  'boring',  'break',  'cheap',  'cold',  'contrived',  'crap',  'creepy',  'crime',  'critics',  'dark',  'dead',  'death',  'desperate',  'devil',  'die',  'dies',  'difficult',  'disappointing',  'disaster',  'doubt',  'dull',  'dumb',  'evil',  'excuse',  'failed',  'fails',  'failure',  'fake',  'fall',  'falls',  'fear',  'fiction',  'funny',  'guilty',  'hard',  'hate',  'hell',  'horrible',  'impossible',  'joke',  'kill',  'killed',  'killer',  'killing',  'kills',  'lack',  'lacks',  'lame',  'laughable',  'lose',  'lost',  'loud',  'mediocre',  'mess',  'miss',  'monster',  'murder',  'mysterious',  'mystery',  'odd',  'pathetic',  'plot',  'pointless',  'poor',  'poorly',  'prison',  'problem',  'problems',  'revenge',  'ridiculous',  'scary',  'scream',  'shame',  'silly',  'slow',  'strange',  'stuck',  'stupid',  'tension',  'terrible',  'trouble',  'twists',  'unfortunately',  'violent',  'waste',  'wasted',  'weak',  'wild',  'worse',  'worst',  'wrong'}
    return pos_words_all, neg_words_all

def get_most_common(reviews, words, n):
    counter = Counter()
    for w in words:
        counter[w] = 0
    for review in reviews:
        for word in review:
            if word in counter:
                counter[word] += 1
    return set(map(lambda k: k[0], counter.most_common()[:n]))

def get_base(reviews, n=5):
    words = get_prebase()
    pos_set = get_most_common(reviews[0], words[0], n)
    neg_set = get_most_common(reviews[1], words[1], n)
    return pos_set.union(neg_set)


# In[789]:


def get_vector(review, base, order):
    vector = {word: 0 for word in base}
    for word in review: 
        if word in base:
            vector[word] += 1
    return [vector[word] for word in order]

def get_dataset(reviews, base, order, seed=8537):
    pos_data = [[1] + get_vector(x, base, order) for x in reviews[0]]
    neg_data = [[0] + get_vector(x, base, order) for x in reviews[1]]
    test_data = [get_vector(x, base, order) for x in reviews[2][0]]
    data_set = pos_data + neg_data
    random.shuffle(data_set)
    return data_set, (test_data, reviews[2][1])


# In[790]:


def v_mul(a, b):
    return [a[i] * b[i] for i in range(len(a))]
        
        

class LinearRegression:
    def __init__(self, epochs, batch_size, l_rate, threshold=0.5):
        self.epochs = epochs
        self.batch_size = batch_size
        self.l_rate = l_rate
        self.threshold = threshold
    
    def predict(self, w, vector):
        return sum(v_mul(w, vector))
    
    def train_loss(self, w, test_set):
        error = 0
        for vector in test_set:
            y, x = vector[0], vector[1:]
            f = sum(v_mul(w, x))
            error += (f - y)**2
        error /= len(test_set)
        return error  
    
    def accuracy(self, w, test_set):
        count = 0
        for v in test_set:
            y, x = v[0], v[1:]
            ans = self.is_positive(w, x)
            if y == 1 and ans:
                count += 1
            if y == 0 and not ans:
                count += 1
        return count/len(test_set)
                    
    def update(self, w, batch):
        d = len(w)
        der = [0.0 for i in range(d)]
        for vector in batch:
            y, x = vector[0], vector[1:]
            f_w = 2*(sum(v_mul(w, x)) - y)
            for i in range(d):
                der[i] += f_w*x[i]
        for i in range(d):
            w[i] = w[i] - self.l_rate * der[i] / len(batch)
        
    def train(self, train_set, test_set, w_gen=lambda: random.uniform(0, 1)):
        w = [w_gen() for i in range(len(train_set[0]) - 1)]
        for epoch in range(self.epochs):
            for i in range(0, len(train_set), self.batch_size):
                batch = train_set[i:i+self.batch_size]
                self.update(w, batch)
        err = self.train_loss(w, test_set)
        acc = self.accuracy(w, test_set)
#         print(f"err:{err}, acc: {acc}")
        return Checker(w, self.threshold)
        
    def is_positive(self, w, vector):
        return self.predict(w, vector) >= self.threshold
    
    def is_negative(self, w, vector):
        return self.predict(w, vector) < self.threshold

    
    
class Checker:
    def __init__(self, w, threshold):
        self.w = w
        self.threshold = threshold
        
    def is_positive(self, vector):
        return self.predict(vector) >= self.threshold
    
    def is_negative(self, vector):
        return self.predict(vector) < self.threshold
    
    def predict(self, vector):
        return sum(v_mul(self.w, vector))
    
    def train_loss(self, test_set):
        error = 0
        for vector in test_set:
            y, x = vector[0], vector[1:]
            f = sum(v_mul(self.w, x))
            error += (f - y)**2
        error /= len(test_set)
        return error  
    
    def accuracy(self, test_set):
        count = 0
        for v in test_set:
            y, x = v[0], v[1:]
            ans = self.is_positive(x)
            if y == 1 and ans:
                count += 1
            if y == 0 and not ans:
                count += 1
        return count/len(test_set)


# In[791]:


def k_fold(k, model, data):
    fold_size = len(data)//k
    checkers = []
    for i in range(0, len(data), fold_size):
        test_set = data[i:i+fold_size]
        train_set = data[:i] + data[:i+fold_size]
        if(len(train_set) < fold_size/2):
            continue
        checkers.append(model.train(train_set, test_set))
    return checkers


# In[792]:


def judge(checkers, vector):
    n = 0
    y = 0
    for c in checkers:
        if c.is_positive(vector):
            y+=1
        else:
            n+=1
    return y > n

def killem_all(checkers, test, files):
    for i in range(len(test)):
        ans = judge(checkers, test[i])
        if ans:
            print(f"{files[i]} 1")
        else:
            print(f"{files[i]} -1")


# In[799]:


reviews = get_reviews()
base = get_base(reviews, 1000)
order = list(base)
model = LinearRegression(30, 4, 0.001)
data, test = get_dataset(reviews, base, order)
checkers = k_fold(5, model, data)
killem_all(checkers, test[0], test[1])

