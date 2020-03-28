from game import Result
from game.players import EpsilonMonteCarlo


class Sarsa(EpsilonMonteCarlo):
    def __init__(self, name: str, gamma=0.8, epsilon=0.05, alfa=1, d_alfa=0.99, d_epsilon=0.99):
        super().__init__(name, gamma, epsilon)
        self.d_epsilon = d_epsilon
        self.d_alfa = d_alfa
        self.alfa = alfa
        self.q[None] = {None: 0}

    def end_game(self, result: Result):
        reward: int = result.value
        moves = self.moves
        for i, (state, move) in enumerate(moves):
            next_state, next_move = moves[i + 1] if i + 1 < len(moves) else (None, None)
            next_q = self.q[next_state][next_move]
            crr_q = self.q[state][move]
            self.q[state][move] = crr_q + self.alfa * (reward + self.gamma * next_q - crr_q)
        self.alfa = max(self.alfa * self.d_alfa, 0.001)
        self.epsilon = max(self.epsilon * self.d_epsilon, 0.05)