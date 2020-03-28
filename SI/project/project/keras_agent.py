from keras import Sequential
from keras.layers import Dense, Flatten
from keras.optimizers import Adam
from rl.agents import DQNAgent
from rl.memory import SequentialMemory
from rl.policy import BoltzmannQPolicy

from .agent import Agent


class KerasAgent(Agent):
    def __init__(self, env, layers, output_activation, mem_size=50000, l_rate=1e-3, warmup_steps=100, *args, **kwargs):
        super().__init__(env)
        model = Sequential()
        model.add(Flatten(input_shape=(1,) + env.observation_space.shape))
        for layer in layers:
            model.add(Dense(layer, activation="relu"))
        model.add(Dense(env.action_space.n, activation=output_activation))
        memory = SequentialMemory(limit=mem_size, window_length=1)
        policy = BoltzmannQPolicy()
        self.dqn = DQNAgent(model=model, nb_actions=env.action_space.n, memory=memory, nb_steps_warmup=warmup_steps,
                            target_model_update=1e-2, policy=policy)
        self.dqn.compile(Adam(lr=l_rate), metrics=["mae"])

    def fit(self, max_steps, visualize, verbose):
        self.dqn.fit(self.env, nb_steps=max_steps, visualize=visualize, verbose=verbose)

    def test(self, episodes, visualize):
        self.dqn.test(self.env, nb_episodes=episodes, visualize=visualize)
