from game.enums import Move
from game.state import State
from game import Result
from game.players import LearningPlayer
from numpy import random


class MonteCarlo(LearningPlayer):
    def __init__(self, name: str, gamma=0.8, *args, **kwargs):
        super().__init__(name)
        self.gamma: float = gamma
        self.count = {state: {move: 1.0 for move in Move} for state in State.states}
        self.sum = {state: {move: 1.0 for move in Move} for state in State.states}
        self.q = {state: {move: 1.0 for move in Move} for state in State.states}

    def end_game(self, result: Result):
        reward: float = result.value
        for state, move in reversed(self.moves):
            self.sum[state][move] += reward
            self.count[state][move] += 1
            self.q[state][move] = self.sum[state][move] / self.count[state][move]
            reward *= self.gamma

    def learned_move(self, state) -> Move:
        return max(self.q[state], key=self.q[state].get)


class EpsilonMonteCarlo(MonteCarlo):
    def __init__(self, name: str, gamma=0.8, epsilon=0.05, *args, **kwargs):
        super().__init__(name, gamma)
        self.epsilon = epsilon

    def learned_move(self, state):
        move = super().learned_move(state)
        change = random.choice([False, True], p=[1 - self.epsilon, self.epsilon])
        if change:
            return Move.HIT if move == Move.STICK else Move.STICK
        return move
