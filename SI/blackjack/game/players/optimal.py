from game import Move, Result
from game.players import LearningPlayer


class Optimal(LearningPlayer):
    def __init__(self, name: str, *args, **kwargs):
        super().__init__(name)

    def end_game(self, result: Result):
        pass

    def learned_move(self, state) -> Move:
        if state.ace and state.score <= 17:
            return Move.HIT
        elif state.ace and state.score >= 18 and state.visible_card >= 9:
            return Move.HIT
        elif not state.ace and state.score <= 16 and state.visible_card >= 7:
            return Move.HIT
        elif not state.ace and state.score <= 12 and state.visible_card in [2, 3]:
            return Move.HIT
        elif not state.ace and state.score <= 11 and state.visible_card in [4, 5, 6]:
            return Move.HIT
        else:
            return Move.STICK
