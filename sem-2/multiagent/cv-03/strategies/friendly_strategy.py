import random

class FriendlyStrategy:

    @staticmethod
    def author_name():
        return "David Bělíček"

    @staticmethod
    def strategy_name():
        return "Friendly Strategy"

    def __init__(self):
        self.N = 3
        self.reset()

    def reset(self):
        self.last = ['C'] * self.N
        self.last0 = 'C'
        self.last1 = 'C'
        self.last2 = 'C'

    def last_move(self, my_move, other_move):
        for i in range(len(self.last) - 1):
            self.last[i] = self.last[i + 1]

        self.last[len(self.last) - 1] = other_move

    def play(self):
        if 'D' in self.last:
            return 'D'
        else:
            return 'C'


def create_strategy():
    return FriendlyStrategy()