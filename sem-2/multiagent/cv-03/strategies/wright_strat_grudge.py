import random

class WrightStrategy:

    @staticmethod
    def author_name():
        return "Kashann Wright"

    @staticmethod
    def strategy_name():
        return "Strategy Grudge Wright"

    def __init__(self):
        self.reset()

    def reset(self):
        self.isGrudge = False
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if self.last_op == 'D':
        	self.isGrudge = True
        if self.isGrudge:
        	return 'D'
        return 'C'

def create_strategy():
    return WrightStrategy()