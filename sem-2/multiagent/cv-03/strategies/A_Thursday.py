# -*- coding: utf-8 -*-
"""
Created on Thu Feb 27 19:39:15 2020

@author: seric
"""

import random

class A_Thursday:

    @staticmethod
    def author_name():
        return "Shubham"

    @staticmethod
    def strategy_name():
        return "A_Thursday"

    def __init__(self):
        self.reset()


    def reset(self):
        self.last_op = None

    def last_move(self, my_move, other_move):
        self.last_op = other_move
    
    def play(self):
               
        
        for i in range (1,2):
             j=0
             if self.last_op=='C':
              j=j+1
             if j==2: 
                return 'D'
             else:
                
                return self.last_op if self.last_op else 'C'

def create_strategy():
    return A_Thursday()