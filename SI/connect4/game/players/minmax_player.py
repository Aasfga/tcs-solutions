import time
from typing import List

from numpy import Infinity
from ..board import Board, Colour
from . import Player
from copy import copy
from collections import OrderedDict


class MinMaxPlayer(Player):
    def __init__(self, name: str, colour: Colour, max_depth=42, memory_limit=10000, timeout=1):
        super().__init__(name, colour)
        self.timeout = timeout
        self.memory_limit = memory_limit
        self.max_depth = max_depth
        self.memory = OrderedDict()

    @staticmethod
    def _calc_score(colour: Colour, board) -> int:
        score = 5 * board.count_series(2, colour)
        score += 50 * board.count_series(3, colour)
        score += 500 * board.count_series(4, colour)
        score += 5000 * board.count_series(5, colour)
        center = board[:, board.width // 2]
        score += len(center[center == colour])
        return score

    def retrieve_moves(self, key: str) -> List:
        if key in self.memory:
            self.memory.move_to_end(key)
            return self.memory[key]
        return []

    def store_moves(self, key: str, moves: List):
        self.memory[key] = moves
        if len(self.memory) > self.memory_limit:
            self.memory.popitem(last=False)

    @staticmethod
    def calc_score(colour: Colour, board: Board) -> int:
        return MinMaxPlayer._calc_score(colour, board) - MinMaxPlayer._calc_score(colour.other, board)

    def minmax(self, player_colour: Colour, board: Board, depth: int, maximizing: bool, alfa, beta):
        if depth == 0 or board.is_terminal():
            return -1, self.calc_score(self.colour, board)
        score = (-1 if maximizing else 1) * Infinity
        column = -1

        order = self.retrieve_moves(board.to_ascii())
        order = sorted(order, key=lambda t: t[1], reverse=maximizing)
        order = [t[0] for t in order]
        order = order + [i for i in range(board.width) if i not in order]
        moves = []

        for i in order:
            if board.is_column_full(i):
                continue
            board.drop_disc(player_colour, i)
            _, value = self.minmax(player_colour.other, board, depth - 1, not maximizing, alfa, beta)
            moves.append((i, value))
            board.remove_disc(i)
            if maximizing and score < value:
                score = value
                column = i
                alfa = max(alfa, score)
            elif not maximizing and score > value:
                score = value
                column = i
                beta = min(beta, score)
            if alfa >= beta:
                break

        self.store_moves(board.to_ascii(), moves)
        return column, score

    def make_move(self, board: Board) -> (Colour, int):
        start = time.time()
        move, score = -1, -1
        for i in range(1, self.max_depth):
            move, score = self.minmax(self.colour, board, i, True, -Infinity, Infinity)
            if time.time() - start > self.timeout:
                break
        return self.colour, move
