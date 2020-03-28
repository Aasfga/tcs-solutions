from game import Result
from game.players import Sarsa


class QLearning(Sarsa):
    def end_game(self, result: Result):
        reward = result.value
        moves = self.moves
        for i, (state, move) in enumerate(moves):
            next_state, next_move = moves[i + 1] if i + 1 < len(moves) else (None, None)
            next_q = max(self.q[next_state].values())
            crr_q = self.q[state][move]
            self.q[state][move] = crr_q + self.alfa * (reward + self.gamma * next_q - crr_q)
        self.alfa = max(self.alfa * self.d_alfa, 0.001)
        self.epsilon = max(self.epsilon * self.d_epsilon, 0.05)
