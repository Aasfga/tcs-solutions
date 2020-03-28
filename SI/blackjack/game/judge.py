from numpy import random

from game import Result, Move, Card
from game.players import Player

all_cards = [Card(2), Card(3), Card(4), Card(5), Card(6), Card(7), Card(8),
             Card(9), Card(10), Card(10), Card(10), Card(10), Card(11, True)]

cards_size = 100000
c_i = 0
cards = random.choice(all_cards, cards_size)


def pick_card():
    global c_i
    card = cards[c_i]
    c_i = (c_i + 1) % cards_size
    return card


class Judge:

    def __init__(self, player: Player, croupier: Player):
        self.player = player
        self.croupier = croupier

    @staticmethod
    def make_round(player: Player):
        while player.score < 21 and player.make_move() is Move.HIT:
            player.next_card(pick_card())

    def end_game(self, player_result: Result, croupier_result: Result):
        self.player.end_game(player_result)
        self.croupier.end_game(croupier_result)

    def start_game(self):
        visible_card = pick_card()
        self.player.start_game(pick_card(), pick_card(), visible_card)
        self.croupier.start_game(visible_card, pick_card(), visible_card)

        self.make_round(self.player)
        self.make_round(self.croupier)

        if self.player.score > 21 or 21 >= self.croupier.score > self.player.score:
            self.player.end_game(Result.DEFEAT)
            self.croupier.end_game(Result.VICTORY)
            return self.croupier
        elif self.player.score == self.croupier.score:
            self.player.end_game(Result.DRAW)
            self.croupier.end_game(Result.DRAW)
            return None
        else:
            self.player.end_game(Result.VICTORY)
            self.croupier.end_game(Result.DEFEAT)
            return self.player
