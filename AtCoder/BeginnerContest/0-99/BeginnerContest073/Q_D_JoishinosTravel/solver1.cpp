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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }

static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; }

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

/*----------------------------------------------------*/

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

#define DIST_INF        (100100100100100100)
#define N_MAX_VERTICE   (200)

class WarshallFloyd {
    SQWORD  sqNumVertice;
    const vector<vector<EDGE_ST>> vvEdge;
    SQWORD  aaDist[N_MAX_VERTICE + 1][N_MAX_VERTICE + 1];

public:
    WarshallFloyd(SQWORD sqV, const vector<vector<EDGE_ST>> vE) : sqNumVertice(sqV), vvEdge(vE) {
        for (SWORD sqI = 1; sqI <= sqV; sqI++) {
            for (SQWORD sqJ = 1; sqJ <= sqNumVertice; sqJ++) {
                aaDist[sqI][sqJ] = DIST_INF;
            }
        }
        for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
            for (auto v:vvEdge[sqI]) {
                aaDist[sqI][v.sqTo] = v.sqCost;
            }
        }
        for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
            aaDist[sqI][sqI] = 0;
        }
    };

    void Solve(void)
    {
        for (SQWORD sqK = 1; sqK <= sqNumVertice; sqK++) {
            for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
                for (SQWORD sqJ = 1; sqJ <= sqNumVertice; sqJ++) {
                    aaDist[sqI][sqJ] = min(aaDist[sqI][sqJ],
                                            aaDist[sqI][sqK] + aaDist[sqK][sqJ]);
                } 
            }
        }
    }

    SQWORD GetDist(SQWORD sqA, SQWORD sqB)
    {
        return aaDist[sqA][sqB];
    }
};


/*----------------------------------------------------*/

#define COST_INF        (100100100100100100)
#define N_MAX_VERTICE_PM   (10000)

class Prim {
private:
    SQWORD  sqNumVertice;
    const   vector<vector<EDGE_ST>> vvEdge;
    SQWORD  aMinCost[N_MAX_VERTICE_PM + 1];        /* 部分集合から頂点までの最小コスト */
    bool    abIsUsed[N_MAX_VERTICE_PM + 1];
    SQWORD  aaCost[N_MAX_VERTICE_PM + 1][N_MAX_VERTICE_PM + 1];
    SQWORD  sqIdxMinV;
    SQWORD  sqIdxMaxV;

public:
    Prim(
        SQWORD sqV, 
        const vector<vector<EDGE_ST>> vvE,
        bool bStartWithZero = false) : sqNumVertice(sqV), vvEdge(vvE) 
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
            for (auto v:vvEdge[sqI]) {
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

int main()
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqR = inputSQWORD();

    printf("a\n");

    vector<vector<EDGE_ST>> vvstEdge(sqN + 1);

    vector<SQWORD> vsqR;
    for (SQWORD sqIdx = 0; sqIdx < sqR; sqIdx++) {
        vsqR.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();
        SQWORD sqC = inputSQWORD();

        vvstEdge[sqA].emplace_back(sqB, sqC);
        vvstEdge[sqB].emplace_back(sqA, sqB);
    }
    printf("a\n");

    WarshallFloyd wf(sqN, vvstEdge);
    wf.Solve();
    printf("b\n");

    vector<vector<EDGE_ST>> vvstEdgeR;
    for (SQWORD sqIdxFrom = 0; sqIdxFrom < vsqR.size(); sqIdxFrom++) {
        for (SQWORD sqIdxTo = 0; sqIdxTo < vsqR.size(); sqIdxTo++) {
            if (sqIdxFrom != sqIdxTo) {
                SQWORD sqCost = wf.GetDist(vsqR[sqIdxFrom], vsqR[sqIdxTo]);
                printf("%lld %lld: %lld\n", vsqR[sqIdxFrom], vsqR[sqIdxTo], sqCost);
                vvstEdgeR[sqIdxFrom].emplace_back(sqIdxTo, sqCost);
            }
        }
    }
#if 0
    Prim pm(sqR, vvstEdgeR);
    SQWORD sqAns;
    sqAns = pm.Solve();

    printf("%lld\n", sqAns);
 #endif
    return 0;
}