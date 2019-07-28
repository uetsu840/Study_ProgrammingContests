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

typedef struct {
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    DWORD   dwSize;

    void initUnionFind(
        DWORD dwSize)
    {
        dwSize = dwSize;
        vdwPar.resize(dwSize);
        vdwRank.resize(dwSize);
        vdwCnt.resize(dwSize);
    
        for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vdwPar[dwIdx]  = dwIdx;
            vdwRank[dwIdx] = 0;
            vdwCnt[dwIdx]  = 1;
        }
    }


    DWORD ufGetParent(DWORD dwIdx) const
    {
        return vdwPar[dwIdx];
    }

    DWORD ufGetRank(DWORD dwIdx) const
    {
        return vdwRank[dwIdx];
    }

    void ufSetParent(DWORD dwIdx, DWORD dwParent)
    {
        vdwPar[dwIdx] = dwParent; 
        if (ufGetRank(dwIdx) == ufGetRank(dwParent)) {
            (vdwRank[dwParent])++;
        }
    }

    DWORD ufGetRoot(DWORD dwIdx) const
    {
        if (ufGetParent(dwIdx) == dwIdx) {
            return dwIdx;
        } else {
            DWORD dwParent = ufGetParent(dwIdx);
            DWORD dwRoot = ufGetRoot(dwParent);
            return dwRoot;
        }
    }

    void ufUnite(DWORD dwX, DWORD dwY)
    {
        DWORD dwRootX = ufGetRoot(dwX);
        DWORD dwRootY = ufGetRoot(dwY);

        if (dwRootX == dwRootY) {
            return;
        }

        if (ufGetRank(dwRootX) < ufGetRank(dwRootY)) {
            ufSetParent(dwRootX, dwRootY);
            (vdwCnt[dwRootY]) += (vdwCnt[dwRootX]);
        } else {
            ufSetParent(dwRootY, dwRootX);
            (vdwCnt[dwRootX]) += (vdwCnt[dwRootY]);
        }
    }

    bool ufIsSame(DWORD dwX, DWORD dwY) const
    {
        return (ufGetRoot(dwX)  == ufGetRoot(dwY));
    }
} ST_UNION_FIND;

/* ------------------------------------------------------------- */


struct EDGE_ST {
    SQWORD sqV1;
    SQWORD sqV2;
    SQWORD sqCost;

    EDGE_ST(SQWORD v1, SQWORD v2, SQWORD cost) {
        sqV1 = v1;
        sqV2 = v2;
        sqCost = cost;
    };
};

bool comp(const EDGE_ST &e1, const EDGE_ST &e2)
{
    return e1.sqCost < e2.sqCost;
}

#define COST_INF        (100100100100100100)
#define N_MAX_VERTICE   (10000)

class Kruskal {
private:
    SQWORD  sqNumVertice;
    SQWORD  sqNumEdge;
    vector<EDGE_ST> vecEdge;
    SQWORD  sqIdxMinV;
    SQWORD  sqIdxMaxV;


public:
    Kruskal(SQWORD sqNumV, SQWORD sqNumE, bool bStartWithZero = false)
    {
        sqNumVertice = sqNumV;
        sqNumEdge = sqNumE;
        if (bStartWithZero) {
            sqIdxMinV = 0;
            sqIdxMaxV = sqNumVertice - 1;
        } else {
            sqIdxMinV = 1;
            sqIdxMaxV = sqNumVertice;
        }
    }

    void AddEdge(SQWORD sqV1, SQWORD sqV2, SQWORD sqCost) {
        vecEdge.emplace_back(sqV1, sqV2, sqCost);
    }

    SQWORD solve(void)
    {
        ST_UNION_FIND Uf;
        Uf.initUnionFind(sqIdxMaxV + 1);

        sort(vecEdge.begin(), vecEdge.end(), comp);

        SQWORD sqCost = 0;

        for (SQWORD sqEdgeIdx = 0; sqEdgeIdx < sqNumEdge; sqEdgeIdx++) {
            EDGE_ST e = vecEdge[sqEdgeIdx];
            if (!Uf.ufIsSame(e.sqV1, e.sqV2)) {
                Uf.ufUnite(e.sqV1, e.sqV2);
//                printf("%lld %lld\n", e.sqV1, e.sqV2);
                sqCost += e.sqCost;
            }
        }
        return sqCost;
    }
};


int main(void)
{
    SQWORD sqInput_V = inputSQWORD();
    SQWORD sqInput_E = inputSQWORD();

    Kruskal kruskal(sqInput_V, sqInput_E, true);

    for (SQWORD sqEdgeIdx = 0; sqEdgeIdx < sqInput_E; sqEdgeIdx++) {
        SQWORD sqInput_s = inputSQWORD();
        SQWORD sqInput_t = inputSQWORD();
        SQWORD sqInput_w = inputSQWORD();

        kruskal.AddEdge(sqInput_s, sqInput_t, sqInput_w);
    }

    /* prim method */
    SQWORD sqMinCost = kruskal.solve();

    printf("%lld\n", sqMinCost);
    return 0;
}