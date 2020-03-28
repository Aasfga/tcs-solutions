from dataclasses import dataclass

from game.cards import Card


@dataclass(frozen=True, eq=True)
class State:
    score: int
    visible_card: int
    ace: bool

    states = []

    def next_state(self, card: Card):
        ace = self.ace
        score = self.score
        visible_card = self.visible_card
        if ace:
            score += 1 if card.is_ace else card.value
        else:
            score += card.value
            ace = card.is_ace
        if score > 21 and ace:
            score -= 10
            ace = False
        return State(score, visible_card, ace)

    @staticmethod
    def get_states():
        return State.states


for s in range(3, 21):
    for v in range(2, 12):
        State.states.append(State(s, v, True))
for s in range(3, 21):
    for v in range(2, 12):
        State.states.append(State(s, v, False))
