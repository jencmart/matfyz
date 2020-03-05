import random

class ReallyWeirdStrategy:
    strategyToPlay = "D"
    @staticmethod
    def author_name():
        return "Michaela Ruzinska"

    @staticmethod
    def strategy_name():
        return "Really Weird Strategy"


    def __init__(self):
        ReallyWeirdStrategy.strategyToPlay = "D"

    def reset(self):
        ReallyWeirdStrategy.strategyToPlay = "D"
    def last_move(self, my_move, other_move):
        if other_move == "C":
            ReallyWeirdStrategy.strategyToPlay = "C"
        else:
            ReallyWeirdStrategy.strategyToPlay = "D"

    def play(self):
        if ReallyWeirdStrategy.strategyToPlay == "D":
            return 'D'
        else:
            if random.randint(0, 1) == 0:
                return 'C'
            else:
                return 'D'

def create_strategy():
    return ReallyWeirdStrategy()