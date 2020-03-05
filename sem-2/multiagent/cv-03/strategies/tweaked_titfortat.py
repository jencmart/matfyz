import random

class TweakedTitForTat:

    @staticmethod
    def author_name():
        return "Peter Guba"

    @staticmethod
    def strategy_name():
        return "Tweaked Tit for Tat"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        aux = random.uniform(0, 10)
        if not self.last_op: return 'C'
        if (aux < 9) and self.last_op: return self.last_op
        else: return 'C'
        #return self.last_op if self.last_op else 'C'

def create_strategy():
    return TweakedTitForTat()