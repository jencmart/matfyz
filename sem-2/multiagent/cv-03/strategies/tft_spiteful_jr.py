class TFTSpitefulJR:

    @staticmethod
    def author_name():
        return "Jana Rezabkova"

    @staticmethod
    def strategy_name():
        return "TFT Spiteful JR"

    def __init__(self):
        self.reset()

    def reset(self):
        self.second = None
        self.first = None
        self.spiteful = False

    def last_move(self, my_move, other_move):
        self.second = self.first
        self.first = other_move
        if self.second == "D" and self.first == "D":
            self.spiteful = True


    def play(self):
        if self.spiteful:
            return "D"
        else:
            return self.first if self.first else 'C'

def create_strategy():
    return TFTSpitefulJR()