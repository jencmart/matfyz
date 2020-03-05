import random

class ShortMemory:

    @staticmethod
    def author_name():
        return "Ondrej Novak"

    @staticmethod
    def strategy_name():
        return "Short Memory"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_ops = []

    def last_move(self, my_move, other_move):
        if len(self.last_ops) == 7:
            self.last_ops = self.last_ops[:-1]
            
        self.last_ops.insert(0, other_move)

    def play(self):
        c_weight, d_weight = 0, 0
        for i in range(len(self.last_ops)):
            if self.last_ops[i] == 'C':
                c_weight += 1 / (i + 1)
            else:
                d_weight += 1 / (i + 1)

        return 'C' if c_weight >= d_weight else 'D'

def create_strategy():
    return ShortMemory()