using namespace std;

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using QWORD = unsigned long long;
using SDWORD = int;
using DWORD = unsigned int;

const int MAX_FACTORY(60);

inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
inline QWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }

using   M_BOOL = bool;
#define M_TRUE  (true)
#define M_FALSE (false)

M_BOOL calc_PrimeFactorization(DWORD dwNum, DWORD *pdwDivisorSum)
{
    DWORD dwCurNum = dwNum;
    DWORD dwCeiling = MIN(dwNum / 2, MAX_FACTORY);

    *pdwDivisorSum = 0;
    for (DWORD dwIdx = 2; dwIdx <= dwCeiling; dwIdx++) {
        while(1) {
            if (0 != dwCurNum % dwIdx) {
                break;
            }
            dwCurNum /= dwIdx;
            (*pdwDivisorSum) += dwIdx;
            if (1 == dwCurNum) {
                break;
            }
        }
    }
    if (1 != dwCurNum) {
        (*pdwDivisorSum) += dwCurNum;
    }
    return M_TRUE;
}

typedef enum {
    VAL_SPLIT_NONE = 0,
    VAL_SPLIT_PARALLEL,
    VAL_SPLIT_SERIES
} VAL_SPLIT_TYPE;

typedef struct {
    DWORD dwLineNum;
    VAL_SPLIT_TYPE eSplitType;
    DWORD dwMinSeriesSplit;
} VALUE_SPLIT_ST;

#define LINE_NUM_NONE   ((DWORD)-1)
#define L_MAX           (1000000)
static VALUE_SPLIT_ST s_astValueSplit[L_MAX];

/**
 * series split(sum)
 */
static DWORD get_series_split(DWORD dwPathNum, DWORD *pdwSplitVal)
{
    *pdwSplitVal = 0;

    DWORD dwLineNumSeries = LINE_NUM_NONE;
    DWORD dwSearchCeiling = sqrt(dwPathNum + 1);
    if (4 <= dwPathNum) {
        for (DWORD dwPathSplit1 = 2; dwPathSplit1 <= dwSearchCeiling; dwPathSplit1++) {
            if (0 == dwPathNum % dwPathSplit1) {
                DWORD dwPathSplit2 = dwPathNum / dwPathSplit1;
                DWORD dwLineNumSum = 0;
                DWORD dwLineNum1, dwLineNum2;

                dwLineNum1 = s_astValueSplit[dwPathSplit1].dwLineNum;
                dwLineNum2 = s_astValueSplit[dwPathSplit2].dwLineNum;
                dwLineNumSum = dwLineNum1 + dwLineNum2;
                if (dwLineNumSum < dwLineNumSeries) {
                    *pdwSplitVal = dwPathSplit1;
                    dwLineNumSeries = dwLineNumSum;
                }
            }
        }
    }
    return dwLineNumSeries;
}

/**
 * parallel(sum) split
 */
static DWORD get_parallel_split(DWORD dwPathNum, DWORD *pdwSplitVal) 
{
    *pdwSplitVal = 0;
    DWORD dwLineNumParallel = LINE_NUM_NONE;
    DWORD dwLineNumTtl;
    if (4 <= dwPathNum) {
        for (DWORD dwPathSplit1 = 2; dwPathSplit1 <= dwPathNum / 2; dwPathSplit1++) {
            DWORD dwPathSplit2 = dwPathNum - dwPathSplit1;
            DWORD dwLineNum1, dwLineNum2;

            dwLineNum1 = s_astValueSplit[dwPathSplit1].dwLineNum;
            dwLineNum2 = s_astValueSplit[dwPathSplit2].dwLineNum;
            dwLineNumTtl = dwLineNum1 + dwLineNum2;
            if (dwLineNumTtl < dwLineNumParallel) {
                *pdwSplitVal = dwPathSplit1;
                dwLineNumParallel = dwLineNumTtl;
            }
        }
    }
    return dwLineNumParallel;
}

static DWORD search_minimum_split(DWORD dwPathNum)
{
    DWORD dwSeriesSplit, dwParallelSplit;
    DWORD dwSplitValSeries, dwSplitValParallel;

    /* multiply (series node) */
    dwSeriesSplit = get_series_split(dwPathNum, &dwSplitValSeries);
//    dwSeriesSplit = VAL_SPLIT_NONE;

    /* sum (parallel node) */
//    dwParallelSplit = get_parallel_split(dwPathNum, &dwSplitValParallel);
    dwParallelSplit = VAL_SPLIT_NONE;

    DWORD dwResult = MIN(dwSeriesSplit, dwParallelSplit);
    if (dwSeriesSplit < dwParallelSplit) {
        s_astValueSplit[dwPathNum].dwLineNum  = dwResult;
        s_astValueSplit[dwPathNum].eSplitType = VAL_SPLIT_SERIES;
        s_astValueSplit[dwPathNum].dwMinSeriesSplit = dwSplitValSeries;
    } else {
        s_astValueSplit[dwPathNum].dwLineNum  = dwResult;
        s_astValueSplit[dwPathNum].eSplitType = VAL_SPLIT_PARALLEL;
        s_astValueSplit[dwPathNum].dwMinSeriesSplit = dwSplitValParallel;
    }

    s_astValueSplit[dwPathNum].dwLineNum = dwResult;
    printf("n: %d l: %d %d %d\n", dwPathNum, dwSeriesSplit, dwParallelSplit, dwResult);
    return dwResult;
}

void solve(DWORD dwPathNumber)
{
    s_astValueSplit[0].dwLineNum = LINE_NUM_NONE;
    s_astValueSplit[1].dwLineNum = LINE_NUM_NONE;
    s_astValueSplit[2].dwLineNum = 2;
    s_astValueSplit[3].dwLineNum = 3;

    DWORD dwLineNum;
    for (DWORD dwIdx = 4; dwIdx < dwPathNumber; dwIdx++) {
        search_minimum_split(dwIdx);
    }
}


int main()
{
    DWORD dwInput_L;
    char str[30];

    cin.getline(str, sizeof(str));
    dwInput_L = atoi(strtok(str, " "));

    printf("L:%d\n", dwInput_L); 
    solve(dwInput_L);

    return 0;
}