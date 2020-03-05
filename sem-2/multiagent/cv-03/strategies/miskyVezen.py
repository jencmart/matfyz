import random


class MiskyVezen:

    @staticmethod
    def author_name():
        return "Michaela Vystrčilová"

    @staticmethod
    def strategy_name():
        return "MeanTat"

    def __init__(self):
        self.changed = 3
        self.goCrazy = 0.1
        self.change_tolerance = 3
        self.buildTrust = 0
        self.isTitForTat = False


    def reset(self):
        self.last_op = None
        self.last_me = None
        self.before_last_me = None
        self.before_last_op = None
        self.changed = 0
        self.goCrazy = 0.1
        self.change_tolerance = 3
        self.buildTrust = 0
        self.isTitForTat = False

    def last_move(self, my_move, other_move):
        self.before_last_op = self.last_op
        self.before_last_me = self.last_me
        self.last_op = other_move
        self.last_me = my_move


    def play(self):
        rand = random.uniform(0, 1)

        if rand < self.goCrazy:
            rand = random.uniform(0, 1)
            if rand > 0.5:
                return 'C'
            return 'D'

        if self.last_op is None:
            return 'C'

        if self.before_last_me is not None:
            if ((self.before_last_me == 'D') and (
                    self.before_last_op == 'C')) and (
                    (self.last_op == 'D')) and not self.isTitForTat:
                self.isTitForTat = True
                self.changed += 1

            if self.before_last_me != self.last_op:
                self.isTitForTat = False

        if self.changed < self.change_tolerance:
            if not self.isTitForTat:
                if self.last_op == 'D':
                   return 'D'
                else:
                    return 'C'
            else:
                return 'C'

        return 'D'

def create_strategy():
    return MiskyVezen()





