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
#define     COST_INF      (100100100100100100)

struct EDGE_ST {
    SQWORD sqFrom;
    SQWORD sqTo;
    SQWORD sqCost;
    SQWORD sqIdx;

    EDGE_ST(SQWORD from, SQWORD to, SQWORD cost, SQWORD i) {
        sqFrom = from;
        sqTo = to;
        sqCost = cost;
        sqIdx = i;
    };
};

void solveDijkstra(
    SQWORD sqFrom, 
    const vector<vector<EDGE_ST>> &vvstEdges,
    SQWORD sqOmitIdx,
    vector<SQWORD> &cost,
    SQWORD sqDst,
    vector<EDGE_ST> &shortestPath)
{
    typedef pair<SQWORD, SQWORD> P;
    priority_queue<P, vector<P>, greater<P>> que;
    SQWORD sqNodeNum = vvstEdges.size();
    vector<EDGE_ST> vsqShortestEdge(sqNodeNum, EDGE_ST{0, 0, 0, 0});
    
    for (SQWORD sqIdx = 0; sqIdx < sqNodeNum; sqIdx++) {
        cost[sqIdx] = COST_INF;
    }
    cost[sqFrom] = 0;
    que.push(P(0, sqFrom));

    while (!que.empty()) {
        P p = que.top();
        que.pop();

        SDWORD v = p.second;
        if (p.first <= cost[v]) {
            for (SDWORD lIdx = 0; lIdx < vvstEdges[v].size(); lIdx++) {
                EDGE_ST e = vvstEdges[v][lIdx];

                if ((e.sqIdx != sqOmitIdx) && (cost[e.sqTo] > cost[v] + e.sqCost)) {
                    cost[e.sqTo] = cost[v] + e.sqCost;
                    que.push(P(cost[e.sqTo], e.sqTo));
                    vsqShortestEdge[e.sqTo] = e;
                }
            }
        }
    }

    /* 最短経路の復元 */
    shortestPath.clear();
    if ((-1 != sqDst) && (cost[sqDst] != COST_INF)) {
        SQWORD sqCur = sqDst;
        while (1) {
            if (sqCur == sqFrom) {
                break;
            }            
            shortestPath.emplace_back(vsqShortestEdge[sqCur]);
            sqCur = vsqShortestEdge[sqCur].sqFrom;
        }
        reverse(shortestPath.begin(), shortestPath.end());
    }
}


/*----------------------------------------------*/

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<vector<EDGE_ST>> vvEdge(sqN);
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqS = inputSQWORD();
        SQWORD sqT = inputSQWORD();
        sqS--;
        sqT--;
        vvEdge[sqS].emplace_back(sqS, sqT, 1, sqIdx);
    }

    vector<EDGE_ST> vsqShortestPath;
    vector<SQWORD> vsqCost(sqN);
    solveDijkstra(0, vvEdge, -1, vsqCost, sqN - 1, vsqShortestPath);

    SQWORD sqMinCost;
    if (vsqCost[sqN - 1] == COST_INF) {
        sqMinCost = -1;
    } else {
        sqMinCost = vsqCost[sqN - 1];
    }

    vector<SQWORD> vsqAns(sqM, sqMinCost);
    for (auto p: vsqShortestPath) {
        printf("i: %lld\n", p.sqIdx);
        solveDijkstra(0, vvEdge, p.sqIdx, vsqCost, sqN-1, vsqShortestPath);
        if (COST_INF == vsqCost[sqN - 1]) {
            vsqAns[p.sqIdx] = -1; 
        } else {
            vsqAns[p.sqIdx] = vsqCost[sqN - 1];        
        }
    }

    for (auto a: vsqAns) {
        printf("%lld\n", a);
    }
   
    return 0;
}
