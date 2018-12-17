#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
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

static void printMatrix(DWORD **p, DWORD dwSize)
{
    printf("---\n");
    for (DWORD i = 0; i < dwSize; i++) {
        for(DWORD j = 0; j < dwSize; j++) {
            printf("%5d", p[i*dwSize + j]);
        }
        cout << endl;
    }
    printf("---\n");
}

int main()
{
    DWORD dwInput_N, dwInput_M, dwInput_Q;
    char str[200];

    fgets(str, sizeof(str), stdin);
    dwInput_N = atoi(strtok(str, " "));
    dwInput_M = atoi(strtok(NULL, " "));
    dwInput_Q = atoi(strtok(NULL, " "));

    /* input train */
    static DWORD aadwTrainMap[501][501];
    for (DWORD dwIdx = 0; dwIdx < dwInput_M; dwIdx++) {
        fgets(str, sizeof(str), stdin);
        WORD wCity_L, wCity_R;

        wCity_L = atoi(strtok(str, " "));
        wCity_R = atoi(strtok(NULL, " "));
        aadwTrainMap[wCity_L][wCity_R]++;
    }

    /* calc camsum(Row, Col) */
    for (DWORD dwRow = 1; dwRow <= dwInput_N; dwRow++) {
        DWORD dwSum = 0;
        for (DWORD dwCol = 1; dwCol <= dwInput_N; dwCol++) {
            dwSum += aadwTrainMap[dwRow][dwCol];
            aadwTrainMap[dwRow][dwCol] = dwSum + aadwTrainMap[dwRow-1][dwCol];
        }
    }

    /* input question */
    for (DWORD dwIdx = 0; dwIdx < dwInput_Q; dwIdx++) {
        DWORD dwAnswer;
        fgets(str, sizeof(str), stdin);
        WORD wBegin = atoi(strtok(str, " "));
        WORD wEnd   = atoi(strtok(NULL, " "));

        dwAnswer= (aadwTrainMap[wEnd][wEnd]
                    - aadwTrainMap[wBegin-1][wEnd]
                    - aadwTrainMap[wEnd][wBegin-1]
                    + aadwTrainMap[wBegin-1][wBegin-1]);
        printf("%d\n", dwAnswer);
    }

    return 0;

}
