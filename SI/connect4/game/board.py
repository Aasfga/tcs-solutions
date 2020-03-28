from enum import Enum
from copy import deepcopy
import numpy as np


class Colour(Enum):
    EMPTY = ' '
    WHITE = '\u25CB'
    BLACK = '\u25CF'

    def __str__(self):
        return self.value

    @property
    def other(self):
        assert self.value != Colour.EMPTY, "There is no such colour"
        return Colour.WHITE if self == Colour.BLACK else Colour.BLACK

    @property
    def ascii(self):
        return 'o' if self == Colour.WHITE else 'x' if self == Colour.BLACK else '-'


class Board:
    def __init__(self, shape: (int, int)):
        self.shape = shape
        self.state = np.array([[Colour.EMPTY] * self.width] * self.height)
        self.counter = [0 for _ in range(self.width)]

    def __copy__(self):
        board = Board(self.shape)
        board.state = deepcopy(self.state)
        board.counter = deepcopy(self.counter)
        return board

    @property
    def height(self):
        return self.shape[0]

    @property
    def width(self):
        return self.shape[1]

    def __getitem__(self, item):
        return self.state.__getitem__(item)

    def __setitem__(self, key, value):
        return self.state.__setitem__(key, value)

    def __str__(self):
        board = '|' + '-' * self.width + '|\n'
        for row in reversed(self.state):
            board += '|' + "".join([str(cell) for cell in row]) + '|\n'
        board += '|' + '-' * self.width + '|'
        return board

    def is_column_full(self, column: int):
        return self.counter[column] == self.height

    def is_column_empty(self, column: int):
        return self.counter[column] == 0

    def is_full(self):
        return all(map(lambda x: x == self.height, self.counter))

    def is_empty(self):
        return all(map(lambda x: x == 0, self.counter))

    def drop_disc(self, colour: Colour, column: int) -> bool:
        assert 0 <= column < self.width, f"`column` {column} is not in [0, {self.width})"
        assert colour != Colour.EMPTY, "There is no empty disc"
        if self.is_column_full(column):
            return False
        row = self.counter[column]
        self.state[row][column] = colour
        self.counter[column] += 1
        return True

    def remove_disc(self, column: int) -> bool:
        assert 0 <= column < self.width, f"`column` {column} is not in [0, {self.width})"
        if self.is_column_empty(column):
            return False
        self.counter[column] -= 1
        row = self.counter[column]
        self.state[row][column] = Colour.EMPTY
        return True

    def vertical_count(self, target: int, colour: Colour) -> int:
        count = 0
        for y in range(self.height - target + 1):
            for x in range(self.width):
                sub_counter = sum(1 if self.state[y + t][x] == colour else 0 for t in range(target))
                count += 1 if sub_counter == target else 0
        return count

    def horizontal_count(self, target: int, colour: Colour) -> int:
        count = 0
        for y in range(self.height):
            for x in range(self.width - target + 1):
                sub_counter = sum(1 if self.state[y][x + t] == colour else 0 for t in range(target))
                count += 1 if sub_counter == target else 0
        return count

    def diagonal_count(self, target: int, colour: Colour, diag_type: str) -> int:
        assert diag_type in ["left", "right"], "Wrong diagonal type"
        d = 1 if diag_type == "left" else -1
        start = 0 if diag_type == "left" else target - 1
        end = self.width - target + 1 if diag_type == "left" else self.width
        count = 0

        for y in range(0, self.height - target + 1):
            for x in range(start, end):
                sub_counter = sum(1 if self.state[y + t][x + d*t] == colour else 0 for t in range(target))
                count += 1 if sub_counter == target else 0
        return count

    def count_series(self, target: int, colour: Colour) -> int:
        vertical = self.vertical_count(target, colour)
        horizontal = self.horizontal_count(target, colour)
        diagonal = self.diagonal_count(target, colour, "left") or self.diagonal_count(target, colour, "right")
        return vertical + horizontal + diagonal

    def is_terminal(self):
        series = self.count_series(4, Colour.BLACK) + self.count_series(4, Colour.WHITE)
        return series > 0 or self.is_full()

    def winning_colour(self):
        if self.count_series(4, Colour.WHITE):
            return Colour.WHITE
        elif self.count_series(4, Colour.BLACK):
            return Colour.BLACK
        return None

    @property
    def possible_moves(self):
        return (column for column in range(self.width) if not self.is_column_full(column))

    def winning_move(self, colour: Colour):
        for move in self.possible_moves:
            self.drop_disc(colour, move)
            win = self.count_series(4, colour) > 0
            self.remove_disc(move)
            if win:
                return move
        return None

    def to_ascii(self) -> str:
        return '\n'.join(''.join(item.ascii for item in row) for row in self.state)
