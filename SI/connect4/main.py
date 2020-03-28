from game import Judge, Player, RandomPlayer, HumanPlayer, MinMaxPlayer, MctsPlayer
from game.board import Board, Colour

alice = HumanPlayer("Alice", Colour.WHITE,timeout=4)
bob = MinMaxPlayer("Bob", Colour.BLACK, timeout=4)
judge = Judge(alice, bob, board_shape=(6, 7))
judge.start_game()
