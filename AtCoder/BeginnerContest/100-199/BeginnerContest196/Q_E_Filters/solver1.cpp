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
#include <list>
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
using LDOUBLE = long double;
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

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) { return 0 < a ? a : -a; }

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

static inline void inputStringSpSeparated(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c) || (' ' == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}

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
    bool bRead = false;
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
            bRead = true;
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
    bool bRead = false;
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
            bRead = true;
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
    bool bRead = false;
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
            bRead = true;
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

/*----------------------------------------------*/

/*----------------------------------------------*/

struct ST_FILTER {
    SQWORD sqThresholdBase;

    ST_FILTER() {
        sqThresholdBase = 0;
    }
};

int main(void)
{
    SQWORD sqN = inputSQWORD();

    SQWORD sqPosOffset = 0;
    ST_FILTER stUpper;
    ST_FILTER stLower;
    
    stUpper.sqThresholdBase = 1000000000000000;
    stLower.sqThresholdBase = -1000000000000000;
//    printf("--- %lld %lld\n", stUpper.sqThresholdBase, stLower.sqThresholdBase);

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqT = inputSQWORD();

        SQWORD sqLastThresholdLower = stLower.sqThresholdBase + sqPosOffset;
        SQWORD sqLastThresholdUpper = stUpper.sqThresholdBase + sqPosOffset;
//        printf("--- %lld %lld\n", sqLastThresholdLower, sqLastThresholdUpper);

        switch(sqT) {
        case 1:
            /* x + ai */
            sqPosOffset += sqA;
            break;
        case 2:
            /* max(x, ai) */
            {
                if (sqLastThresholdLower < sqA) {
                    stLower.sqThresholdBase += (sqA - sqLastThresholdLower);
//                    printf("low>>>%lld\n", stLower.sqThresholdBase);
                }
                if (stUpper.sqThresholdBase < stLower.sqThresholdBase) {
                    stUpper.sqThresholdBase = stLower.sqThresholdBase;
                }
            }
            break;
        case 3:
            /* min(x, ai) */
            {
                if (sqA < sqLastThresholdUpper) {
                    stUpper.sqThresholdBase += (sqA - sqLastThresholdUpper);
//                    printf("up>>>%lld\n", stUpper.sqThresholdBase);
                }
                if (stUpper.sqThresholdBase < stLower.sqThresholdBase) {
                    stLower.sqThresholdBase = stUpper.sqThresholdBase;
                }
            }
            break;
        }
//        printf("%lld [%lld %lld]\n", sqPosOffset, stLower.sqThresholdBase, stUpper.sqThresholdBase);
    }

    SQWORD sqQ = inputSQWORD();
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqAns = 0;
        if (sqX < stLower.sqThresholdBase) {
            sqAns = stLower.sqThresholdBase + sqPosOffset;
        } else if (stUpper.sqThresholdBase < sqX) {
            sqAns = stUpper.sqThresholdBase + sqPosOffset;
        } else {
            sqAns = sqX + sqPosOffset;
        }
        printf("%lld ", sqAns);
    }
    printf("\n");
    return 0;
}
