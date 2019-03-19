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


#define MAX_STONES  (200000)
#define MAX_COLOR   (200000)
static SQWORD s_asqLeftCnt[MAX_COLOR + 1];
static SQWORD s_asqLastIdx[MAX_STONES + 1];
static SQWORD s_asqCumSum[MAX_STONES + 1];
#define ANS_MOD         (1000000007)


int main()
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqCumSumCur = 0;
    SQWORD sqLastColor = 0;
    for (SQWORD sqIdx = 1; sqIdx <= sqInput_N; sqIdx++) {
        SQWORD sqColor = inputSQWORD();

        if (sqLastColor != sqColor) {
            if (0 < s_asqLastIdx[sqColor]) {
                SQWORD sqPairIdx = s_asqLastIdx[sqColor];
//                SQWORD sqPairIncrease = s_asqCumSum[sqPairIdx] + 1;
                SQWORD sqPairIncrease = s_asqLeftCnt[sqColor] + 1;
//                printf("idx[%lld] p: color[%lld], pair[%lld], inc[%lld]\n", sqIdx, sqColor, sqPairIdx, sqPairIncrease);

                s_asqCumSum[sqIdx] = (s_asqCumSum[sqIdx - 1] + sqPairIncrease) % ANS_MOD;
                s_asqLeftCnt[sqColor] = (s_asqLeftCnt[sqColor] + s_asqCumSum[sqIdx]) % ANS_MOD;
//                printf("  sum[%lld], left[%lld]\n", s_asqCumSum[sqIdx], s_asqLeftCnt[sqColor]);
            } else {
                s_asqCumSum[sqIdx] = s_asqCumSum[sqIdx - 1];
                s_asqLeftCnt[sqColor] = s_asqCumSum[sqIdx];
//                printf("idx[%lld]\n", sqIdx);
//                printf("  sum[%lld], left[%lld]\n", s_asqCumSum[sqIdx], s_asqLeftCnt[sqColor]);
            }
            /* update last idx */
            s_asqLastIdx[sqColor] = sqIdx;
        } else {
            s_asqCumSum[sqIdx] = s_asqCumSum[sqIdx - 1];
            s_asqLeftCnt[sqColor] = s_asqCumSum[sqIdx];
//            printf("idx[%lld]\n", sqIdx);
//            printf("  sum[%lld], left[%lld]\n", s_asqCumSum[sqIdx], s_asqLeftCnt[sqColor]);
        }

        sqLastColor = sqColor;
    }
    printf("%lld\n", s_asqCumSum[sqInput_N] + 1) % ANS_MOD;

    return 0;
}