import random

class Rule():
    def __init__(self):
        self.answer = 'C'  # type: str
        self.weight = 0.0  # type: float
        self.pos = 0  # type: int
        self.expected = 'C'  # type: str

class Member():
    trans_dict = {'C': 0, 'D': 1, 0: 'C', 1: 'D'}

    def __init__(self, rules):
        if isinstance(rules, list):
            self.rules = rules
        else:
            self.rules = [None for _ in range(rules)]  # type: List[Rule]

    def next(self, view):
        answer_weights = [0, 0]
        for rule in self.rules:
            rule_answer_pos = Member.trans_dict[rule.answer]
            rule_match = rule.expected == view[-rule.pos - 1]
            if rule_match:
                answer_weights[rule_answer_pos] += rule.weight
            else:
                answer_weights[1 - rule_answer_pos] += rule.weight
        maxpos = 0 if answer_weights[0] > answer_weights[1] else 1
        to_return = Member.trans_dict[maxpos]
        return to_return

class Valkovic:

    @staticmethod
    def author_name():
        return "Patrik Valkovic"

    @staticmethod
    def strategy_name():
        return "Valkovic"

    def __init__(self):
        m = Member(1)
        m.rules = []
        r = Rule()
        r.weight = 0.5322241295317233
        r.answer = 'D'
        r.expected = 'D'
        r.pos = 3
        m.rules.append(r)
        r = Rule()
        r.weight = 0.2350757719528627
        r.answer = 'D'
        r.expected = 'D'
        r.pos = 0
        m.rules.append(r)
        r = Rule()
        r.weight = 0.30537447427387193
        r.answer = 'D'
        r.expected = 'C'
        r.pos = 4
        m.rules.append(r)
        r = Rule()
        r.weight = 0.5381718393038948
        r.answer = 'C'
        r.expected = 'D'
        r.pos = 1
        m.rules.append(r)
        r = Rule()
        r.weight = 0.4566215818340533
        r.answer = 'C'
        r.expected = 'C'
        r.pos = 1
        m.rules.append(r)
        r = Rule()
        r.weight = 0.09353652729804701
        r.answer = 'D'
        r.expected = 'D'
        r.pos = 1
        m.rules.append(r)
        r = Rule()
        r.weight = 0.31909565277375745
        r.answer = 'C'
        r.expected = 'D'
        r.pos = 5
        m.rules.append(r)
        r = Rule()
        r.weight = 0.5364944935638869
        r.answer = 'D'
        r.expected = 'C'
        r.pos = 5
        m.rules.append(r)
        r = Rule()
        r.weight = 0.4227510699804072
        r.answer = 'C'
        r.expected = 'D'
        r.pos = 0
        m.rules.append(r)
        r = Rule()
        r.weight = 0.6812654563560714
        r.answer = 'D'
        r.expected = 'D'
        r.pos = 2
        m.rules.append(r)
        r = Rule()
        r.weight = -0.004272974048341649
        r.answer = 'C'
        r.expected = 'D'
        r.pos = 0
        m.rules.append(r)
        r = Rule()
        r.weight = 0.6288510202940285
        r.answer = 'D'
        r.expected = 'C'
        r.pos = 0
        m.rules.append(r)

        self.m = m
        self.view = max(map(lambda x: x.pos, self.m.rules)) + 1
        self.history = ['C' for _ in range(self.view)]

    def reset(self):
        self.history = ['C' for _ in range(self.view)]

    def last_move(self, my_move, other_move):
        self.history.append(other_move)
        self.history = self.history[-self.view:]

    def play(self):
        return self.m.next(self.history)

def create_strategy():
    return Valkovic()
