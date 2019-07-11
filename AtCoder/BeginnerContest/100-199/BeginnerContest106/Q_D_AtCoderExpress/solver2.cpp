#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
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
    DWORD aadwTrainMap[dwInput_N+1][dwInput_N+1];
    memset(aadwTrainMap, 0, sizeof(aadwTrainMap));
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

    DWORD aadwAnswer[dwInput_N+1][dwInput_N+1];
    for (DWORD dwBegin = 1; dwBegin <= dwInput_N; dwBegin++) {
        for (DWORD dwEnd = 1; dwEnd <= dwInput_N; dwEnd++) {
            DWORD dwAnswer= (aadwTrainMap[dwEnd][dwEnd]
                        - aadwTrainMap[dwBegin-1][dwEnd]
                        - aadwTrainMap[dwEnd][dwBegin-1]
                        + aadwTrainMap[dwBegin-1][dwBegin-1]);
            aadwAnswer[dwBegin][dwEnd] = dwAnswer;
        }
    }


    /* input question */
    char szInputQ[100000 * 8];
    fread(szInputQ, 1, sizeof(szInputQ), stdin);
    char * szStr;
    WORD wBegin = atoi(strtok(szInputQ, " "));
    WORD wEnd   = atoi(strtok(NULL, " \n"));
    printf("%d\n", aadwAnswer[wBegin][wEnd]);

    for (DWORD dwIdx = 1; dwIdx < dwInput_Q; dwIdx++) {
        DWORD dwAnswer;
        WORD wBegin = atoi(strtok(NULL, " \n"));
        WORD wEnd   = atoi(strtok(NULL, " \n"));

        printf("%d\n", aadwAnswer[wBegin][wEnd]);
    }

    return 0;

}
