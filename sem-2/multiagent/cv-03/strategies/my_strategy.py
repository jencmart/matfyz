import random

class MyStrategy:

    @staticmethod
    def author_name():
        return "Jan Kočur"

    @staticmethod
    def strategy_name():
        return "R3m3mb3r"

    def __init__(self):
        self.reset()

        # Maximum number of moves that will get buffered
        # My + opponents
        self.MAXIMUM_BUFFER_LENGTH = 4
        
        self.moves = {}
        self.buffer = []

    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move

        self.buffer.append(my_move)

        # Calculate the score
        if len(self.buffer) >= self.MAXIMUM_BUFFER_LENGTH:
            score = self.get_score(my_move, other_move)
            self.update_buffer(score)

    def update_buffer(self, score):
        if len(self.buffer) > self.MAXIMUM_BUFFER_LENGTH:
            self.buffer = self.buffer[-self.MAXIMUM_BUFFER_LENGTH:]
        buffer_string = "".join(self.buffer)

        # Hold info about last n moves
        self.moves[buffer_string] = score
   
        
    def get_score(self, m1, m2):
        if m1 not in ['C', 'D']:
            return -1

        if m2 not in ['C', 'D']:
            return 5
        
        if m1 == 'C':
            return 0 if m2 == 'D' else 3
        if m1 == 'D':
            return 1 if m2 == 'D' else 5

    def play(self):
        # Consider all the moves and return the max
        best_move = self.last_op if self.last_op else 'C'
        best_score = -1

        # Find the move using the scores
        if len(self.buffer) > self.MAXIMUM_BUFFER_LENGTH:
            buffer_copy = self.buffer[self.MAXIMUM_BUFFER_LENGTH-2:]
            buffer_copy.append(self.last_op)
        
            c_entry = "".join(buffer_copy) + 'C'
            d_entry = "".join(buffer_copy) + 'D'
        
            # If we have both the entries do:
            if c_entry in self.moves and d_entry in self.moves:
                for i in ['C', 'D']:
                    considered_move = buffer_copy
                    considered_move_string = "".join(buffer_copy) + i

                    if considered_move_string in self.moves:
                        score = self.moves[considered_move_string]
                        if score > best_score:
                            best_score = score
                            best_move = i

        # Append the enemy move
        if self.last_op != None:
            self.buffer.append(self.last_op)
        return best_move

def create_strategy():
    return MyStrategy()
