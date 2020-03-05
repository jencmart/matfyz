class SpitefulTFTJones:

    @staticmethod
    def author_name():
        return "Jan Jones"

    @staticmethod
    def strategy_name():
        return "Spiteful TFT (Jones)"

    def __init__(self):
        self.reset()

    def reset(self):
        self.always_d = False
        self.last_op = None

    def last_move(self, my_move, other_move):
        if self.last_op == 'D' and other_move == 'D':
            self.always_d = True
        self.last_op = other_move

    def play(self):
        if self.always_d:
            return 'D'
        return self.last_op if self.last_op else 'C'


def create_strategy():
    return SpitefulTFTJones()
