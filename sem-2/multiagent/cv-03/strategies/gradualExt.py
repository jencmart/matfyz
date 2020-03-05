import random

# BEAUFILS, B., Delahaye, J.-P. & Mathieu, P. (1996). Our meeting with gradual, a good strategy for the iterated prisoner’s dilemma. In Proceedings of the Fi h International Workshop on the Synthesis and Simulation of Living Systems (ALIFE’5), (pp. 202–209). Boston, MA: The MIT Press/Bradford Books.

class GradualExt:
    # Variable
    step = 0
    chill = False
    punishment = False
    number_of_punishments = 0
    current_punishment = 0
    number_of_oponents_defections = 0
    NUMBER_OF_DEFECTIONS_BEFORE_PERMANENT_PUNISHMENT = 5
    
    @staticmethod
    def author_name():
        return "Petr Illner"

    @staticmethod
    def strategy_name():
        return "Gradual + extension"

    def __init__(self):
        pass

    def reset(self):
        self.step = 0
        self.chill = False
        self.punishment = False
        self.number_of_punishments = 0
        self.current_punishment = 0
        self.number_of_oponents_defections = 0

    def last_move(self, my_move, other_move):
        if not(self.chill) and not(self.punishment) and other_move == 'D':
            self.punishment = True
            self.number_of_punishments += 1
            self.current_punishment = 0

        if other_move == 'D':
            self.number_of_oponents_defections += 1

        if (self.step == self.number_of_oponents_defections == self.NUMBER_OF_DEFECTIONS_BEFORE_PERMANENT_PUNISHMENT):
            self.chill = False
            self.punishment = True
            self.number_of_punishments = 9999999

    def play(self):
        self.step += 1

        if self.chill:
            self.chill = False
            return 'C'

        if self.punishment:
            if self.current_punishment < self.number_of_punishments:
                self.current_punishment += 1
                return 'D'
            else:
                self.chill = True
                self.punishment = False
                self.current_punishment = 0
                return 'C'

        return 'C'

def create_strategy():
    return GradualExt()