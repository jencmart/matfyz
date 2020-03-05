import random

class Evil:

    

    @staticmethod
    def author_name():
        return "Zuzana Drázdová"

    @staticmethod
    def strategy_name():
        return "Evil"

    def __init__(self):
        self.d_counter = 0
        self.reset()

    def reset(self):
        self.last_op = None
        self.d_counter = 0


    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        if self.last_op == 'D':
            self.d_counter += 1
        if self.d_counter == 0:
            return 'C'
        if self.d_counter > 5:
            return 'D'
        else:
            return self.last_op if self.last_op else 'C'


def create_strategy():
    return Evil()
