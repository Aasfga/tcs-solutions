from typing import List
from numpy import array
from game import Move, Result, State, Card
from game.players import Player


class LearningPlayer(Player):
    def __init__(self, name: str):
        super().__init__(name)
        self.moves: List[(State, Move)] = []

    def make_move(self) -> Move:
        move = super().make_move()
        self.moves.append((self.state, move))
        return move

    def start_game(self, first: Card, second: Card, visible_card: Card):
        self.moves = []
        super().start_game(first, second, visible_card)

    def end_game(self, result: Result):
        raise NotImplementedError

    def learned_move(self, state) -> Move:
        raise NotImplementedError

    @property
    def next_move(self) -> Move:
        return self.learned_move(self.state)

    @property
    def learned_moves(self):
        with_ace = []
        without_ace = []
        for s in range(3, 21):
            sub_array = []
            for v in range(2, 12):
                state = State(s, v, True)
                sub_array.append(self.learned_move(state))
            with_ace.append(sub_array)
        for s in range(3, 21):
            sub_array = []
            for v in range(2, 12):
                state = State(s, v, False)
                sub_array.append(self.learned_move(state))
            without_ace.append(sub_array)
        return array(with_ace), array(without_ace)
