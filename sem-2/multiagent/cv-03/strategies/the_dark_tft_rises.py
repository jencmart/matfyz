class StubbornTFT:
    failed = False
    D_days = 12
    current_Ddays = 0

    @staticmethod
    def author_name():
        return "Daniel Šipoš"

    @staticmethod
    def strategy_name():
        return "The Dark TFT Rises"

    def __init__(self):
        self.reset()

    def reset(self):
        self.failed = False
        self.current_Ddays = 0
        self.second_last_op = None
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.second_last_op = self.last_op
        self.last_op = other_move
        if other_move == 'D':
            if self.last_op:
                if self.second_last_op == 'D': 
                    self.current_Ddays = self.D_days
            else:
                self.current_Ddays = self.D_days 
        else: 
            self.current_Ddays -= 1

    def play(self):
        if self.last_op and self.current_Ddays > 0:
            return 'D'
        else:
            return 'C'


def create_strategy():
    return StubbornTFT()