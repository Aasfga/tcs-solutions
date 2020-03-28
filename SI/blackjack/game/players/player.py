from game import Result, Move
from game.state import State
from game.cards import Card


class Player:
    def __init__(self, name: str):
        self.name: str = name
        self.state: State = State(0, 0, False)

    def start_game(self, first: Card, second: Card, visible_card: Card):
        state = State(0, visible_card.value, False).next_state(first).next_state(second)
        self.state = state

    def next_card(self, card: Card):
        self.state = self.state.next_state(card)

    def make_move(self) -> Move:
        return self.next_move

    def end_game(self, result: Result):
        raise NotImplementedError

    @property
    def next_move(self) -> Move:
        raise NotImplementedError

    @property
    def visible_card(self):
        return self.state.visible_card

    @property
    def has_ace(self):
        return self.state.ace

    @property
    def score(self):
        return self.state.score

    def __eq__(self, o: object) -> bool:
        if isinstance(o, Player):
            return self.name == o.name
        return False
