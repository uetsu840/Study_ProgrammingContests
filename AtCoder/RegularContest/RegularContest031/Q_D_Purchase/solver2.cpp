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

#define MAX_V   (1000)

struct Flow {
private:
    vector<ST_EDGE> avGraph[MAX_V];
    SQWORD  sqEdgeNum;
    SQWORD  asqDist[MAX_V];
    SDWORD  alPrevV[MAX_V];   
    SDWORD  alPrevEdge[MAX_V];      /*直前の頂点と辺 */

    SDWORD  alDinic_Level[MAX_V];
    SDWORD  alDinic_Iter[MAX_V];

public:
    void addEdge(SDWORD lFrom, SDWORD lTo, SQWORD sqCap, SQWORD sqCost) {
//        printf("add edge %d --[%lld]--> %d\n", lFrom, sqCap, lTo);

        avGraph[lFrom].push_back((ST_EDGE){lTo, sqCap, sqCost, (SDWORD)(avGraph[lTo].size())});
        avGraph[lTo].push_back((ST_EDGE){lFrom, 0, -sqCost, (SDWORD)(avGraph[lFrom].size() - 1)});
    }

    SDWORD minCostFlow(SDWORD lStart, SDWORD lTarget, SQWORD sqFlow)
    {
        SQWORD sqRes = 0;
        SQWORD sqFlowRest = sqFlow;
        while (0 < sqFlowRest) {
            /* Bellman-Ford Method */
            fill(asqDist, asqDist + ArrayLength(asqDist), MAX_SDWORD);
            asqDist[lStart] = 0;
            bool bIsUpdate = true;
            while (bIsUpdate) {
                bIsUpdate = false;
                for (SDWORD lV = 0; lV < MAX_V; lV++) {
                    for (SDWORD lIdx = 0; lIdx < avGraph[lV].size(); lIdx++) {
                        ST_EDGE &stE = avGraph[lV][lIdx];
                        if ((asqDist[lV] != MAX_SDWORD)
                                && (0 < stE.sqCap)
                                && (asqDist[lV] + stE.sqCost < asqDist[stE.lTo])) {
                            asqDist[stE.lTo] = asqDist[lV] + stE.sqCost;
                            alPrevV[stE.lTo] = lV;
                            alPrevEdge[stE.lTo] = lIdx;
                            bIsUpdate = true;
                        }
                    }
                }
            }
            if (asqDist[lTarget] == MAX_SDWORD) {
                return -1;
            }

            SQWORD sqAddFlow = MAX_SQWORD;
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                sqAddFlow = min(sqAddFlow, avGraph[alPrevV[lV]][alPrevEdge[lV]].sqCap);
            }
            sqFlowRest -= sqAddFlow;
            sqRes += sqAddFlow * asqDist[lTarget];      // 後で目的変数は書き換える
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                ST_EDGE &stE = avGraph[alPrevV[lV]][alPrevEdge[lV]];
                stE.sqCap -= sqAddFlow;
                avGraph[lV][stE.lRev].sqCap += sqAddFlow;
            }
        }
        return sqRes;
    }

private:
    SQWORD maxFlowDinicBfs(SDWORD lS)
    {
        memset(alDinic_Level, -1, sizeof(alDinic_Level));
        queue<SDWORD> queSearch;
        alDinic_Level[lS] = 0;
        queSearch.push(lS);
        while (!queSearch.empty()) {
            SDWORD lV = queSearch.front();
            queSearch.pop();
            for (SDWORD lIdx = 0; lIdx < avGraph[lV].size(); lIdx++) {
                ST_EDGE &stE = avGraph[lV][lIdx];
                if ((0 < stE.sqCap) && (alDinic_Level[stE.lTo] < 0)) {
                    alDinic_Level[stE.lTo] = alDinic_Level[lV] + 1;
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
        for (SDWORD &lI = alDinic_Iter[lV]; lI < avGraph[lV].size(); lI++) {
            ST_EDGE &stE = avGraph[lV][lI];
            if ((0 < stE.sqCap) && (alDinic_Level[lV] < alDinic_Level[stE.lTo])) {
                SQWORD sqDiffFlow = maxFlowDinicDfs(stE.lTo, lT, min(sqF, stE.sqCap));
                if (0 < sqDiffFlow) {
                    stE.sqCap -= sqDiffFlow;
                    avGraph[stE.lTo][stE.lRev].sqCap += sqDiffFlow;
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
            if (alDinic_Level[lTarget] < 0) {
                return sqFlow;
            }
            memset(alDinic_Iter, 0, sizeof(alDinic_Iter));
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

#define PNT_NO_START        (200)
#define PNT_NO_TARGET       (201)
#define PNT_NO_OFS_VALUES   (100)
#define MULTIPLIER_B        (10000)
#define DUMMY_COST          (0)         /* 費用。今回は使用しない */

/**
*   価値/お金 > a/bか。
*   b = 10^12固定
*/ 
static bool IsItemValueGreater(
    SDWORD lA, 
    const vector<SDWORD> &vValues, 
    const vector<SDWORD> &vPrices,
    const vector<vector<SDWORD>> &vvItemMembers)
{
    Flow solverFlow;
    SQWORD sqMaxValue = 0;

//    printf("try : %d\n", lA);

    for (DWORD dwIdx = 0; dwIdx < vValues.size(); dwIdx++) {
        const vector<SDWORD> &vItemMember = vvItemMembers[dwIdx];
        SDWORD lValuePntNo = dwIdx + PNT_NO_OFS_VALUES;
        SQWORD sqCap = (SQWORD)vValues[dwIdx] * (SQWORD)MULTIPLIER_B;

        solverFlow.addEdge(PNT_NO_START, lValuePntNo,   0, DUMMY_COST);
        solverFlow.addEdge(lValuePntNo,  PNT_NO_TARGET, sqCap, DUMMY_COST);
        for (DWORD dwItemIdx = 0; dwItemIdx < vItemMember.size(); dwItemIdx++) {
            solverFlow.addEdge(vItemMember[dwItemIdx] - 1, lValuePntNo, MAX_SQWORD, DUMMY_COST);
        }

        sqMaxValue += sqCap;
    }
    for (DWORD dwItemIdx = 0; dwItemIdx < vPrices.size(); dwItemIdx++) {
        solverFlow.addEdge(PNT_NO_START, dwItemIdx, (SQWORD)vPrices[dwItemIdx] * (SQWORD)lA, DUMMY_COST);
        solverFlow.addEdge(dwItemIdx, PNT_NO_TARGET, 0, DUMMY_COST);
    }
    SQWORD sqFlow = solverFlow.MaxFlow(PNT_NO_START, PNT_NO_TARGET);

//   printf(" max val [%lld] flow[%lld]\n", sqMaxValue, sqFlow);

    return (sqMaxValue - sqFlow) <= 0;
}

/**
*   2分探索
*/
static SDWORD binarySearch(
    bool (*judge)(SDWORD, const vector<SDWORD>&, const vector<SDWORD>&, const vector<vector<SDWORD>>&), 
    SQWORD sqInitLb, 
    SQWORD sqInitUb, 
    const vector<SDWORD> &vValues,
    const vector<SDWORD> &vPrices,
    const vector<vector<SDWORD>> &vvItemMembers)
{
    SQWORD sqLb = sqInitLb;
    SQWORD sqUb = sqInitUb;

    while (1LL < sqUb - sqLb) {
        SQWORD sqMid = (sqUb + sqLb) / 2LL;
        if (judge((QWORD)sqMid, vValues, vPrices, vvItemMembers)) {
            sqUb = sqMid;
        } else {
            sqLb = sqMid;
        }
    }
    return sqUb;
}



#define MAX_N_UNIONS    (100)
#define MAX_M_ITEMS     (100)
#define MAX_K_UM        (10)

static SDWORD s_alValues[MAX_N_UNIONS];
static SDWORD s_alPrices[MAX_M_ITEMS];

static SDWORD s_alUnionMemberNum[MAX_N_UNIONS];
static SDWORD s_aalUnionMembers[MAX_N_UNIONS][MAX_K_UM];

int main()
{
    DWORD dwInput_N, dwInput_M;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    vector<SDWORD> vValues;
    vector<SDWORD> vPrices;
    vector<vector<SDWORD>> vvItemMembers;
    SDWORD lValueMax = 0;

    for (DWORD dwUnionIdx = 0; dwUnionIdx < dwInput_N; dwUnionIdx++) {
        SDWORD lValue = inputSDWORD();
        lValueMax += lValue;
        vValues.push_back(lValue);
    }
    for (DWORD dwItemIdx = 0; dwItemIdx < dwInput_M; dwItemIdx++) {
        SDWORD lPrice = inputSDWORD();
        vPrices.push_back(lPrice);
    }

    for (DWORD dwUnionIdx = 0; dwUnionIdx < dwInput_N; dwUnionIdx++) {
        SDWORD lMemberNum = inputSDWORD();
        vector<SDWORD> vMember;
        for (DWORD dwItemIdx = 0; dwItemIdx < lMemberNum; dwItemIdx++) {
            SDWORD lMember = inputSDWORD();
            vMember.push_back(lMember);
        }
        vvItemMembers.push_back(vMember);
    }

    SDWORD lBound = binarySearch(IsItemValueGreater, 
                                    -1, 
                                    (lValueMax + 1) * MULTIPLIER_B,
                                    vValues, 
                                    vPrices, 
                                    vvItemMembers);

    printf("%0.6f\n", DOUBLE(lBound) / DOUBLE(MULTIPLIER_B));

    return 0;
}
