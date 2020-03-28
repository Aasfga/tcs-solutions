from game import Result, Card
from game.enums import Move
from game.players.player import Player
from typing import Type


# noinspection PyMissingConstructor
class LoggerPlayer(Player):
    def __init__(self, player: Player):
        self.player = player

    def start_game(self, first: Card, second: Card, visible_card: Card):
        self.player.start_game(first, second, visible_card)
        print(f"Player {self.name} starts game with {self.state}")

    def next_card(self, card: Card):
        old_state = self.state
        self.player.next_card(card)
        print(f"Player {self.name} receives card {card}. {old_state} -> {self.state}")

    def make_move(self) -> Move:
        move = self.player.make_move()
        print(f"Player {self.name} {move}S in {self.state}")
        return move

    def end_game(self, result: Result):
        print(f"Player {self.name} - {result} in {self.state}")

    @property
    def next_move(self) -> Move:
        return self.player.next_move

    @property
    def name(self):
        return self.player.name

    @property
    def state(self):
        return self.player.state


def add_logging(player: Player):
    return LoggerPlayer(player)


