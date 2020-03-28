class Agent:
    def __init__(self, env):
        self.env = env

    def fit(self, max_steps, visualize, verbose):
        raise NotImplementedError

    def test(self, episodes, visualize):
        raise NotImplementedError

    @property
    def action_space(self):
        return self.env.action_space.n

    @property
    def observation_space(self):
        return self.env.observation_space.shape[0]
