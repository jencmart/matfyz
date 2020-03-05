import random
THRESHOLD = 5

class TryBeforeTrust:

    @staticmethod
    def author_name():
        return "Petr Bures"

    @staticmethod
    def strategy_name():
        return "Try Before Trust"

    def __init__(self):
        self.reset()

    def reset(self):
        self.rude_opponents = 0
        self.nice_opponents = 0

    def last_move(self, my_move, other_move):
        if(other_move == 'D'):
            self.rude_opponents += 1
        elif(other_move == 'C'):    
            self.nice_opponents += 1

    def play(self):
        if(self.rude_opponents > THRESHOLD * self.nice_opponents):
            return 'D'
        else:
            return 'C'

def create_strategy():
    return TryBeforeTrust()