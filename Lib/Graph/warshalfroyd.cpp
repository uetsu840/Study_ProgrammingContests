#include "common.h"

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

#define DIST_INF        (100100100100100100)
#define N_MAX_VERTICE   (300)

class WarshallFloyd {
    SQWORD  sqNumVertice;
    const vector<EDGE_ST> *pvecEdge;
    SQWORD  aaDist[N_MAX_VERTICE + 1][N_MAX_VERTICE + 1];

public:
    WarshallFloyd(SQWORD sqV, const vector<EDGE_ST> *vE) : sqNumVertice(sqV), pvecEdge(vE) {
        for (SWORD sqI = 1; sqI <= sqV; sqI++) {
            for (SQWORD sqJ = 1; sqJ <= sqNumVertice; sqJ++) {
                aaDist[sqI][sqJ] = DIST_INF;
            }
        }
        for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
            for (auto v:pvecEdge[sqI]) {
                aaDist[sqI][v.sqTo] = v.sqCost;
            }
        }
        for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
            aaDist[sqI][sqI] = 0;
        }
    };

    void Solve(void)
    {
        for (SQWORD sqK = 1; sqK <= sqNumVertice; sqK++) {
            for (SQWORD sqI = 1; sqI <= sqNumVertice; sqI++) {
                for (SQWORD sqJ = 1; sqJ <= sqNumVertice; sqJ++) {
                    aaDist[sqI][sqJ] = min(aaDist[sqI][sqJ],
                                            aaDist[sqI][sqK] + aaDist[sqK][sqJ]);
                } 
            }
        }
    }
};