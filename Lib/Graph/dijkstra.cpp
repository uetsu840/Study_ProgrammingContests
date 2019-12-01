#include "common.h"

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

void solveDijkstra(
    SQWORD sqFrom, 
    const vector<EDGE_ST> *pvstEdges,
    vector<SQWORD> &cost)
{
    typedef pair<SQWORD, SQWORD> P;
    priority_queue<P, vector<P>, greater<P>> que;
    
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
}

