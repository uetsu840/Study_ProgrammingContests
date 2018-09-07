import sys
import numpy as np


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


class knapsack_objects:
    def __init__(self, object_num):
        self.objects_value = np.zeros([object_num])
        self.objects_weight = np.zeros([object_num])

    def set_object(self, idx, value, weight):
        # print('idx:', idx, 'value:', value, 'weight:', weight)
        self.objects_value[idx] = value
        self.objects_weight[idx] = weight

    def get_max_value(self, cur_obj_idx, rest_weight):
        if 0 == cur_obj_idx:
            if self.objects_weight[cur_obj_idx] <= rest_weight:
                return self.objects_value[0]
            else:
                return 0
        else:
            if rest_weight < self.objects_weight[cur_obj_idx]:
                return self.get_max_value(cur_obj_idx-1, rest_weight)
            else:
                value1 = self.get_max_value(cur_obj_idx-1, rest_weight-self.objects_weight[cur_obj_idx]) +\
                         self.objects_value[cur_obj_idx]
                value0 = self.get_max_value(cur_obj_idx-1, rest_weight)
                return max(value0, value1)

        get_max_value()

def solve(inputs):
    line1_input = list(map(int, inputs.get_one_line_with_split()))
    object_num = line1_input[0]
    ttl_weight_max = line1_input[1]
    objs = knapsack_objects(object_num)

    for obj_idx in range(object_num):
        line_input = list(map(int, inputs.get_one_line_with_split()))
        o_value = line_input[0]
        o_size = line_input[1]
        objs.set_object(obj_idx, o_value, o_size)

    print(int(objs.get_max_value(object_num-1, ttl_weight_max)))





inputs = paiza_fmt_input()
inputs.input_all()
solve(inputs)
