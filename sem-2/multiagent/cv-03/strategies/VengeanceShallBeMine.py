
# u-pl4.ms.mff.cuni.cz:/tmp/mas/strategies/
# na windowsech WinSCP

# An extension of tit-for-tat: the player cooperates, and if the opponent defects, he will defect for n turns and then
# cooperate twice, where n is the number of defections that the opponent has made in total
class VengeanceShallBeMine:

    @staticmethod
    def author_name():
        return "Jaroslav Šafář"

    @staticmethod
    def strategy_name():
        return "VengeanceShallBeMine"

    # constructor
    def __init__(self):
        self.opponent_defects = 0
        self.remaining_defects = 0
        self.remaining_cooperates = 0

    def reset(self):
        self.opponent_defects = 0
        self.remaining_defects = 0
        self.remaining_cooperates = 0

    def last_move(self, my_move, other_move):

        # If you played 'D' and it was the last 'D' in your revenge then finish the revenge with 2 'C'
        if my_move == 'D' and self.remaining_defects == 0:
            self.remaining_cooperates = 2

        # If the opponent played 'D'
        if other_move == 'D':
            self.opponent_defects += 1

            # And if you are finished with your revenge from the last time he played 'D' then start a new revenge
            if self.remaining_defects == 0 and self.remaining_cooperates == 0:
                self.remaining_defects = self.opponent_defects

    def play(self):
        # If the opponent played 'D' then play as many 'D' as he played in the whole game together
        if self.remaining_defects > 0:
            self.remaining_defects -= 1
            return 'D'

        # If you played 'D's as payback then play at least 2 'C'
        if self.remaining_cooperates > 0:
            self.remaining_cooperates -= 1
            return 'C'

        # Else be nice and play 'C'
        return 'C'

def create_strategy():
    return VengeanceShallBeMine()
