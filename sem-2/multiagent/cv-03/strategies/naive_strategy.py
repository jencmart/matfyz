import random

class NaiveStrategy:
    @staticmethod
    def author_name():
        return "Jakub Svoboda"

    @staticmethod
    def strategy_name():
        return "Naive Strategy"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None
        self.cooperate_count = 0
        self.last_last_op = None
        self.is_alternating = False

    def last_move(self, my_move, other_move):
        if other_move == 'C':
            self.cooperate_count += 1
        
        self.last_last_op = self.last_op
        self.last_op = other_move

        if self.last_last_op: 
            self.is_alternating = False
            if (self.last_last_op == 'D' and self.last_op == 'C') or (self.last_last_op == 'C' and self.last_op == 'D'):
                self.is_alternating = True

    def play(self):
        if self.is_alternating:
            return 'D'
        if self.cooperate_count > 1 and self.last_op == 'C':
            return 'D'
        
        return self.last_op if self.last_op else 'C'

def create_strategy():
    return NaiveStrategy()