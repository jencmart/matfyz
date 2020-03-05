import random

class ForgivingTitForTat:

    @staticmethod
    def author_name():
        return "Stepan Prochazka"

    @staticmethod
    def strategy_name():
        return "Forgiving Tit for Tat"

    def __init__(self, forgivness):
        self.forgivness = forgivness
        self.reset()

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if (
            (not self.last_op)
            or (self.last_op == 'C')
            or (random.random() < self.forgivness)
        ):
            return 'C'
        else:
            return 'D'

def create_strategy():
    return ForgivingTitForTat(forgivness=0.03)
