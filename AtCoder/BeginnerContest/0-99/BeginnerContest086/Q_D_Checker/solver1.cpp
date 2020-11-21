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
#include <assert.h>
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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }

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

#define MAX_H   (2000)
#define MAX_W   (2000)

static SQWORD s_aasqColCnt[MAX_H + 2][MAX_W + 2];

static SQWORD getSum(SQWORD sqX1, SQWORD sqY1, SQWORD sqX2, SQWORD sqY2)
{
    SQWORD sqSum = s_aasqColCnt[sqX2][sqY2] 
                    - s_aasqColCnt[sqX2][sqY1]
                    - s_aasqColCnt[sqX1][sqY2]
                    + s_aasqColCnt[sqX1][sqY1];
//    printf(">%lld ", sqSum);
    return sqSum;
}

int main()
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqK = inputSQWORD();
    SQWORD sqCycle = sqK * 2;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        char c = getchar();

        SQWORD sqPhaseX, sqPhaseY;
        if ('W' == c) {
            sqPhaseX = sqX % sqCycle;
            sqPhaseY = sqY % sqCycle;
        } else if ('B' == c) {
            sqPhaseX = (sqX + sqK) % sqCycle;
            sqPhaseY = sqY % sqCycle;
        } else {
            assert(false);
            return -1;
        }
//        printf("[%lld %lld]\n", sqPhaseX + 1, sqPhaseY + 1);
        s_aasqColCnt[sqPhaseX + 1][sqPhaseY + 1]++;
    }

    for (SQWORD sqX = 1; sqX <= sqCycle; sqX++) {
        for (SQWORD sqY = 1; sqY <= sqCycle; sqY++) {
            s_aasqColCnt[sqX][sqY] += s_aasqColCnt[sqX][sqY - 1];
        }
    }
    for (SQWORD sqX = 1; sqX <= sqCycle; sqX++) {
        for (SQWORD sqY = 1; sqY <= sqCycle; sqY++) {
            s_aasqColCnt[sqX][sqY] += s_aasqColCnt[sqX - 1][sqY];
//            printf("%lld ", s_aasqColCnt[sqX][sqY]);
        }
//        printf("\n");
    }
    
    SQWORD sqAns = 0;
    for (SQWORD sqX = 0; sqX <= sqK; sqX++) {
        for (SQWORD sqY = 0; sqY <= sqK; sqY++) {
            SQWORD sqSum = 0;
            sqSum += getSum(0,         0,         sqX,     sqY);
            sqSum += getSum(0,         sqY + sqK, sqX,     sqCycle);
            sqSum += getSum(sqX + sqK, 0,         sqCycle, sqY);
            sqSum += getSum(sqX + sqK, sqY + sqK, sqCycle, sqCycle);
            sqSum += getSum(sqX,       sqY,       sqX + sqK, sqY + sqK);
//            printf("%lld\n", sqSum);
            sqAns = MAX(sqAns, sqSum);
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}