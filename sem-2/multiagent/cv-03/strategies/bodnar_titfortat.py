# The presence of the Random strategy means, that all the submitted
# Tit For Tats will be competing with each other in the game of Getting Points In The Class,
# and that their order will be decided by random.

import random

class Bodnar_TitForTat:

    @staticmethod
    def author_name():
        return "Jan Bodnar"

    @staticmethod
    def strategy_name():
        return "Jan Bodnar - Tit for Tat"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None
        self.my_last_move = None
        self.logs={"CC":0,"DD":0, "CD":0, "DC":0}

    def last_move(self, my_move, other_move):
        self.last_op = other_move
        self.logs[my_move+other_move]+=1

    def play(self):
        sum_=sum(self.logs.values())
        if(self.logs["CD"] >= 10 and
           self.logs["DC"] >= 10 and
           self.logs["CC"] >= 10 and
           self.logs["DD"] >= 10 and
           self.logs["CD"] + self.logs["DD"]>=sum_/3 and
           self.logs["CC"] + self.logs["DC"]>=sum_/3):
             return "D"
        return self.last_op if self.last_op else 'C'

def create_strategy():
    return Bodnar_TitForTat()