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
        self.objects = np.zeros([object_num, 2], dtype=np.uint64)
        self.IDX_VALUE  = 0
        self.IDX_WEIGHT = 1
        self.values_memo_weight = np.array(())

    def set_object(self, idx, value, weight):
        # print('idx:', idx, 'value:', value, 'weight:', weight)
        self.objects[idx, self.IDX_VALUE] = value
        self.objects[idx, self.IDX_WEIGHT] = weight

    def init_for_search(self, knapsack_weight):
        # sort objects with weight order (descend)
        sort_col_idx = self.IDX_WEIGHT
        self.objects = self.objects[np.argsort(self.objects[:, sort_col_idx])[::-1]]
        max_weight = self.objects[:, self.IDX_WEIGHT].max()
        max_value  = self.objects[:, self.IDX_VALUE].max()
#        print(max_weight, max_value)
        if max_weight < 1000:
            self.values_memo_weight = np.full((len(self.objects[:, 1]), knapsack_weight+1), -1, dtype=np.int64)
        else:
            self.values_memo_weight = None

    def get_max_value(self, cur_obj_idx, rest_weight):
        if self.values_memo_weight is not None:
            if -1 != self.values_memo_weight[cur_obj_idx, rest_weight]:
                return self.values_memo_weight[cur_obj_idx, rest_weight]

        cur_weight = self.objects[cur_obj_idx, self.IDX_WEIGHT]
        cur_value  = self.objects[cur_obj_idx, self.IDX_VALUE]
        if 0 == cur_obj_idx:
            if cur_weight <= rest_weight:
                ret_val = cur_value
            else:
                ret_val = 0
        else:
            if rest_weight < cur_weight:
                ret_val = 0
            else:
                value1 = self.get_max_value(cur_obj_idx-1, int(rest_weight-cur_weight)) + cur_value
                value0 = self.get_max_value(cur_obj_idx-1, int(rest_weight))
                ret_val = max(value0, value1)
        if self.values_memo_weight is not None:
            self.values_memo_weight[cur_obj_idx, rest_weight] = ret_val
        return ret_val

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

    objs.init_for_search(ttl_weight_max)
    print(int(objs.get_max_value(object_num-1, ttl_weight_max)))

inputs = paiza_fmt_input()
inputs.input_all()
solve(inputs)
