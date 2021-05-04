#include "common.h"

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

#define COST_INF        (100100100100100100)
#define N_MAX_VERTICE   (10000)

class Prim {
private:
    SQWORD  sqNumVertice;
    const   vector<EDGE_ST> *pvecEdge;
    SQWORD  aMinCost[N_MAX_VERTICE + 1];        /* 部分集合から頂点までの最小コスト */
    bool    abIsUsed[N_MAX_VERTICE + 1];
    SQWORD  aaCost[N_MAX_VERTICE + 1][N_MAX_VERTICE + 1];
    SQWORD  sqIdxMinV;
    SQWORD  sqIdxMaxV;

public:
    Prim(
        SQWORD sqV, 
        const vector<EDGE_ST> *vE,
        bool bStartWithZero = false) : sqNumVertice(sqV), pvecEdge(vE) 
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
            for (auto v:pvecEdge[sqI]) {
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
