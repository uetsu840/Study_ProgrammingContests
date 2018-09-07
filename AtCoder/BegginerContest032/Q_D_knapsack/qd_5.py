import sys
import numpy as np
from ortoolpy import knapsack


class paiza_fmt_input:
    def __init__(self):
        self.cur = 0
        self.all_input = []

    def input_all(self):
        for line in sys.stdin:
            self.all_input.append(line)

    def get_one_line_with_split(self):
        input_cur = self.all_input[self.cur]
        self.cur += 1
        splitted_input = input_cur.split()
        return splitted_input


def solve(inputs):
    line1_input = list(map(int, inputs.get_one_line_with_split()))
    object_num = line1_input[0]
    ttl_weight_max = line1_input[1]

    value = []
    weight = []

    for obj_idx in range(object_num):
        line_input = list(map(int, inputs.get_one_line_with_split()))
        o_value = line_input[0]
        o_weight = line_input[1]
        value.append(o_value)
        weight.append(o_weight)
    print(int(knapsack(weight, value, ttl_weight_max)[0]))


inputs = paiza_fmt_input()
inputs.input_all()
solve(inputs)
