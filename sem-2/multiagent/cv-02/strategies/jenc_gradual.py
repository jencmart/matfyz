import random


# mean number of the runs 200
# var 25

# next week ... number of the runs can be different ( do not count on that)

#   C   D
# C  R   S
# D  T    P


#    C      D
# C  3/3   5/0
# D  5/0   1/1

# !!! playing the best way against all == always D ...
# thus we can not play the best against all in collective environment
# we want to score maximal number of the points

# T > R > P > S

# 17 deterministic strategies
# all_c / all_d
# play ddc / ccd / cd
# tft / tft with first D
# prober .. repead DCC ; D if oponent C in step 2 and 3 ; rest is TFT
# tf2t ...    CC ... D if D in last two (eg. DD)
# mem2 ..TFT for 1,2 then change between ALL_D, ALL_C, TFT, TF2t
# hard_tft -- CC ... D if D in at least one of last two
# slow_tft -- CC ... D if oponend DD , C if oponennent CC
# spitefull (coop until other D, then just D)
# soft_majo ... C until poroportion of opponent #C >= #D
# hard_majo .. same bust firt move is D
# gradual .. C .. n times D after n-th D from oponent ; calm if CC

# best strategies -- Gradual, tftm mem2, soft_majo

# PROBABILISTC
# Equalizer .. other player payoff between P and R
# Extortion ... can dominate any opponent in 1 to 1 meeting
# ( both Zero-Determinant [ZD] strategies )


# copy it to the
# scp ./strategies/martin_jenc_basic_strategy.py jencmar@u-pl4.ms.mff.cuni.cz:/tmp/mas/strategies
class MyStrategy:

    @staticmethod
    def author_name():
        return "Martin Jenc"

    @staticmethod
    def strategy_name():
        return "Martin Jenc - grad"

    # Use the RE - replicator equation
    # read Nature paper - optimal replicator equation
    def __init__(self):
        self.reset()
        self.op_turns = []
        self.my_turns = []
        self.cnt_D = 0
        self.remaining_D = 0

    def reset(self):
        self.op_turns = []
        self.my_turns = []
        self.op_turns = []
        self.my_turns = []
        self.cnt_D = 0
        self.remaining_D = 0

    def last_move(self, my_move, other_move):
        self.op_turns.append(other_move)
        self.my_turns.append(my_move)
        if other_move == 'D':
            self.cnt_D += 1

    def play(self):

        # first move
        if len(self.op_turns) == 0:
            return 'C'

        # second move
        if len(self.op_turns) == 1:
            if self.op_turns[-1] == 'D':
                return 'D'
            else:
                return 'C'

        # not 'D' mode
        if self.remaining_D == 0:
            if self.op_turns[-1] == 'C':
                return 'C'
            else:
                self.remaining_D = self.cnt_D - 1
                return 'D'

        # 'D' mode
        else:
            if self.op_turns[-1] == 'C' and self.op_turns[-2] == 'C':
                self.remaining_D = 0
                return 'C'

            self.remaining_D -= 1
            return 'D'


def create_strategy():
    return MyStrategy()
