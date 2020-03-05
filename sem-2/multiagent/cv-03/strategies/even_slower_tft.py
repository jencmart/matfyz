import random

class TitForTat:

    @staticmethod
    def author_name():
        return "Tomas Pilar"

    @staticmethod
    def strategy_name():
        return "Even slower Tit for Tat"

    def __init__(self):
        self.reset()

    def reset(self):
        self.moves = ['C', 'C', 'C']

    def last_move(self, my_move, other_move):
        self.moves = self.moves[1:] + [other_move]

    def play(self):
        return self.moves[0]

def create_strategy():
    return TitForTat()