import random
from collections import deque

from keras import Sequential
from keras.layers import Flatten, Dense, Input
from keras.optimizers import Adam
from .agent import Agent
import numpy as np
import statistics


class QAgent(Agent):
    def __init__(self, env, layers, output_activation, mem_size=50000, l_rate=1e-3, gamma=0.94, e_rate=1.0,
                 e_decay=0.996, e_min=0.01, warmup_steps=100, batch_size=32):
        super().__init__(env)
        self.warmup_steps = warmup_steps
        self.e_min = e_min
        self.e_decay = e_decay
        self.e_rate = e_rate
        self.gamma = gamma
        self.l_rate = l_rate
        self.batch_size = batch_size
        self.memory = deque(maxlen=mem_size)
        self.model = Sequential()
        self.model.add(Dense(layers[0], input_shape=(self.observation_space,)))
        for layer in layers[1:]:
            self.model.add(Dense(layer, activation="relu"))
        self.model.add(Dense(self.action_space, activation=output_activation))
        self.model.compile(loss="mse", optimizer=Adam(lr=l_rate))

    def predict(self, state):
        return self.model.predict(state)[0]

    def reshape_state(self, state):
        return np.reshape(state, [1, self.observation_space])

    def save_in_memory(self, state, action, reward, next_state, terminal):
        self.memory.append((state, action, reward, next_state, terminal))

    def make_move(self, state):
        if np.random.rand() < self.e_rate:
            return random.randrange(self.action_space)
        return np.argmax(self.predict(state))

    def update_exploration_rate(self):
        self.e_rate *= self.e_decay
        self.e_rate = max(self.e_rate, self.e_min)

    def update_model(self):
        batch = random.sample(self.memory, self.batch_size)
        train_x = []
        train_y = []
        for state, action, reward, next_state, terminal in batch:
            update = reward if terminal else reward + self.gamma * np.amax(self.predict(next_state))
            values = self.predict(state)
            values[action] = update
            train_x.append(state[0])
            train_y.append(values)
            values = np.reshape(values, [1, len(values)])
        train_x = np.array(train_x)
        train_y = np.array(train_y)
        self.model.fit(train_x, train_y, epochs=1, batch_size=self.batch_size, verbose =0)
        self.update_exploration_rate()

    def fit(self, max_steps, visualize, verbose, reward_transformer=lambda reward, state: reward):
        episode = 0
        all_steps = 0
        while all_steps < max_steps:
            steps = 0
            episode += 1
            state = self.env.reset()
            state = self.reshape_state(state)
            rewards = []
            while all_steps < max_steps:
                steps += 1
                all_steps += 1
                if visualize:
                    self.env.render()
                action = self.make_move(state)
                next_state, reward, terminal, info = self.env.step(action)
                next_state = self.reshape_state(next_state)
                reward *= -1 if terminal else 1
                reward = reward_transformer(reward, state)
                rewards.append(reward)
                self.save_in_memory(state, action, reward, next_state, terminal)
                state = next_state
                if all_steps >= self.warmup_steps:
                    self.update_model()
                if terminal:
                    print(f"Episode {episode}, reward_mean={statistics.mean(rewards)}, reward_sum={sum(rewards)}")
                    break

    def test(self, episodes, visualize):
        pass
