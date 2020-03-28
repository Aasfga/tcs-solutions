from random import randint
from . import Player
from ..board import Board, Colour


class RandomPlayer(Player):
    def make_move(self, board: Board) -> (Colour, int):
        column = randint(0, board.width - 1)
        while board.is_column_full(column):
            column = randint(0, board.width - 1)
        return self.colour, column
