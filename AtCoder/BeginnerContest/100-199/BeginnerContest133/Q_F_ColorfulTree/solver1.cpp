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
    SQWORD sqColor;

    EDGE_ST(SQWORD to, SQWORD color, SQWORD cost) {
        sqTo = to;
        sqColor = color;
        sqCost = cost;
    };
};

#define N_MAX_NODES     (100000)
#define MAX_LOG_NODES   (20)

struct TREE {
    vector<EDGE_ST> avecstEdge[N_MAX_NODES + 1];
    SQWORD sqNumNode;
    SQWORD aasqParent[MAX_LOG_NODES][N_MAX_NODES + 1];
    SQWORD asqDepth[N_MAX_NODES + 1];
    SQWORD asqCost[N_MAX_NODES + 1];        /* rootからのコスト */

    TREE(SQWORD sqN) {
        sqNumNode = sqN;
    }

    void retistEdge(SQWORD sqA, SQWORD sqB, SQWORD sqColor, SQWORD sqCost)
    {
        avecstEdge[sqA].emplace_back(sqB, sqColor, sqCost);
        avecstEdge[sqB].emplace_back(sqA, sqColor, sqCost);
    }

    void dfs(SQWORD sqNode, SQWORD sqP = -1, SQWORD sqDepth = 0, SQWORD sqCost = 0)
    {
        aasqParent[0][sqNode] = sqP;
        asqDepth[sqNode] = sqDepth;
        asqCost[sqNode] = sqCost;
        for (auto edge:avecstEdge[sqNode]) {
            if (edge.sqTo != sqP) {
                dfs(edge.sqTo, sqNode, sqDepth + 1, sqCost + edge.sqCost);
            }
        }
    }

    /**
     *  initialize
     */    
    void init(SQWORD sqRoot) 
    {
        /* measure Depth */
        dfs(sqRoot);

        /* initialize parent */
        for (SQWORD sqLogIdx = 0; sqLogIdx + 1 < MAX_LOG_NODES; sqLogIdx++) {
            for (SQWORD sqNode = 1; sqNode <= sqNumNode; sqNode++) {
                if (aasqParent[sqLogIdx][sqNode] < 0) {
                    aasqParent[sqLogIdx + 1][sqNode] = -1;
                } else {
                    aasqParent[sqLogIdx + 1][sqNode] = aasqParent[sqLogIdx][aasqParent[sqLogIdx][sqNode]];
                }
            }
        }
    }

    /**
     * calculate lca of U and V
     */
    SQWORD lca(SQWORD sqU, SQWORD sqV)
    {
        /* u, v が同じ深さになるまで親を辿る */
        if (asqDepth[sqU] > asqDepth[sqV]) {
            swap(sqU, sqV);
        }
        for (SQWORD sqLogIdx = 0; sqLogIdx < MAX_LOG_NODES; sqLogIdx++) {
            if (((asqDepth[sqV] - asqDepth[sqU]) >> sqLogIdx) & 0x1) {
                sqV = aasqParent[sqLogIdx][sqV];
            }
        }
        if (sqU == sqV) {
            return sqU;
        }

        /* 二分探索でLCAを求める */
        for (SQWORD sqLogIdx = MAX_LOG_NODES - 1; 0 <= sqLogIdx; sqLogIdx--) {
            if (aasqParent[sqLogIdx][sqU] != aasqParent[sqLogIdx][sqV]) {
                sqU = aasqParent[sqLogIdx][sqU];
                sqV = aasqParent[sqLogIdx][sqV];
            }
        }

        return aasqParent[0][sqU];
    }
};

struct Query {
    SQWORD sqIdx;
    SQWORD sqColor;
    SQWORD sqCost;
    SQWORD sqCoeff;

    Query(SQWORD idx, SQWORD col, SQWORD cost, SQWORD coeff)
        :sqIdx(idx), sqColor(col), sqCost(cost), sqCoeff(coeff) {};
};

#define N_MAX_QUERY (100000)
#define N_MAX_COLOR (100000)
static SQWORD s_asqAns[N_MAX_QUERY];
static SQWORD s_asqCostColor[N_MAX_COLOR + 1];
static SQWORD s_asqNumColor[N_MAX_COLOR + 1];

static void dfsQuery(
    SQWORD sqNode, 
    SQWORD sqParent, 
    const vector<Query> pvecQuery[], 
    TREE &stTree,
    SQWORD asqCostColor[],
    SQWORD asqNumColor[])
{
    /* exec query */
    for (auto query: pvecQuery[sqNode]) {
//        printf("%lld %lld %lld\n", query.sqIdx, query.sqColor, asqCostColor[query.sqColor]);
        s_asqAns[query.sqIdx] -= query.sqCoeff * asqCostColor[query.sqColor];
        s_asqAns[query.sqIdx] += query.sqCoeff * query.sqCost * asqNumColor[query.sqColor];
    }

    /* dfs */
    for (auto edge: stTree.avecstEdge[sqNode]) {
        if (edge.sqTo != sqParent) {
            asqCostColor[edge.sqColor] += edge.sqCost;
            asqNumColor[edge.sqColor]++;
            dfsQuery(edge.sqTo, sqNode, pvecQuery, stTree, asqCostColor, asqNumColor);
            asqCostColor[edge.sqColor] -= edge.sqCost;
            asqNumColor[edge.sqColor]--;
        }
    }
}


int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_Q = inputSQWORD();
    static TREE    stTree(sqInput_N);

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N - 1; sqIdx++) {
        SQWORD sqInput_a = inputSQWORD();
        SQWORD sqInput_b = inputSQWORD();
        SQWORD sqInput_c = inputSQWORD();
        SQWORD sqInput_d = inputSQWORD();
        stTree.retistEdge(sqInput_a, sqInput_b, sqInput_c, sqInput_d);
    }
    stTree.init(1);

    static vector<Query> avecQuery[N_MAX_NODES + 1];
    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqInput_Q; sqQueryIdx++) {
        SQWORD sqInput_x = inputSQWORD();
        SQWORD sqInput_y = inputSQWORD();
        SQWORD sqInput_u = inputSQWORD();
        SQWORD sqInput_v = inputSQWORD();

        SQWORD sqLca = stTree.lca(sqInput_u, sqInput_v);
        avecQuery[sqInput_u].emplace_back(sqQueryIdx, sqInput_x, sqInput_y, 1);
        avecQuery[sqInput_v].emplace_back(sqQueryIdx, sqInput_x, sqInput_y, 1);
        avecQuery[sqLca].emplace_back(sqQueryIdx, sqInput_x, sqInput_y, -2);       

        s_asqAns[sqQueryIdx] = stTree.asqCost[sqInput_u]
                                 + stTree.asqCost[sqInput_v]
                                 - 2 * stTree.asqCost[sqLca];
//        printf("orgcost[%lld %lld] %lld\n", sqInput_u, sqInput_v, s_asqAns[sqQueryIdx]);
                                 
    }

    /* exec query with dfs */
    dfsQuery(1, -1, avecQuery, stTree, s_asqCostColor, s_asqNumColor);

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqInput_Q; sqQueryIdx++) {
        printf("%lld\n", s_asqAns[sqQueryIdx]);
    }

    return 0;
}