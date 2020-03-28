from game import Result
from game.enums import Move
from game.players.player import Player


class SimplePlayer(Player):
    def __init__(self, name, threshold: int):
        super().__init__(name)
        self.threshold: int = threshold

    def end_game(self, result: Result):
        pass

    @property
    def next_move(self) -> Move:
        return Move.HIT if self.score < self.threshold else Move.STICK
