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
#define MAX_SQWORD (0xFFFFFFFFFFFFFFFF)

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
    SDWORD lCap;
    SDWORD lCost;
    SDWORD lRev;
} ST_EDGE;

#define MAX_V   (1000)

struct Flow {
private:
    vector<ST_EDGE> avGraph[MAX_V];
    SDWORD   lEdgeNum;
    SDWORD   alDist[MAX_V];
    SDWORD   alPrevV[MAX_V];   
    SDWORD   alPrevEdge[MAX_V];      /*直前の頂点と辺 */

public:
    void addEdge(SDWORD lFrom, SDWORD lTo, SDWORD lCap, SDWORD lCost) {
        avGraph[lFrom].push_back((ST_EDGE){lTo, lCap, lCost, (SDWORD)(avGraph[lTo].size())});
        avGraph[lTo].push_back((ST_EDGE){lFrom, 0, -lCost, (SDWORD)(avGraph[lFrom].size() - 1)});
    }

    SDWORD minCostFlow(SDWORD lStart, SDWORD lTarget, SDWORD lFlow)
    {
        SDWORD lRes = 0;
        SDWORD lFlowRest = lFlow;
        while (0 < lFlowRest) {
            /* Bellman-Ford Method */
            fill(alDist, alDist + ArrayLength(alDist), MAX_SDWORD);
            alDist[lStart] = 0;
            bool bIsUpdate = true;
            while (bIsUpdate) {
                bIsUpdate = false;
                for (SDWORD lV = 0; lV < MAX_V; lV++) {
                    for (SDWORD lIdx = 0; lIdx < avGraph[lV].size(); lIdx++) {
                        ST_EDGE &stE = avGraph[lV][lIdx];
                        if ((alDist[lV] != MAX_SDWORD)
                                && (0 < stE.lCap)
                                && (alDist[lV] + stE.lCost < alDist[stE.lTo])) {
                            alDist[stE.lTo] = alDist[lV] + stE.lCost;
                            alPrevV[stE.lTo] = lV;
                            alPrevEdge[stE.lTo] = lIdx;
                            bIsUpdate = true;
                        }
                    }
                }
            }
            if (alDist[lTarget] == MAX_SDWORD) {
                return -1;
            }

            SDWORD lAddFlow = MAX_SDWORD;
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                lAddFlow = min(lAddFlow, avGraph[alPrevV[lV]][alPrevEdge[lV]].lCap);
            }
            lFlowRest -= lAddFlow;
            lRes += lAddFlow * alDist[lTarget];      // 後で目的変数は書き換える
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                ST_EDGE &stE = avGraph[alPrevV[lV]][alPrevEdge[lV]];
                stE.lCap -= lAddFlow;
                avGraph[lV][stE.lRev].lCap += lAddFlow;
            }
        }
        return lRes;
    }
};

#define POS_R (901)
#define POS_G (902)
#define POS_B (903)

#define PNT_NO_START    (904)
#define PNT_NO_TARGET   (905)

#define PNT_NO_OFS  (450)

int main()
{
    DWORD dwInput_R, dwInput_G, dwInput_B;

    dwInput_R = inputSDWORD();
    dwInput_G = inputSDWORD();
    dwInput_B = inputSDWORD();

    Flow stFlow;

    /* Start = 10000, Goal = 10001 */
    stFlow.addEdge(PNT_NO_START, POS_R, dwInput_R, 0);
    stFlow.addEdge(PNT_NO_START, POS_G, dwInput_G, 0);
    stFlow.addEdge(PNT_NO_START, POS_B, dwInput_B, 0);

    for (SDWORD lPntNo = -PNT_NO_OFS; lPntNo <= PNT_NO_OFS; lPntNo++) {
        stFlow.addEdge(POS_R, (lPntNo + PNT_NO_OFS), 1, abs(lPntNo - (-100)));
        stFlow.addEdge(POS_G, (lPntNo + PNT_NO_OFS), 1, abs(lPntNo - 0));
        stFlow.addEdge(POS_B, (lPntNo + PNT_NO_OFS), 1, abs(lPntNo - 100));
        
        stFlow.addEdge((lPntNo + PNT_NO_OFS), PNT_NO_TARGET, 1, 0);
    }

    SDWORD lMinCost = stFlow.minCostFlow(PNT_NO_START, PNT_NO_TARGET, (dwInput_R + dwInput_G + dwInput_B));
    printf("%d\n", lMinCost);

    return 0;
}
