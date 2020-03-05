import random

from collections import deque

class Robocop:

    @staticmethod
    def author_name():
        return "Jan Uhlík"

    @staticmethod
    def strategy_name():
        return "Robocop"

    def __init__(self):
        self.last_moves = None # deque with last moves
        self.eps = None # probability of playing 'D'
        self.alpha = None # rate for increasing self.eps during the game

        self.reset()

    def reset(self):
        self.last_moves = deque(maxlen=35)
        self.eps = random.uniform(1e-4, 5e-4)
        self.alpha = 1 + random.uniform(1e-3, 5e-2)

    def last_move(self, my_move, other_move):
        # Save last move
        self.last_moves.append(other_move)

        # Increase probability of 'D'
        self.eps *= self.alpha
        self.eps = min(self.eps, 2e-3)

    def play(self):
        if random.uniform(0, 1) < self.eps:
            if len(self.last_moves) != 0:
                return self.last_moves[-1]
            else:
                return 'C'
        if self.last_moves.count('D') > 0.8 * self.last_moves.count('C'):
            return 'D'
        elif 'D' in list(self.last_moves)[-5:]:
            return 'D'
        else:
            return 'C'

def create_strategy():
    return Robocop()
