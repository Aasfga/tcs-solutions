from game import Judge, Move
from game.judge import pick_card
from game.players import SimplePlayer, MonteCarlo, EpsilonMonteCarlo, LearningPlayer, Optimal, Sarsa, QLearning
import matplotlib.pyplot as plt
import numpy as np


def moves_to_colors(moves):
    array = []
    for s in moves:
        array.append(list(map(lambda m: (88, 129, 87) if m == Move.HIT else (237, 106, 90), s)))
    return array


def percent(x, n, p=1):
    return f"{format(x * 100 / n, f'.{p}f')}%"


def show_image(moves, name, save=False, filename=''):
    array = moves_to_colors(moves)
    plt.imshow(array)
    plt.xticks(np.arange(-0.5, 10, 1), range(2, 12))
    plt.yticks(np.arange(-0.5, 18, 1), range(3, 21))
    plt.xlabel('Visible card')
    plt.ylabel('Score')
    plt.grid(True, color=(0, 0, 0))
    plt.title(name)
    if save:
        plt.savefig(f"plots/{filename}.png")
    else:
        plt.show()


player: LearningPlayer = EpsilonMonteCarlo('Bob', alfa=0.5, d_alfa=0.99, epsilon=0.05, d_epsilon=0.99, gamma=0.98)
croupier = SimplePlayer('Croupier', 17)
judge = Judge(player=player, croupier=croupier)
class_name = player.__class__.__name__
wins, losses, draws = 0, 0, 0

n = 1 * pow(10, 6) + 1
interval = n//1000
plot_interval = n//4

print(f"Player {class_name} after {n} games")
for i in range(n):
    if i % interval == 0:
        print(f"\rProgress {percent(i, n)}: ", end="")
        print(f"wins={percent(wins, interval)} ", end="")
        print(f"losses={percent(losses, interval)} ", end="")
        print(f"draws={percent(draws, interval)}", end="")
        wins, losses, draws = 0, 0, 0
    if i % plot_interval == 0:
        with_ace, without_ace = player.learned_moves
        show_image(with_ace, name='With usable ace')
        show_image(without_ace, name='Without usable ace')

    winner = judge.start_game()
    if winner == player:
        wins += 1
    elif winner == croupier:
        losses += 1
    else:
        draws += 1

with_ace, without_ace = player.learned_moves
show_image(with_ace, name='With usable ace', save=False, filename=f"{class_name}_ace")
show_image(without_ace, name='Without usable ace', save=False, filename=f"{class_name}_no_ace")
