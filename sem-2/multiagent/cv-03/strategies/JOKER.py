import random

class JOKER:

    @staticmethod
    def author_name():
        return "Patrik Romanský"

    @staticmethod
    def strategy_name():
        return "JOKER"

    def __init__(self):
        self.DD = 0
        self.CD = 0
        self.DC = 0
        self.CC = 0
        self.alwaysD = False
        self.alwaysC = False
        self.reset()

    def reset(self):

        if self.DC == 0 and self.CC == 0:
            self.alwaysD = True
        else:
            self.alwaysD = False

        if self.DD == 0 and self.CD == 0:
            self.alwaysC = True
        else:
            self.alwaysC = False

        self.DD = 0
        self.CD = 0
        self.DC = 0
        self.CC = 0

    def last_move(self, my_move, other_move):
        if my_move == 'D':
            if other_move == 'D':
                self.DD += 1
            else:
                self.DC += 1
        else:
            if other_move == 'D':
                self.CD += 1
            else:
                self.CC += 1

    def reward(self):
        parm_CC = 3
        parm_DC = 5
        parm_DD = 3
        parm_CD = 5

        play_C = parm_CC * self.CC
        play_D = parm_DC * self.DC + parm_DD * self.DD + parm_CD * self.CD

        return play_C - play_D

    def play(self):

        if self.alwaysD:
            return 'D'

        if random.random() < 0.005:
            return 'C' if random.randint(0, 1) == 0 else 'D'

        #if self.alwaysC:
        #   return 'D'

        if self.reward() < 0:
            return 'D'
        else:
            return 'C'

def create_strategy():
    return JOKER()