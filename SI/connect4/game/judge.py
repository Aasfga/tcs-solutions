from typing import Union
import numpy as np

from . import Player
from . import Board
from .board import Colour


class Judge:
    def __init__(self, alice: Player, bob: Player, board_shape=(6, 7)):
        self.players = [alice, bob]
        self.board: Board = Board(shape=board_shape)
        self._i = 0

    @property
    def next_player(self) -> Player:
        player = self.players[self._i]
        self._i = 1 - self._i
        return player

    def check_board(self) -> (bool, Union[Player, None]):
        if self.board.count_series(4, self.players[0].colour) > 0:
            return True, self.players[0]
        elif self.board.count_series(4, self.players[1].colour) > 0:
            return True, self.players[1]
        return False, None

    def start_game(self):
        is_winner, winner = False, None
        while not is_winner and not self.board.is_full():
            print(f"{self.board}\n")
            colour, column = self.next_player.make_move(self.board)
            assert self.board.drop_disc(colour, column), "Player made wrong move!"
            is_winner, winner = self.check_board()
        print(f"{self.board}")
        print(f"Player {winner} won!")
