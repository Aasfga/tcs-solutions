from ..board import Colour, Board


class Player:
    def __init__(self, name: str, colour: Colour, *args, **kwargs):
        self.name = name
        self.colour = colour

    def make_move(self, board: Board) -> (Colour, int):
        raise NotImplementedError

    def __str__(self):
        return self.name

