using namespace std;
#include <iostream>
#include <algorithm> // sort, for_each, random_shuffle
#include <stdlib.h>
#include <string.h>
#include <math.h>

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)

#define MAX_STATION_NUM (1000000)

static DWORD s_adwDpMap[MAX_STATION_NUM];

int main()
{
    DWORD dwInput_N, dwInput_K;
    DWORD dwObjMap=0;
    DWORD dwObjPos;
    DWORD dwCumSum;
    char str[300];

    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));
    dwInput_K = atoi(strtok(NULL, " "));

    if (dwInput_K == 2) {
        s_adwDpMap[1] = 1;
        s_adwDpMap[2] = 0;
        dwCumSum = 1;
    } else {
        s_adwDpMap[1] = 1;
        s_adwDpMap[2] = 1;
        dwCumSum = 2;
    }

    for (DWORD dwAddStationIdx = 3; dwAddStationIdx <= dwInput_N; dwAddStationIdx++) {
        DWORD dwSum = 0;
        DWORD dwVal;
        dwSum = dwCumSum; 
        if (dwInput_K <= dwAddStationIdx) {
            dwVal = (dwSum + 1000000007 - s_adwDpMap[dwAddStationIdx - dwInput_K + 1]) % 1000000007;
//            printf("A: %d %d\n", dwAddStationIdx, dwVal);
        } else {
            dwVal = dwSum;
//            printf("B: %d %d\n", dwAddStationIdx, dwVal);
        }
        s_adwDpMap[dwAddStationIdx] = dwVal;
        dwCumSum = (dwCumSum + dwVal) % 1000000007;
    }
    printf("%d\n", s_adwDpMap[dwInput_N]);

    return 0;
}
