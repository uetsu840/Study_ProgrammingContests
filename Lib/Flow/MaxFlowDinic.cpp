#include "common.h"

typedef struct {
    SDWORD lTo;
    SQWORD sqCap;
    SQWORD sqCost;
    SDWORD lRev;
} ST_EDGE;

#define MAX_V   (20002)

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
        // printf("%d ----[%lld] ----> %d\n", lFrom, sqCap, lTo);

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
