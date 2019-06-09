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


#define MAX_V   (2010)

#define PNT_NO_MID_PP       (2001)
#define PNT_NO_MID_PN       (2002)
#define PNT_NO_MID_NP       (2003)
#define PNT_NO_MID_NN       (2004)

#define PNT_NO_START        (2008)
#define PNT_NO_END          (2009)

#define PNT_NO_OFFSET_B     (1000)

#define COST_OFFSET         (2000000000LL)
#define NO_COST             (0)

struct ST_BALL {
    SQWORD sqX;
    SQWORD sqY;
    SQWORD sqC;

    ST_BALL(SQWORD x, SQWORD y, SQWORD c) {
        sqX = x;
        sqY = y;
        sqC = c;
    }
};

int main()
{
    SQWORD  sqInput_N = inputSQWORD();
    vector<ST_BALL> vecstBallR;
    vector<ST_BALL> vecstBallB;
    SQWORD sqNumBall = 0;

    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        SQWORD sqC = inputSQWORD();
        vecstBallR.emplace_back(sqX, sqY, sqC);
        sqNumBall += sqC;
    }
    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        SQWORD sqC = inputSQWORD();
        vecstBallB.emplace_back(sqX, sqY, sqC);
    }


    Flow solverFlow(MAX_V);
    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        solverFlow.addEdge(PNT_NO_START,
                            lIdx, 
                            vecstBallR[lIdx].sqC, NO_COST);
    }
    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        solverFlow.addEdge(lIdx + PNT_NO_OFFSET_B,
                            PNT_NO_END,
                            vecstBallB[lIdx].sqC, NO_COST);
    }

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        ST_BALL &stB = vecstBallR[sqIdx];
        solverFlow.addEdge(sqIdx,
                            PNT_NO_MID_PP, 
                            stB.sqC,
                            stB.sqX + stB.sqY + COST_OFFSET);
        solverFlow.addEdge(sqIdx,
                            PNT_NO_MID_PN, 
                            stB.sqC,
                            stB.sqX - stB.sqY + COST_OFFSET);
        solverFlow.addEdge(sqIdx,
                            PNT_NO_MID_NP, 
                            stB.sqC,
                            - stB.sqX + stB.sqY + COST_OFFSET);
        solverFlow.addEdge(sqIdx,
                            PNT_NO_MID_NN, 
                            stB.sqC,
                            - stB.sqX - stB.sqY + COST_OFFSET);
    }

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        ST_BALL &stB = vecstBallB[sqIdx];
        solverFlow.addEdge(PNT_NO_MID_PP, 
                            sqIdx + PNT_NO_OFFSET_B,
                            stB.sqC,
                            - stB.sqX - stB.sqY + COST_OFFSET);
        solverFlow.addEdge(PNT_NO_MID_PN, 
                            sqIdx + PNT_NO_OFFSET_B,
                            stB.sqC,
                            - stB.sqX + stB.sqY + COST_OFFSET);
        solverFlow.addEdge(PNT_NO_MID_NP, 
                            sqIdx + PNT_NO_OFFSET_B,
                            stB.sqC,
                            stB.sqX - stB.sqY + COST_OFFSET);
        solverFlow.addEdge(PNT_NO_MID_NN, 
                            sqIdx + PNT_NO_OFFSET_B,
                            stB.sqC,
                            stB.sqX + stB.sqY + COST_OFFSET);
    }

    SQWORD sqCost = solverFlow.minCostFlow(PNT_NO_START, PNT_NO_END, sqNumBall);
    sqCost -= (COST_OFFSET * 2 * sqNumBall);
    sqCost *= -1LL;
    printf("%lld\n", sqCost);

    return 0;
}
