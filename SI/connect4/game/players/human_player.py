from . import Player
from ..board import Board, Colour


class HumanPlayer(Player):
    def make_move(self, board: Board) -> (Colour, int):
        print("Choose column: ", end="")
        column = int(input())
        while board.is_column_full(column):
            print("Choose column: ", end="")
            column = int(input())
        return self.colour, column
