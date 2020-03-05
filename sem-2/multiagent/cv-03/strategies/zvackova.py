import random

class Zvackova:

    @staticmethod
    def author_name():
        return "Magda Zvackova"

    @staticmethod
    def strategy_name():
        return "Zvackova"

    def __init__(self):
        self.reset()

    def reset(self):
        self.First = None
        self.Second = None
        self.TfT = None

    def last_move(self, my_move, other_move):
        if not self.First:
            self.First = other_move
            return
        if not self.Second:
            self.Second = other_move
            if self.First == 'C' and self.Second == 'D':
                self.TfT = True
            return
        if self.First and self.Second:
            if other_move == 'D':
                self.TfT = None


    def play(self):
        if not self.First:
            return 'D'
        if not self.Second:
            return 'C'
        return 'C' if self.TfT else 'D'

def create_strategy():
    return Zvackova()