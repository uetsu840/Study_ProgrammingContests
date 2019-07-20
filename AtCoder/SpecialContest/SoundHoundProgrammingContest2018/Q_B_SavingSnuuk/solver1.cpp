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
#define     SQWORD_INF      (100100100100100100)

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

SQWORD solveDijkstra(
    SQWORD sqFrom, 
    SQWORD sqTo, 
    SQWORD sqNodeNum,
    const vector<EDGE_ST> *pvstEdges)
{
     typedef pair<SQWORD, SQWORD> P;
    priority_queue<P, vector<P>, greater<P>> que;
    vector<SQWORD> cost(sqNodeNum, SQWORD_INF);
    
    cost[sqFrom] = 0;
    que.push(P(0, sqFrom));

    while (!que.empty()) {
        P p = que.top();
        que.pop();

        SDWORD v = p.second;
        if (p.first <= cost[v]) {
            for (SDWORD lIdx = 0; lIdx < pvstEdges[v].size(); lIdx++) {
                EDGE_ST e = pvstEdges[v][lIdx];

                if (cost[e.sqTo] > cost[v] + e.sqCost) {
                    cost[e.sqTo] = cost[v] + e.sqCost;
                    que.push(P(cost[e.sqTo], e.sqTo));
                }
            }
        }
    }

    return cost[sqTo];
}


#define N_MAX_CITIES    (100000)
#define CITY_SNUUK_OFS  (N_MAX_CITIES)
#define N_MAX_NODE      (N_MAX_CITIES * 2 + 1)

int main(void)
{
    SDWORD lInput_n = inputSDWORD();
    SDWORD lInput_m = inputSDWORD();
    SDWORD lInput_s = inputSDWORD();
    SDWORD lInput_t = inputSDWORD();

    static vector<EDGE_ST> avecEdge[N_MAX_NODE];

    for (SDWORD lIdx = 0; lIdx < lInput_m; lIdx++) {
        SDWORD lInput_u = inputSDWORD();
        SDWORD lInput_v = inputSDWORD();
        SDWORD lInput_a = inputSDWORD();
        SDWORD lInput_b = inputSDWORD();

        avecEdge[lInput_u].emplace_back(lInput_v, lInput_a);
        avecEdge[lInput_v].emplace_back(lInput_u, lInput_a);

        avecEdge[lInput_u + CITY_SNUUK_OFS].emplace_back(lInput_v + CITY_SNUUK_OFS, lInput_b);
        avecEdge[lInput_v + CITY_SNUUK_OFS].emplace_back(lInput_u + CITY_SNUUK_OFS, lInput_b);
    }

    vector<SQWORD> vecsqAns;
    for (SDWORD lCityNo = lInput_n; 1 <= lCityNo; lCityNo--) {
        avecEdge[lCityNo].emplace_back(lCityNo + CITY_SNUUK_OFS, 0);
        SQWORD sqCost = solveDijkstra(lInput_s, lInput_t + CITY_SNUUK_OFS, N_MAX_NODE, avecEdge);
        vecsqAns.emplace_back(1000000000000000 - sqCost);
    }
    reverse(vecsqAns.begin(), vecsqAns.end());
    for (auto ans: vecsqAns) {
        printf("%lld\n", ans);
    }

    return 0;
}