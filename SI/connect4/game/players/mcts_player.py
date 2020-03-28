import time
from copy import copy
from typing import Union, List
from math import sqrt, log
from random import randint

from ..board import Board, Colour
from . import Player


class MemoryCell:
    def __init__(self, board: Board, colour: Colour, parent: Union['MemoryCell', None], move: int):
        self.move = move
        self.colour = colour
        self.parent = parent
        self.board = board
        self.moves: Union[List['MemoryCell'], None] = None
        self.game_counter = 1
        self.win_counter = 0

    @property
    def win_ratio(self):
        return self.win_counter / self.game_counter

    @property
    def node_value(self):
        return self.win_counter / self.game_counter + 1.41 * sqrt(log(self.parent.game_counter) / self.game_counter)


class MctsPlayer(Player):
    def __init__(self, name: str, colour: Colour, timeout=3):
        super().__init__(name, colour)
        self.timeout = timeout
        self._choices = [randint(0, 100000) for _ in range(100000)]
        self._i = 0

    def choice(self, array):
        x = self._choices[self._i] % len(array)
        self._i = (self._i + 1) % len(self._choices)
        return array[x]

    @staticmethod
    def add_moves(cell: MemoryCell):
        cell.moves = []
        board = cell.board
        colour = cell.colour
        for i in board.possible_moves:
            board.drop_disc(colour, i)
            cell.moves.append(MemoryCell(copy(board), colour.other, cell, i))
            board.remove_disc(i)

    def simulation(self, board: Board, colour: Colour):
        trace = []
        while not board.is_terminal():
            moves = list(board.possible_moves)
            next_move = board.winning_move(colour)
            if next_move is None:
                next_move = self.choice(moves)
            trace.append(next_move)
            board.drop_disc(colour, next_move)
            colour = colour.other
        winner = board.winning_colour()
        for move in reversed(trace):
            board.remove_disc(move)
        return winner

    def make_move(self, board: Board) -> (Colour, int):
        cell = MemoryCell(board, self.colour, None, -1)
        start = time.time()


        i = 0
        while time.time() - start < self.timeout:
            print(f"\r{i}", end="")
            i+= 1
            # Selection
            while cell.moves is not None:
                cell = max(cell.moves, key=lambda c: c.node_value)

            loop_end, win = False, False
            if cell.board.is_terminal():
                loop_end = True
                win = cell.board.winning_colour() == self.colour

            # Expansion and simulation
            if not loop_end:
                self.add_moves(cell)
                cell = self.choice(cell.moves)
                win = self.simulation(board, cell.colour) == self.colour

            # Backpropagation
            while True:
                cell.game_counter += 1
                cell.win_counter += 1 if win else 0
                if cell.parent is None:
                    break
                cell = cell.parent
        moves = [(child.win_ratio, child.move) for child in cell.moves]
        score, move = max(moves, key=lambda x: x[0])
        return self.colour, move
