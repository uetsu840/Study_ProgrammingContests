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

typedef struct {
    SDWORD lTo;
    SQWORD sqCap;
    SQWORD sqCost;
    SDWORD lRev;
} ST_EDGE;

struct Flow {
private:
    vector<vector<ST_EDGE>> vvecGraph;
    SQWORD  sqNumV;
    SQWORD  sqEdgeNum;
    vector<SQWORD>  vecsqDist;
    vector<SDWORD>  veclPrevV;   
    vector<SDWORD>  veclPrevEdge;      /*直前の頂点と辺 */

    vector<SDWORD>  veclDinic_Level;
    vector<SDWORD>  veclDinic_Iter;

public:
    Flow(SDWORD lMaxV) 
    {
        sqNumV = lMaxV;
        vecsqDist.resize(lMaxV);
        veclPrevV.resize(lMaxV);   
        veclPrevEdge.resize(lMaxV);      /*直前の頂点と辺 */
        veclDinic_Level.resize(lMaxV);
        veclDinic_Iter.resize(lMaxV);
        vvecGraph.resize(lMaxV);
    }

    void addEdge(SDWORD lFrom, SDWORD lTo, SQWORD sqCap, SQWORD sqCost) {
        // printf("%d ----[%lld] ----> %d\n", lFrom, sqCap, lTo);

        vvecGraph[lFrom].push_back((ST_EDGE){lTo, sqCap, sqCost, (SDWORD)(vvecGraph[lTo].size())});
        vvecGraph[lTo].push_back((ST_EDGE){lFrom, 0, -sqCost, (SDWORD)(vvecGraph[lFrom].size() - 1)});
    }

    SQWORD minCostFlow(SDWORD lStart, SDWORD lTarget, SQWORD sqFlow)
    {
        SQWORD sqRes = 0;
        SQWORD sqFlowRest = sqFlow;
        while (0 < sqFlowRest) {
            /* Bellman-Ford Method */
            fill(vecsqDist.begin(), vecsqDist.end(), MAX_SQWORD);
            vecsqDist[lStart] = 0;
            bool bIsUpdate = true;
            while (bIsUpdate) {
                bIsUpdate = false;
                for (SDWORD lV = 0; lV < sqNumV; lV++) {
                    for (SDWORD lIdx = 0; lIdx < vvecGraph[lV].size(); lIdx++) {
                        ST_EDGE &stE = vvecGraph[lV][lIdx];
                        if ((vecsqDist[lV] != MAX_SQWORD)
                                && (0 < stE.sqCap)
                                && (vecsqDist[lV] + stE.sqCost < vecsqDist[stE.lTo])) {
                            vecsqDist[stE.lTo] = vecsqDist[lV] + stE.sqCost;
                            veclPrevV[stE.lTo] = lV;
                            veclPrevEdge[stE.lTo] = lIdx;
                            bIsUpdate = true;
                        }
                    }
                }
            }
            if (vecsqDist[lTarget] == MAX_SQWORD) {
                return -1;
            }

            SQWORD sqAddFlow = MAX_SQWORD;
            for (SDWORD lV = lTarget; lV != lStart; lV = veclPrevV[lV]) {
                sqAddFlow = min(sqAddFlow, vvecGraph[veclPrevV[lV]][veclPrevEdge[lV]].sqCap);
            }
            sqFlowRest -= sqAddFlow;
            sqRes += sqAddFlow * vecsqDist[lTarget];      // 後で目的変数は書き換える
            for (SDWORD lV = lTarget; lV != lStart; lV = veclPrevV[lV]) {
                ST_EDGE &stE = vvecGraph[veclPrevV[lV]][veclPrevEdge[lV]];
                stE.sqCap -= sqAddFlow;
                vvecGraph[lV][stE.lRev].sqCap += sqAddFlow;
            }
        }
        return sqRes;
    }

private:
    SQWORD maxFlowDinicBfs(SDWORD lS)
    {
        fill(veclDinic_Level.begin(), veclDinic_Level.end(), -1);
        queue<SDWORD> queSearch;
        veclDinic_Level[lS] = 0;
        queSearch.push(lS);
        while (!queSearch.empty()) {
            SDWORD lV = queSearch.front();
            queSearch.pop();
            for (SDWORD lIdx = 0; lIdx < vvecGraph[lV].size(); lIdx++) {
                ST_EDGE &stE = vvecGraph[lV][lIdx];
                if ((0 < stE.sqCap) && (veclDinic_Level[stE.lTo] < 0)) {
                    veclDinic_Level[stE.lTo] = veclDinic_Level[lV] + 1;
                    queSearch.push(stE.lTo);
                }
            }
        }

        return 0;
    }

    SQWORD maxFlowDinicDfs(SDWORD lV, SDWORD lT, SQWORD sqF)
    {
        if (lV == lT) {
            return sqF;
        }
        for (SDWORD &lI = veclDinic_Iter[lV]; lI < vvecGraph[lV].size(); lI++) {
            ST_EDGE &stE = vvecGraph[lV][lI];
            if ((0 < stE.sqCap) && (veclDinic_Level[lV] < veclDinic_Level[stE.lTo])) {
                SQWORD sqDiffFlow = maxFlowDinicDfs(stE.lTo, lT, min(sqF, stE.sqCap));
                if (0 < sqDiffFlow) {
                    stE.sqCap -= sqDiffFlow;
                    vvecGraph[stE.lTo][stE.lRev].sqCap += sqDiffFlow;
                    return sqDiffFlow;
                }
            }
        }
        return 0;
    }

public:
    SQWORD MaxFlow(SDWORD lStart, SDWORD lTarget)
    {
        SQWORD sqFlow = 0LL;
        for (;;) {
            maxFlowDinicBfs(lStart);
            if (veclDinic_Level[lTarget] < 0) {
                return sqFlow;
            }
            fill(veclDinic_Iter.begin(), veclDinic_Iter.end(),  0);
            while (1) {
                SQWORD sqDiffFlow = maxFlowDinicDfs(lStart, lTarget, MAX_SQWORD);
                sqFlow += sqDiffFlow;
                if (0LL == sqDiffFlow) {
                    break;
                }
            }
        }
    }
};


#define MAX_V   (202)

#define DUMMY_COST          (0)
#define MAX_WIDTH           (100)

#define PNT_NO_OFFSET_COL   (0)       /* 0-99 */
#define PNT_NO_OFFSET_ROW   (100)     /* 100-199 */

#define PNT_NO_START    (200)
#define PNT_NO_TARGET   (201)

int main()
{
    DWORD dwInput_H, dwInput_W;

    dwInput_H = inputSDWORD();
    dwInput_W = inputSDWORD();

    char acInputLine[MAX_WIDTH + 1];

    Flow solverFlow(MAX_V);

    for (DWORD dwRowIdx = 0; dwRowIdx < dwInput_H; dwRowIdx++) {
        inputString(acInputLine);

        for (DWORD dwColIdx = 0; dwColIdx < dwInput_W; dwColIdx++) {
            if (acInputLine[dwColIdx] == 'S') {
                solverFlow.addEdge(PNT_NO_START,
                                    dwColIdx + PNT_NO_OFFSET_COL,
                                    MAX_SDWORD, DUMMY_COST);
                solverFlow.addEdge(PNT_NO_START,
                                    dwRowIdx + PNT_NO_OFFSET_ROW,
                                    MAX_SDWORD, DUMMY_COST);
            } else if (acInputLine[dwColIdx] == 'T') {
                solverFlow.addEdge(dwColIdx + PNT_NO_OFFSET_COL,
                                    PNT_NO_TARGET,
                                    MAX_SDWORD, DUMMY_COST);
                solverFlow.addEdge(dwRowIdx + PNT_NO_OFFSET_ROW,
                                    PNT_NO_TARGET,
                                    MAX_SDWORD, DUMMY_COST);
            } else if (acInputLine[dwColIdx] == 'o') {
                solverFlow.addEdge(dwColIdx + PNT_NO_OFFSET_COL, 
                                    dwRowIdx + PNT_NO_OFFSET_ROW,
                                    1, DUMMY_COST);
                solverFlow.addEdge(dwRowIdx + PNT_NO_OFFSET_ROW, 
                                    dwColIdx + PNT_NO_OFFSET_COL,
                                    1, DUMMY_COST);
            }
        }
    }
    QWORD qwMaxFlow = solverFlow.MaxFlow(PNT_NO_START, PNT_NO_TARGET);
    if (MAX_SDWORD <= qwMaxFlow) {
        printf("-1\n");
    } else {
        printf("%lld\n", qwMaxFlow);
    }

    return 0;
}
