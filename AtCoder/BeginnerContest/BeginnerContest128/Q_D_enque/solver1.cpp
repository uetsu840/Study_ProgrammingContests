#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
using namespace std;

using QWORD  = uint64_t;
using SQWORD = int64_t;
using DWORD  = uint32_t;
using SDWORD = int32_t;
using WORD   = uint16_t;
using SWORD  = int16_t;
using BYTE   = uint8_t;
using SBYTE  = int8_t;
using DOUBLE = double;
using FLOAT  = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0x7FFFFFFFFFFFFFFF)

#define MAX_QWORD  (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD  (0xFFFFFFFF)
#define MAX_WORD   (0xFFFF)
#define MAX_BYTE   (0xFF)

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
#define DIVISOR (1000000007)

static inline void inputString(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}


static inline SQWORD inputSQWORD(void)
{
    SQWORD sqNumber = 0;
    SQWORD sqMultiplier = 1;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                sqMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            sqNumber *= 10LL;
            sqNumber += (SQWORD)(c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return sqNumber * sqMultiplier;
            }
        }
    }
}


static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    SDWORD lMultiplier = 1;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                lMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber * lMultiplier;
            }
        }
    }
}

static inline DOUBLE inputFP(void)
{
    DOUBLE dInt = 0.0;
    DOUBLE dFrac = 0.0;
    DOUBLE dMultiplier = 1.0;
    DWORD dwFpCnt = 0;
    DOUBLE *pdCur = &dInt;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                dMultiplier = -1;
            }
        }
        if ('.' == c) {
            pdCur = &dFrac;
        } else if (('0' <= c) && (c <= '9')) {
            (*pdCur) *= 10;
            (*pdCur) += (DOUBLE)(c - '0');
            bRead = M_TRUE;
            if (pdCur == &dFrac) {
                dwFpCnt++;
            }
        } else {
            if (bRead) {
                return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0 , (DOUBLE)dwFpCnt)));
            }
        }
    }
}

/*----------------------------------------------*/

static SQWORD getMaxScore(
    vector<SQWORD> vecTreasures, 
    SDWORD lDl, 
    SDWORD lDr, 
    SDWORD lNumProc)
{
    vector<SQWORD> vsqGetTreasures;

//    printf("%d - %d\n", lDl, lDr);

    for (SDWORD lIdx = 0; lIdx < lDl; lIdx++) {
        vsqGetTreasures.emplace_back(vecTreasures[lIdx]);
    }
    SDWORD lSize = vecTreasures.size();
    for (SDWORD lIdx = lSize - 1; lSize - lDr - 1 < lIdx; lIdx--) {
        vsqGetTreasures.emplace_back(vecTreasures[lIdx]);
    }

    sort(vsqGetTreasures.begin(), vsqGetTreasures.end());

    SDWORD lMaxDropNum = lNumProc - (lDr + lDl);
//    printf("drop : %d\n", lMaxDropNum);
    for (SDWORD lIdx = 0; lIdx < lMaxDropNum; lIdx++) {
        if (vsqGetTreasures.size() <= lIdx) {
            break;
        }
        if (vsqGetTreasures[lIdx] < 0) {
            vsqGetTreasures[lIdx] = 0;
        }
    }
    SQWORD sqSum = accumulate(vsqGetTreasures.begin(), vsqGetTreasures.end(), 0);
//    printf("%lld\n", sqSum);
    return sqSum;
}


#define SQWORD_INF_N   (-1000000000000)

int main()
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_K = inputSQWORD();
    vector<SQWORD> vsqTreasures;

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        vsqTreasures.emplace_back(inputSQWORD());
    }

    SQWORD sqAns = SQWORD_INF_N;
    for (SDWORD lDl = 0; lDl <= sqInput_K; lDl++) {
        for (SDWORD lDr = 0; lDr <= (min(sqInput_K, sqInput_N) - lDl); lDr++) {
            sqAns = max(sqAns, getMaxScore(vsqTreasures, lDl, lDr, sqInput_K));
        }
    }
    printf("%lld\n", sqAns);

    return 0;
}