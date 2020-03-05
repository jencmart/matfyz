import random

class stoic_mysak:
    @staticmethod
    def author_name():
        return "Pavel Mysicka"

    @staticmethod
    def strategy_name():
        return "stoic"

    def _init__(self):
        self.reset()
    
    def reset(self):    
        self.last_op = 'C'
        self.kindness_balance = 3

    def last_move(self, my_move, other_move):
        self.last_op = other_move
        if other_move == 'D':
            self.kindness_balance -= 1
        elif self.kindness_balance < 3:     
            self.kindness_balance += 1
            

    def play(self):
        if self.kindness_balance > 0 : return 'C'
        else: return self.last_op

def create_strategy():
    return stoic_mysak()

