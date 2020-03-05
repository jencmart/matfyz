import random

class aaaa:

    @staticmethod
    def author_name():
        return "Patrik Dokoupil"

    @staticmethod
    def strategy_name():
        return "aaaa"

    def __init__(self):
        self.reset()
        self.stepsD = 0

    def reset(self):
        self.last_op = None
        self.stepsD = 0

    def last_move(self, my_move, other_move):
        self.last_op = other_move

        if (self.stepsD > 0 and other_move == 'C'):
            self.stepsD = random.randint(0,3)
            return

        if (self.last_op == 'D'):
            self.stepsD = random.randint(1,5)
        else:
            self.stepsD = 0

    def play(self):
        if (self.last_op == None):
            return 'C'
        else:
            if (self.stepsD > 0):
                self.stepsD = self.stepsD - 1
                return 'D'
            else:
                return 'C'

def create_strategy():
    return aaaa()