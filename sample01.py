import sys
import numpy as np
# coding: utf-8
# 自分の得意な言語で
# Let's チャレンジ！！

def solve(inputs):
    cur = 0
    input_cur = inputs[cur]
    cur += 1
#    print(input_cur)
    input_first_line =  input_cur.split()
    n_height  = int(input_first_line[0])
    n_width   = int(input_first_line[1])
    n_players = int(input_first_line[2])
    
    cards = np.zeros((n_height, n_width))
    
    # initialize cards
    for row in range(n_height):
        input_cur = inputs[cur]
        input_cards =  input_cur.split()
        cur += 1
        
        for col in range(n_width):
            cards[row, col] = int(input_cards[col])
    
    # read player logs
    input_cur = inputs[cur]
    cur += 1
    input_log_lines =  input_cur.split()
    n_logs  = int(input_log_lines[0])
    
    player_results = np.zeros(n_players)
    current_player = 0
    
    for turns in range(n_logs):
        input_turns = inputs[cur].split()
        cur += 1
        
#        print (input_turns)
        r1 = int(input_turns[0])
        c1 = int(input_turns[1])
        r2 = int(input_turns[2])
        c2 = int(input_turns[3])
        if (cards[r1 - 1, c1 - 1] == cards[r2 - 1, c2 - 1]):
#            print('hit!')
            player_results[current_player] += 2
        else:
#            print('miss')
            current_player = (current_player + 1) % n_players
    
#    print(player_results)
    return player_results
    
    
    



a = []
for line in sys.stdin:
    a.append(line)

player_results = solve(a)
for i in range(len(player_results)):
    print(int(player_results[i]))

