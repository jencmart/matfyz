import random

class TFTRD:

    @staticmethod
    def author_name():
        return "Martin Safko"

    @staticmethod
    def strategy_name():
        return "TFTRD"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if self.last_op is None:
            return 'C'
        elif self.last_op == 'C':
            if random.randint(1,100) < 35:
                return 'D'
            else:
                return 'C'
        else:
            return 'D'

def create_strategy():
    return TFTRD()