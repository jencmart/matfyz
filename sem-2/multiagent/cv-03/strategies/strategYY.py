import random

class StrategYY:

    @staticmethod
    def author_name():
        return "YY"

    @staticmethod
    def strategy_name():
        return "StrategYY"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None
        self.num_allowed_D = 3
        self.since_my_last_D = 0
        self.D_after_D = 3
        self.was_D_recently = False

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if self.last_op is None:
            return 'C'
        elif self.num_allowed_D <= 0:
            return 'D'
        elif self.was_D_recently:
            if self.since_my_last_D < self.D_after_D:
                self.since_my_last_D += 1
                return 'D'
            self.was_D_recently = False
            return 'C'
        elif self.last_op == 'D':
            self.was_D_recently = True
            self.num_allowed_D -= 1
            self.since_my_last_D = 0
            return 'D'
        return 'C'

def create_strategy():
    return StrategYY()
