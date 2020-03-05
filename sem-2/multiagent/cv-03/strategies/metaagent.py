import random

class TitForTat:
    @staticmethod
    def author_name():
        return "Martin Pilat"

    @staticmethod
    def strategy_name():
        return "Tit for Tat"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

    def play(self):
        return self.last_op if self.last_op else 'C'

# play TitForTat but try to exploit very weak agents in the second half of the match
class ExploitativeTitForTat:
    @staticmethod
    def author_name():
        return "Jirka Balhar"

    # @staticmethod
    def strategy_name(self):
        return "Exploitative Tit for Tat"

    def __init__(self):
        self.reset()

    def reset(self):
        self.last_op = None
        self.my_last_op = None
        self.my_last_last_op = None
        self.match_length = 200
        self.round = 0
        self.defect_prob = 0.1
        self.apology = 0
        self.keep_on_tittin = False

    def last_move(self, my_move, other_move):
        self.my_last_last_op = self.my_last_op
        self.my_last_op = my_move
        self.last_op = other_move

    def play(self):
        self.round += 1
        titfortat = self.last_op if self.last_op else 'C'

        # play titfortat for the first part of the round
        if self.round < self.match_length * 0.6 or self.keep_on_tittin:
            # if the opponent defects at any time in this part, keep this strategy for the whole round
            if self.last_op == "D":
                self.keep_on_tittin = True
            return titfortat
        # in the second half try to exploit opponents that are too nice
        else:
            if self.apology > 0:
                if self.my_last_op == "D":
                    return "C" # after my last defect always proceed with cooperate
            
                if self.my_last_last_op == "D" and self.my_last_op == "C":
                    # if the other immediately responded with D (not easily exploited),
                    # continue with TitForTat for the rest of the game (and we owe him one apology)
                    if self.last_op == "D":
                        self.defect_prob *= 0.2
                        self.apology -= 1
                        return "C"
                    else:
                        self.defect_prob *= 1.5
            
            if random.uniform(0, 1) < self.defect_prob:
                # print("try defect")
                self.apology += 1
                return "D"
            else:
                # print("titfortat", titfortat)
                return titfortat
            
def argmax(x):
    return x.index(max(x))

# switch between two kinds of agents: exploitative and cooperative
class MetaAgent:
    @staticmethod
    def author_name():
        return "Jirka Balhar"

    @staticmethod
    def strategy_name():
        return "Meta Agent"

    def __init__(self):
        self.round = 0
        self.match = 0
        self.match_lengths = []
        self.match_length = 200

        self.agents = [TitForTat(), ExploitativeTitForTat()]
        self.current_agent = 0
        self.rewards = [3, 3]
        self.match_reward = 0

        self.epsilon = 0.1

        self.reset()
    
    def reset(self):
        # update tournament estimates
        if self.round > 0:
            self.match += 1
            self.match_lengths.append(self.round)
            self.match_length = sum(self.match_lengths) / self.match

            for ag in self.agents:
                ag.match_length = self.match_length

            # ===== GOOD SETTINGS FOR _repeats_ OUTERLOOP ======
            # self.epsilon *= 0.95
            # alpha = 0.01
            # ===== GOOD SETTINGS FOR _repeats_ INNERLOOP ======
            alpha = 1.0
            # ===== END SETTINGS =====

            # update rewards according to the last match
            self.rewards[self.current_agent] += ((self.match_reward / self.round) - self.rewards[self.current_agent]) * alpha

        self.round = 0
        self.match_reward = 0

        for ag in self.agents:
            ag.reset()

        if random.uniform(0, 1) < self.epsilon:
            self.current_agent = random.randint(0, len(self.agents) - 1)
            # print("picking at random", self.current_agent)
        else:
            self.current_agent = argmax(self.rewards)
            # print(self.rewards, "picking", self.current_agent)

    def last_move(self, my_move, other_move):
        moves = {
            "CC": 3,
            "CD": 0,
            "DC": 5,
            "DD": 1,
        }
        self.match_reward += moves[my_move+other_move]

        for ag in self.agents:
            ag.last_move(my_move, other_move)

    def play(self):
        self.round += 1
        return self.agents[self.current_agent].play()

def create_strategy():
    return MetaAgent()