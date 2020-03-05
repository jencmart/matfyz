import random

class UniquelyNamedStrategy:

    @staticmethod
    def author_name():
        return "Jan Waltl"

    @staticmethod
    def strategy_name():
        return "Uniquely Named Strategy"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None
        self.last_my = None
        self.betrayal_count = 0

    def last_move(self, my_move, other_move):
        self.last_op = other_move
        self.last_my = my_move

    def play(self):
        if self.last_op=='D' and self.last_my=='C':
            self.betrayal_count+=1
        if self.betrayal_count>5:
            return 'D'
        else:
            return 'C'
        tft = self.last_op if self.last_op else 'C'
        e = random.random()
        if e<0.5: #TitForTat
            return tft
        elif e<0.9: #Forgive
            return 'C'
        else: #Betray
            return 'D'

def create_strategy():
    return UniquelyNamedStrategy()