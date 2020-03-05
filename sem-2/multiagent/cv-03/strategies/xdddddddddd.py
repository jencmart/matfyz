import random

class xDDDDDDDDD:

    @staticmethod
    def author_name():
        return "Adam Lechovský"

    @staticmethod
    def strategy_name():
        return "xDDDDDDDDDD"

    def __init__(self):
        self.reset()

    def reset(self):
        self.iter = 0
        self.other_prev_moves = []
        self.my_prev_moves = []

    def last_move(self, my_move, other_move):
        self.other_prev_moves.append(other_move)
        self.my_prev_moves.append(my_move)

    def play(self):
        self.iter += 1
        #if random.random() < 0.1:
        #    return 'D'
        dd = 0
        cd = 0
        dc = 0
        cc = 0
        for i in range(len(self.other_prev_moves)):
            dd += 1 if self.my_prev_moves[i] == 'D' and self.other_prev_moves[i] == 'D' else 0
            cd += 1 if self.my_prev_moves[i] == 'C' and self.other_prev_moves[i] == 'D' else 0
            dc += 1 if self.my_prev_moves[i] == 'D' and self.other_prev_moves[i] == 'C' else 0
            cc += 1 if self.my_prev_moves[i] == 'C' and self.other_prev_moves[i] == 'C' else 0
        if (dd + cd + dc) - (cc) > 0:
            return 'D'
        else:
            return 'C'

def create_strategy():
    return xDDDDDDDDD()