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

struct ST_EDGE {
    SQWORD sqTo;
    SQWORD sqW;

    ST_EDGE(SQWORD t, SQWORD w): sqTo(t), sqW(w) {};
};

#define MAX_NODES   (200000)
#define MAX_BITS    (64)
static SQWORD asqOddSum[MAX_BITS];
static SQWORD asqEvenSum[MAX_BITS];
static SQWORD s_aasqSumsEven[MAX_NODES][MAX_BITS];
static SQWORD s_aasqSumsOdd[MAX_NODES][MAX_BITS];
static SQWORD s_aasqReduceSumsOdd[MAX_NODES][MAX_BITS];
static SQWORD s_aasqReduceSumsEven[MAX_NODES][MAX_BITS];

static void addSum(SQWORD sqWeight)
{
    for (SQWORD sqBit = 0; sqBit < MAX_BITS; sqBit++) {
        if (sqWeight & (0x1 << sqBit)) {
            SQWORD sqEven = asqEvenSum[sqBit];
            SQWORD sqOdd = asqOddSum[sqBit];
            asqEvenSum[sqBit] += sqOdd;
            asqOddSum[sqBit] += sqEven;
            asqEvenSum[sqBit]++;
        } else {
            asqEvenSum[sqBit] *= 2;
            asqOddSum[sqBit] *= 2;
            asqOddSum[sqBit]++;
        }
    } 
}

static void addReduceSums(SQWORD sqFrom, SQWORD sqCur)
{
    for (SQWORD sqBit = 0; sqBit < MAX_BITS; sqBit++) {
        s_aasqReduceSumsEven[sqCur][sqBit] += s_aasqSumsEven[sqFrom][sqBit];
        s_aasqReduceSumsOdd[sqCur][sqBit]  += s_aasqSumsOdd[sqFrom][sqBit];
    }
}

static void setSums(SQWORD sqCur) {
    printf("SET: %lld\n", sqCur);
    memcpy(s_aasqSumsEven[sqCur], asqEvenSum, sizeof(asqEvenSum));
    memcpy(s_aasqSumsOdd[sqCur], asqOddSum, sizeof(asqOddSum));
}

static void reduceSum(SQWORD sqWeight, SQWORD sqFrom)
{
    for (SQWORD sqBit = 0; sqBit < MAX_BITS; sqBit++) {
        if (sqBit < 4) {
            printf("r: %lld %lld\n", s_aasqReduceSumsEven[sqFrom][sqBit], s_aasqReduceSumsOdd[sqFrom][sqBit]);
        }
        if (sqWeight & (0x1 << sqBit)) {
            asqEvenSum[sqBit] -= s_aasqReduceSumsOdd[sqFrom][sqBit];
            asqOddSum[sqBit] -= s_aasqReduceSumsEven[sqFrom][sqBit];
        } else {
            asqEvenSum[sqBit] -= s_aasqReduceSumsEven[sqFrom][sqBit];
            asqOddSum[sqBit] -= s_aasqReduceSumsOdd[sqFrom][sqBit];
        }
    }  
}

static void dfs(
    SQWORD sqFrom, 
    SQWORD sqCur, 
    SQWORD sqWeight,
    const vector<vector<ST_EDGE>> &vvsqEdge)
{
    printf("[%lld -> %lld]\n", sqFrom, sqCur);

    if (-1 != sqFrom) {
        addSum(sqWeight);
        reduceSum(sqWeight, sqFrom);
        setSums(sqCur);
        addReduceSums(sqFrom, sqCur);
    }

    for (SQWORD sqIdx = 0; sqIdx < 10; sqIdx++) {
        printf("%lld ", asqEvenSum[sqIdx]);
    }
    printf("\n");
    for (SQWORD sqIdx = 0; sqIdx < 10; sqIdx++) {
        printf("%lld ", asqOddSum[sqIdx]);
    }
    printf("\n");

    for (auto e: vvsqEdge[sqCur]) {
        if (e.sqTo != sqFrom) {
            dfs(sqCur, e.sqTo, e.sqW, vvsqEdge);
        }
    }
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    vector<vector<ST_EDGE>> vvEdge(sqN);

    for (SQWORD sqIdx = 0; sqIdx < sqN - 1; sqIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();
        SQWORD sqW = inputSQWORD();
        sqU--;
        sqV--;
        vvEdge[sqU].emplace_back(sqV, sqW);
        vvEdge[sqV].emplace_back(sqU, sqW);
    }
    dfs(-1, 0, 0, vvEdge);

    SQWORD sqAns = 0;
    for (SQWORD sqBit = 0; sqBit < sqN; sqBit++) {
        sqAns += (0x1 << sqBit) * asqEvenSum[sqBit];
    }

    printf("%lld\n", sqAns);

    return 0;
}
