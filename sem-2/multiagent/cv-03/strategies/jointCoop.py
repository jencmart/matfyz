import random

class jointCoop:

    @staticmethod
    def author_name():
        return "Kaito Sato"

    @staticmethod
    def strategy_name():
        return "jointCoop"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if self.last_op == 'C':
            return 'C'
        else:
            return 'D'

def create_strategy():
    return jointCoop()