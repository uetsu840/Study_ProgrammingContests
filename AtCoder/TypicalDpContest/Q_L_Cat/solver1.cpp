using namespace std;

#include <iostream>
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

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)

const int MAX_FACTORY(60);

typedef struct {
    DWORD elem_2;
    DWORD elem_3;
    DWORD elem_5;
} DICE_PRIME_ELEM;

#define LOW_MAX (1000)
#define COL_MAX (1000)

static SDWORD s_aalF[LOW_MAX][COL_MAX];
static SDWORD s_aalLossMatrix[LOW_MAX][COL_MAX];

static void printMatrix(SDWORD pMat[][1000], DWORD dwInput_N)
{
    for (DWORD i = 0; i<dwInput_N; i++) {
        for (DWORD j=0; j<dwInput_N; j++) {
            printf("%d ", pMat[i][j]);
        }
        printf("\n");
    }
}

static DWORD calcMinimumLoss(DWORD dwInput_N)
{
    memset(s_aalLossMatrix, 0, sizeof(s_aalLossMatrix));
    for (DWORD dwLow=1; dwLow<dwInput_N; dwLow++) {
        SDWORD lCurLowSum = 0;
        for (DWORD dwCol=0; dwCol < dwLow; dwCol++) {
            SDWORD lUpperLowMinValue = 0;
            lCurLowSum += s_aalF[dwLow][dwCol];
            for (DWORD dwColUpper = 0; dwColUpper <= dwCol; dwColUpper++) {
                lUpperLowMinValue = MIN(lUpperLowMinValue, s_aalLossMatrix[dwLow-1][dwColUpper]);
            }
            s_aalLossMatrix[dwLow][dwCol] = lUpperLowMinValue + lCurLowSum;        
       }
    }
}


int main()
{
    DWORD dwInput_N;
    QWORD qwInput_D;
    DICE_PRIME_ELEM stElem_D;
    char str[6000];

    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));

    memset(s_aalF, 0, sizeof(s_aalF));

    for (DWORD dwLow = 0; dwLow < dwInput_N; dwLow++) {
        cin.getline(str, sizeof(str));
        s_aalF[dwLow][0] = atoi(strtok(str, " "));
        for (DWORD dwCol = 1; dwCol < dwInput_N; dwCol++) {
            s_aalF[dwLow][dwCol] = atoi(strtok(NULL, " "));
        }
    }

    calcMinimumLoss(dwInput_N);

    /* calc sum */
    SDWORD lSum = 0;
    for (DWORD dwLow=0; dwLow<dwInput_N; dwLow++) {
        for (DWORD dwCol=0; dwCol<dwLow; dwCol++) {
            lSum += s_aalF[dwLow][dwCol];
        }
    }

    /* calc minimum loss */
    SDWORD lMinimumLoss = 0;
    for (DWORD dwCol = 0; dwCol<dwInput_N; dwCol++) {
        lMinimumLoss = MIN(lMinimumLoss, s_aalLossMatrix[dwInput_N-1][dwCol]);
    }

    printf("%d\n", (lSum - lMinimumLoss) * 2);
    return 0;
}
