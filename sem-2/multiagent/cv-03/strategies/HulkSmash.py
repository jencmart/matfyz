class HulkSmash:
    """
    Tries to cooperate, unless opponent defects -> defects n-times after n-th defection (gets angrier over time), 
    but tries to calm things down afterwards with 2 cooperations.
    Gets upset if chosen threshold is reached -> after that always defects (maximum anger reached: Time to smash).
    """

    @staticmethod
    def author_name():
        return "Marinco Möbius"

    @staticmethod
    def strategy_name():
        return "HulkSmash"

    def __init__(self, upset):
        self.reset()
        self.upset = upset  # threshold value -> choose carefully

    def reset(self):
        self.last_op = None  # last move of opponent
        self.defection_count = 0  # keeps track how often opponent has defected
        self.moves = []  # queue for own moves to play
        self.memory = list()  # store for past moves of opponent

    def last_move(self, my_move, other_move):
        self.last_op = other_move
        self.memory.append(other_move)

    def play(self):
        if self.moves:
            return self.moves.pop(0)
        elif self.defection_count >= self.upset:
            # Hulk angry -> time to smash
            return 'D'
        elif self.last_op == 'D':
            # Hulk getting more and more annoyed
            self.defection_count += 1
            for _ in range(self.defection_count):
                self.moves.append('D')
            self.moves.append('C')
            self.moves.append('C')
            return self.moves.pop(0)
        else:
            # Hulk happy
            return 'C'

def create_strategy():
    return HulkSmash(upset=2)