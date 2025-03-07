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
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

#define COST_INF        (100100100100100100)
#define N_MAX_VERTICE   (10000)

class Prim {
private:
    SQWORD  sqNumVertice;
    const   vector<EDGE_ST> *pvecEdge;
    SQWORD  aMinCost[N_MAX_VERTICE + 1];        /* 部分集合から頂点までの最小コスト */
    bool    abIsUsed[N_MAX_VERTICE + 1];
    SQWORD  aaCost[N_MAX_VERTICE + 1][N_MAX_VERTICE + 1];
    SQWORD  sqIdxMinV;
    SQWORD  sqIdxMaxV;

public:
    Prim(
        SQWORD sqV, 
        const vector<EDGE_ST> *vE,
        bool bStartWithZero = false) : sqNumVertice(sqV), pvecEdge(vE) 
    {
        if (bStartWithZero) {
            sqIdxMinV = 0;
            sqIdxMaxV = sqNumVertice - 1;
        } else {
            sqIdxMinV = 1;
            sqIdxMaxV = sqNumVertice;
        }

        /* 辺が存在しない箇所のコストはINF */
        for (SWORD sqI = sqIdxMinV; sqI <= sqV; sqI++) {
            for (SQWORD sqJ = 1; sqJ <= sqIdxMaxV; sqJ++) {
                aaCost[sqI][sqJ] = COST_INF;
            }
        }
        /* 辺が存在する箇所のコストは辺のコストそのもの */
        for (SQWORD sqI = sqIdxMinV; sqI <= sqIdxMaxV; sqI++) {
            for (auto v:pvecEdge[sqI]) {
                aaCost[sqI][v.sqTo] = v.sqCost;
            }
        }
        /* 自身へのコストは0 */
        for (SQWORD sqI = sqIdxMinV; sqI <= sqIdxMaxV; sqI++) {
            aaCost[sqI][sqI] = 0;
        }

        /* usedフラグ初期化 */
        for (SQWORD sqI = sqIdxMinV; sqI <= sqIdxMaxV; sqI++) {
            abIsUsed[sqI] = false;
        }

        /* 最小コスト初期化 */
        for (SQWORD sqV = sqIdxMinV; sqV <= sqIdxMaxV; sqV++) {
            aMinCost[sqV] = COST_INF;
        }
    };

    SQWORD Solve(void)
    {
        aMinCost[sqIdxMinV] = 0;
        SQWORD sqMinCostTtl = 0;

        while(1) {
            SQWORD sqMinCostV = -1;

            /* 最小全域木に属さない頂点のうち、最小コストの辺で接続されているものを探す */
            for (SQWORD sqNextV = sqIdxMinV; sqNextV <= sqIdxMaxV; sqNextV++) {
                if (!abIsUsed[sqNextV]) {
                    if (-1 == sqMinCostV) {
                        sqMinCostV = sqNextV;
                    } else if (aMinCost[sqNextV] < aMinCost[sqMinCostV]) {
                        sqMinCostV = sqNextV;
                    }
                }
            }

            if (-1 == sqMinCostV) {
                break;
            }

            abIsUsed[sqMinCostV] = true;
            sqMinCostTtl += aMinCost[sqMinCostV];

            /* 加えた頂点から集合の範囲外の各点への最小コストを更新する */
            for (SQWORD sqV = sqIdxMinV; sqV <= sqIdxMaxV; sqV++) {
                if (!abIsUsed[sqV]) {
                    aMinCost[sqV] = min(aMinCost[sqV], aaCost[sqMinCostV][sqV]);
                }
            }
        }
        return sqMinCostTtl;
    }
};


int main(void)
{
    SQWORD sqInput_V = inputSQWORD();
    SQWORD sqInput_E = inputSQWORD();

    static vector<EDGE_ST>  s_avecEdges[N_MAX_VERTICE + 1];

    for (SQWORD sqEdgeIdx = 0; sqEdgeIdx < sqInput_E; sqEdgeIdx++) {
        SQWORD sqInput_s = inputSQWORD();
        SQWORD sqInput_t = inputSQWORD();
        SQWORD sqInput_w = inputSQWORD();

        s_avecEdges[sqInput_s].emplace_back(sqInput_t, sqInput_w);
        s_avecEdges[sqInput_t].emplace_back(sqInput_s, sqInput_w);
    }

    /* prim method */
    static Prim prim(sqInput_V, s_avecEdges, true);
    SQWORD sqMinCost = prim.Solve();

    printf("%lld\n", sqMinCost);
    return 0;
}