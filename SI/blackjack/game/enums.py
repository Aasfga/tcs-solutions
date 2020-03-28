from enum import Enum


class Move(Enum):
    HIT = "HIT"
    STICK = "STICK"

    def __str__(self):
        return self.value


class Result(Enum):
    DEFEAT = -1
    DRAW = 0
    VICTORY = 1
