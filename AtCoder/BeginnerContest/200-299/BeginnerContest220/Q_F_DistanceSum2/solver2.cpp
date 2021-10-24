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

static inline SQWORD ABS(SQWORD a) {return 0 < a ? a: -a;}

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

static vector<vector<SQWORD>> vvsqEdge;
static vector<map<SQWORD, SQWORD>> vmapNodeCnts;
static vector<map<SQWORD, SQWORD>> vmapDistSums;
static vector<SQWORD> vsqSiblingNodeSums;
static SQWORD sqN;

void dfsFwd(SQWORD sqFrom, SQWORD sqCur, SQWORD sqDistSum)
{
    SQWORD sqSumCur = sqDistSum;
    SQWORD sqNodeCnt = 1;
    for (auto sums: vmapDistSums[sqCur]) {
        sqDistSum += sums.second;
    }
    for (auto nodes: vmapNodeCnts[sqCur]) {
        sqNodeCnt += nodes.second;
    }

    if (-1 != sqFrom) {
        vmapDistSums[sqCur][sqFrom] = sqDistSum;
//        printf("distsum [%lld->%lld]>> %lld\n", sqCur, sqFrom,  sqDistSum);
    }

    for (SQWORD sqNodeIdx = 0; sqNodeIdx < vvsqEdge[sqCur].size(); ++sqNodeIdx) {
        const auto &d = vvsqEdge[sqCur][sqNodeIdx];
        if (d != sqFrom) {
            dfsFwd(sqCur, d, sqDistSum - vmapDistSums[sqCur][d] 
                                + (sqNodeCnt - vmapNodeCnts[sqCur][d]));
        }
    }
}

SQWORD dfs(SQWORD sqFrom, SQWORD sqCur, SQWORD &sqDistSum)
{
//    printf("--- %lld\n", sqCur);
    sqDistSum = 0;
    SQWORD sqNodeCnt = 1;
    for (SQWORD sqNodeIdx = 0; sqNodeIdx < vvsqEdge[sqCur].size(); ++sqNodeIdx) {
        const auto &d = vvsqEdge[sqCur][sqNodeIdx];
        if (d != sqFrom) {
            SQWORD sqDistSumLocal;
            SQWORD sqNodes = dfs(sqCur, d, sqDistSumLocal);
            sqNodeCnt += sqNodes;
            sqDistSum += sqDistSumLocal;
//            printf("nodecnt d [%lld/%lld]>> %lld\n", sqCur, d, sqNodes);
            vmapNodeCnts[sqCur][d] = sqNodes;
        }
    }
    sqDistSum += sqNodeCnt;
    if (-1 != sqFrom) {
//        printf("distsum [%lld->%lld]>> %lld\n", sqFrom, sqCur,  sqDistSum);
        vmapDistSums[sqFrom][sqCur] = sqDistSum;
//        printf("nodecnt [%lld/%lld]>> %lld\n", sqCur, sqFrom,  sqN - sqNodeCnt);
        vmapNodeCnts[sqCur][sqFrom] = sqN - sqNodeCnt;
    }
    return sqNodeCnt;
}

struct EDGE_ST {
    SQWORD sqFrom;
    SQWORD sqTo;

    EDGE_ST(SQWORD f, SQWORD t): sqFrom(f), sqTo(t) {};
};

int main(void)
{
    sqN = inputSQWORD();
    vvsqEdge.resize(sqN);
    vmapNodeCnts.resize(sqN);
    vsqSiblingNodeSums.resize(sqN);
    vmapDistSums.resize(sqN);
    vector<EDGE_ST> vstEdges;

    for (SQWORD sqIdx = 0; sqIdx < sqN - 1; sqIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();
        sqU--;
        sqV--;

        vvsqEdge[sqU].emplace_back(sqV);
        vvsqEdge[sqV].emplace_back(sqU);
        vstEdges.emplace_back(sqU, sqV);
    }

    SQWORD sqDistSum = 0;
    dfs(-1, 0, sqDistSum);
    dfsFwd(-1, 0, 0);

    for (auto n: vmapDistSums) {
        SQWORD sqSum = 0;
        for (auto c: n) {
            sqSum += c.second;
        }
        printf("%lld\n", sqSum);
    }

    return 0;
}
