#include <iostream>
#include <string.h>
using namespace std;
typedef unsigned long long DWORD;
typedef struct {
    DWORD dwWeight;
    DWORD dwValue;
} KNAPSACK_OBJ_ST;

#define OBJ_NUM_MAX 200

inline DWORD MAX(DWORD a, DWORD b) 
{ 
    return a > b ? a : b; 
}

DWORD get_max_value(
    DWORD dwCurObjIdx,
    KNAPSACK_OBJ_ST *pstObjs,
    DWORD dwRestWeight)
{
    if (0 == dwCurObjIdx) {
        if (pstObjs[dwCurObjIdx].dwWeight <= dwRestWeight) {
            return pstObjs[dwCurObjIdx].dwValue;
        } else {
            return 0;
        }
    }
    if (dwRestWeight < pstObjs[dwCurObjIdx].dwWeight) {
        return get_max_value(dwCurObjIdx-1, pstObjs, dwRestWeight);
    } else {
        DWORD dwValue1, dwValue2;
        dwValue1 = get_max_value(dwCurObjIdx-1, pstObjs, dwRestWeight-pstObjs[dwCurObjIdx].dwWeight) + pstObjs[dwCurObjIdx].dwValue;
        dwValue2 = get_max_value(dwCurObjIdx-1, pstObjs, dwRestWeight);
        return MAX(dwValue1, dwValue2);
    }
}


int main() {
    char str[30], *token1, *token2;
    DWORD num_objects;
    DWORD knapsack_weight;
    KNAPSACK_OBJ_ST astObjs[OBJ_NUM_MAX];

    cin.getline(str, sizeof(str));
    num_objects     = atoi(strtok(str, " "));
    knapsack_weight = atoi(strtok(NULL, " "));

    for(int objIdx = 0; objIdx<num_objects; objIdx++) {
        cin.getline(str, sizeof(str));
        astObjs[objIdx].dwValue = atoi(strtok(str, " "));
        astObjs[objIdx].dwWeight = atoi(strtok(NULL, " "));
    }

    DWORD answer;
    answer = get_max_value(num_objects - 1, astObjs, knapsack_weight);
    printf("%llu\n", answer);

    return 0;
}