import gym

from project import KerasAgent, QAgent

env = gym.make("CartPole-v0")
print(env.action_space)
agent = KerasAgent(env, layers=[40, 80], batch_size=32, e_decay=0.999, output_activation="linear")
agent.fit(max_steps=500000, visualize=True, verbose=2)
agent.test(episodes=5, visualize=True)
