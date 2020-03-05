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
        return "Martin Jenc - Basic strategy"

    # Use the RE - replicator equation
    # read Nature paper - optimal replicator equation
    def __init__(self):
        self.reset()
        self.other_results = []
        self.my_results = []
        self.strategy = 'unk'

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move
        self.my_last = my_move
        self.other_results.append(other_move)
        self.my_results.append(my_move)

    def all_def(self):
        for i in self.other_results:
            if i != 'D':
                return False
        return True

    def all_coop(self):
        for i in self.other_results:
            if i != 'C':
                return False
        return True

    def is_tft(self):
        if self.other_results[0] != 'C':
            return False

        for i in range(1, len(self.other_results)):
            if self.other_results[i] != self.my_results[i-1]:
                return False

    def try_determine_strategy(self):

        if len(self.other_results) < 5:
            self.strategy = 'unknown'
            return
        if self.all_def():
            self.strategy = 'ad'
            return
        if self.all_coop():
            self.strategy = 'ac'
            return
        if self.is_tft():
            self.strategy = 'tft'
        self.strategy = 'unknown'

    def play(self):
        if self.strategy == 'unk':
            return self.last_op if self.last_op else 'C'
        if self.strategy == 'ac':
            return 'D'
        if self.strategy == 'ad':
            return 'D'
        if self.strategy == 'random':
            return 'D'
        if self.strategy == 'tft':
            return 'D'
        self.try_determine_strategy()


def create_strategy():
    return MyStrategy()
