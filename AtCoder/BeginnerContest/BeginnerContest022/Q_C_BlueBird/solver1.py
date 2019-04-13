import sys
import numpy as np
import heapq

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


def get_bin_value(np_b_ary):
    ret_val = 0
    for pos, val in enumerate(np_b_ary):
        if 0 < val:
            ret_val = ret_val | 1 << pos
    return ret_val


def get_hash_value(pos, _is_visited):
    pos_val = pos
    is_visited_val = get_bin_value(_is_visited)
    ret_val = (is_visited_val << 9) | pos_val
    return ret_val


def get_next_hash(src_hash, dest_pos_idx):
    is_visited_map   = ((src_hash>>9) & 127)
    is_visited_map   |= (1 << dest_pos_idx)
    ret_val = (is_visited_map << 9) | dest_pos_idx
    return ret_val


def get_pos_idx(hash_value):
    return hash_value & 511


def is_all_visited(hash_value, pos_num):
    all_visit_mask = (1 << pos_num) - 1

    if ((hash_value>>3) & all_visit_mask) == all_visit_mask:
        return True
    else:
        return False


class Dijkstra(object):
    # Q_C Blue Bird 用のダイクストラ法。
    # is_visited を pos として扱うため、変形版。
    def dijkstra_s013(self, pos_num, is_dangerous_init, cost_fwd, cost_bak):

        is_visited_init = np.zeros(pos_num)
        start_hash = get_hash_value(pos_num - 1, is_visited_init)

        cost = {} # 始点から各頂点までの最短距離を格納する
        prev = {} # 最短経路における，その頂点の前の頂点のIDを格納する

        cost[start_hash] = 0
        q = []  # プライオリティキュー．各要素は，(startからある頂点vまでの仮の距離, 頂点vのID)からなるタプル
        heapq.heappush(q, (0, start_hash))  # 始点をpush

        while len(q) != 0:
            prov_cost, src_hash = heapq.heappop(q)  # pop
            cur_pos = get_pos_idx(src_hash)

            if (cur_pos == pos_num - 1) and (is_all_visited(src_hash, pos_num)):
                # get path
                path = [pos_num]
                hash_val = src_hash
                while True:
                    hash_val = prev[hash_val]
                    pos_no = get_pos_idx(hash_val) + 1
                    path.append(pos_no)
                    if pos_no == pos_num:
                        break
                return list(reversed(path))

            # プライオリティキューに格納されている最短距離が，現在計算できている最短距離より大きければ，distの更新をする必要はない
            if cost[src_hash] < prov_cost:
                continue

            for dest_pos_idx in range(pos_num):
                if dest_pos_idx != cur_pos:
                    next_cost = cost_fwd if cur_pos < dest_pos_idx else cost_bak
                    dest_hash = get_next_hash_with_invert(src_hash, dest_pos_idx)
                    if (dest_hash not in cost) or (cost[src_hash] + next_cost < cost[dest_hash]):
                        cost[dest_hash] = cost[src_hash] + next_cost  # costの更新
                        heapq.heappush(q, (cost[dest_hash], dest_hash))  # キューに新たな仮の距離の情報をpush
                        prev[dest_hash] = src_hash  # 前の頂点を記録


def solve(inputs):
    input_first_line = inputs.get_one_line_with_split()
    n_points = int(input_first_line[0])
    n_roads  = int(input_first_line[1])

    for i in range(n_roads):
        input_road = inputs.get_one_line_with_split()
        p0 = int(input_road[0])
        p1 = int(input_road[1])
        dist = int(input_road[2])
        print(p0, p1, dist)

#    dj = Dijkstra()
#    path = dj.dijkstra_s013(n_points, is_dangerous_init, cost_to_future, cost_to_past)
#    print(*path)

inputs = paiza_fmt_input()
inputs.input_all()
solve(inputs)
