#include "common.h"

struct EDGE_ST {
    SQWORD sqTo;

    EDGE_ST(SQWORD to) {
        sqTo = to;
    };
};

#define N_MAX_NODES     (100000)
#define MAX_LOG_NODES   (20)

class SCC_Graph {
    vector<vector<EDGE_ST>> vvstEdge;
    vector<vector<EDGE_ST>> vvstRevEdge;
    vector<SQWORD> vsqInOrderFwd;
    SQWORD  sqNumNode;
    vector<SQWORD>  vsqOrder;
    vector<SQWORD>  vsqComp;

    void dfs(SQWORD sqNode, vector<bool> &vbIsVisited)
    {
        if (vbIsVisited[sqNode]) {
            return;
        }
        vbIsVisited[sqNode] = true; 
        for (auto e: vvstEdge[sqNode]) {
            dfs(e.sqTo, vbIsVisited);
        }
        vsqOrder.emplace_back(sqNode);
    }

    void rdfs(SQWORD sqNode, SQWORD sqCnt)
    {
        if (vsqComp[sqNode] != -1) {
            return;
        }
        vsqComp[sqNode] = sqCnt;
        for(auto to : vvstRevEdge[sqNode]) {
            rdfs(to.sqTo, sqCnt);
        }
    }

public:
    SCC_Graph(SQWORD sqN) {
        sqNumNode = sqN;
        vvstEdge.resize(sqNumNode + 1, vector<EDGE_ST>{});
        vvstRevEdge.resize(sqNumNode + 1, vector<EDGE_ST>{});
        vsqComp.resize(sqNumNode + 1, -1);
    }

    void RegistEdge(SQWORD sqA, SQWORD sqB)
    {
        vvstEdge[sqA].emplace_back(sqB);
        vvstRevEdge[sqB].emplace_back(sqA);
    }
        
    SQWORD operator[](SQWORD k) {
        return vsqComp[k];
    }

    void Build(vector<vector<EDGE_ST>> &t) {
        vector<bool>  vIsVisitedFwd(sqNumNode, false);
        vector<bool>  vIsVisitedRev(sqNumNode, false);

        for (SQWORD sqStart = 1; sqStart < sqNumNode + 1; sqStart++) {
            dfs(sqStart, vIsVisitedFwd);
        }

        reverse(vsqOrder.begin(), vsqOrder.end());
        SQWORD ptr = 0;
        for (auto rStart: vsqOrder) {
            if (vsqComp[rStart] == -1) {
                rdfs(rStart, ptr);
                ptr++;
            }
        }

        t.resize(ptr);
        for(SQWORD sqNode = 1; sqNode < sqNumNode + 1; sqNode++) {
            for(auto &to : vvstEdge[sqNode]) {
                SQWORD sqX = vsqComp[sqNode], sqY = vsqComp[to.sqTo];
                printf("[%lld -> %lld / %lld %lld]\n", sqNode, to.sqTo, sqX, sqY);
                if (sqX != sqY) {
                    t[sqX].push_back(EDGE_ST{sqY});
                }
            }
        }
    }
};
