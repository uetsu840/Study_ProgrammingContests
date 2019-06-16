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

typedef enum {
    RATE_GOLD   = 0,
    RATE_SILVER,
    RATE_BLONZE,
    RATE_NUM
} CHG_RATE_TYPE;

typedef struct {
    SQWORD asqRate[RATE_NUM];
} ST_CHG_RATE;

static void getInput(ST_CHG_RATE *pstRate)
{
    pstRate->asqRate[RATE_GOLD]   = inputSQWORD();
    pstRate->asqRate[RATE_SILVER] = inputSQWORD();
    pstRate->asqRate[RATE_BLONZE] = inputSQWORD(); 
}

#define MAX_N       (5000)
#define MAX_RATE    (5000)

/**
 *  交換所を2回通った時のドングリの個数を求める。
 */
static SQWORD getMaximumChange(
    SQWORD sqInitD,
    const ST_CHG_RATE *pstChgM, 
    const ST_CHG_RATE *pstChgD)
{
    /* 
     * DP[i] 金・銀・銅に交換するドングリ i グラムを考慮したときの、
     * 交換先で得られるドングリの最大数 
     * 
     *  DP[i] = max(DP[i - g_a] + g_b, DP[i - s_a] + s_b, DP[i - b_a] + b_b)
     */
    static SQWORD s_asqDpTbl[MAX_N * MAX_N];
    for (SQWORD sqIdx = 0; sqIdx < ArrayLength(s_asqDpTbl); sqIdx++) {
        s_asqDpTbl[sqIdx] = 0;
    }

    for (SQWORD sqDpTblIdx = 1; sqDpTblIdx <= sqInitD; sqDpTblIdx++) {
        SQWORD sqNextDpNum = sqDpTblIdx;
        for (SQWORD sqChgMaterialIdx = 0; sqChgMaterialIdx < ArrayLength(pstChgM->asqRate); sqChgMaterialIdx++) {
            SQWORD sqChgDNum = pstChgM->asqRate[sqChgMaterialIdx];
            if (sqChgDNum <= sqDpTblIdx) {
                sqNextDpNum = max(sqNextDpNum, 
                                  s_asqDpTbl[sqDpTblIdx - sqChgDNum] + pstChgD->asqRate[sqChgMaterialIdx]);
            }
        }
        s_asqDpTbl[sqDpTblIdx] = sqNextDpNum;
    }

    return s_asqDpTbl[sqInitD];
}

int main()
{
    ST_CHG_RATE stRateA;
    ST_CHG_RATE stRateB;

    SQWORD sqInput_N = inputSQWORD();
    getInput(&stRateA);
    getInput(&stRateB);

    SQWORD sqMidNumD   = getMaximumChange(sqInput_N, &stRateA, &stRateB);
    SQWORD sqFinalNumD = getMaximumChange(sqMidNumD, &stRateB, &stRateA); 

    printf("%lld\n", sqFinalNumD);

    return 0;
}