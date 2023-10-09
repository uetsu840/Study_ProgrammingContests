#pragma GCC optimize ("O3")
using namespace std;

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
#include <cstdint>
#include <stack>

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
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}
static inline SQWORD DIV_UP(SQWORD a, SQWORD x) {return (a + (x - 1)) / x;}

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
struct EDGE_ST {
    SQWORD sqDst;
    SQWORD sqCost;

    EDGE_ST (SQWORD d, SQWORD c) : sqDst(d), sqCost(c) {};
};

static bool dfs (
    vector<vector<EDGE_ST>> &vvstEdge,
    vector<SQWORD> &vsqPotential,    
    vector<SQWORD> &vsqGroupIdx,
    SQWORD sqCur,
    SQWORD sqPotential,
    SQWORD sqGroupIdx)
{
//    printf("---------------> %lld\n", sqCur);
    vsqPotential[sqCur] = sqPotential;
    vsqGroupIdx[sqCur] = sqGroupIdx;
    bool bRet = true;
    for (auto e: vvstEdge[sqCur]) {
        if (0 == vsqGroupIdx[e.sqDst]) {
            if (!dfs(vvstEdge, vsqPotential, vsqGroupIdx, e.sqDst, sqPotential + e.sqCost, sqGroupIdx)) {
                bRet = false;
            }
        } else {
            if (sqPotential + e.sqCost != vsqPotential[e.sqDst]) {
                bRet = false;
            }
        }
    }
    return bRet;
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    vector<vector<EDGE_ST>> vvstEdge(sqN);

    /* Edges */
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqA = inputSQWORD() - 1; /* 0-indexed */
        SQWORD sqB = inputSQWORD() - 1; /* 0-indexed */
        SQWORD sqC = inputSQWORD();

        vvstEdge[sqA].emplace_back(sqB, sqC);
        vvstEdge[sqB].emplace_back(sqA, -sqC);
    }

    /* tree */
    vector<SQWORD> vsqPotential(sqN);    
    vector<SQWORD> vsqGroupIdx(sqN);
    vector<bool> vbIsInf(sqN + 1);

    SQWORD sqCurGroupIdx = 1;
    for (SQWORD sqStartIdx = 0; sqStartIdx < sqN; sqStartIdx++) {
        if (0 == vsqGroupIdx[sqStartIdx]) {
//            printf("DFS group [%lld]\n", sqCurGroupIdx);
            if (!dfs(vvstEdge, vsqPotential, vsqGroupIdx, sqStartIdx, 0, sqCurGroupIdx)) {
                vbIsInf[sqCurGroupIdx] = true;
            }
            sqCurGroupIdx++;
        }
    }

    /* query */
    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqX = inputSQWORD() - 1;
        SQWORD sqY = inputSQWORD() - 1;

///        printf(">> %lld %lld\n", vsqGroupIdx[sqX], vsqGroupIdx[sqY]);

        if (vsqGroupIdx[sqX] == vsqGroupIdx[sqY]) {
            if (vbIsInf[vsqGroupIdx[sqX]]) {
                printf("inf\n");
            } else {
                printf("%lld\n", vsqPotential[sqY] - vsqPotential[sqX]);
            }

        } else {
            printf("nan\n");
        }
    }

    return 0;
}
