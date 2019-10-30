#include "common.h"

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;
    SQWORD sqColor;

    EDGE_ST(SQWORD to, SQWORD color, SQWORD cost) {
        sqTo = to;
        sqColor = color;
        sqCost = cost;
    };
};

#define N_MAX_NODES     (100000)
#define MAX_LOG_NODES   (20)

struct TREE {
    vector<EDGE_ST> avecstEdge[N_MAX_NODES + 1];
    SQWORD sqNumNode;
    SQWORD aasqParent[MAX_LOG_NODES][N_MAX_NODES + 1];
    SQWORD asqDepth[N_MAX_NODES + 1];
    SQWORD asqCost[N_MAX_NODES + 1];        /* rootからのコスト */

    TREE(SQWORD sqN) {
        sqNumNode = sqN;
    }

    void retistEdge(SQWORD sqA, SQWORD sqB, SQWORD sqColor, SQWORD sqCost)
    {
        avecstEdge[sqA].emplace_back(sqB, sqColor, sqCost);
        avecstEdge[sqB].emplace_back(sqA, sqColor, sqCost);
    }

    void dfs(SQWORD sqNode, SQWORD sqP = -1, SQWORD sqDepth = 0, SQWORD sqCost = 0)
    {
        aasqParent[0][sqNode] = sqP;
        asqDepth[sqNode] = sqDepth;
        asqCost[sqNode] = sqCost;
        for (auto edge:avecstEdge[sqNode]) {
            if (edge.sqTo != sqP) {
                dfs(edge.sqTo, sqNode, sqDepth + 1, sqCost + edge.sqCost);
            }
        }
    }

    /**
     *  initialize
     */    
    void init(SQWORD sqRoot) 
    {
        /* measure Depth */
        dfs(sqRoot);

        /* initialize parent */
        for (SQWORD sqLogIdx = 0; sqLogIdx + 1 < MAX_LOG_NODES; sqLogIdx++) {
            for (SQWORD sqNode = 1; sqNode <= sqNumNode; sqNode++) {
                if (aasqParent[sqLogIdx][sqNode] < 0) {
                    aasqParent[sqLogIdx + 1][sqNode] = -1;
                } else {
                    aasqParent[sqLogIdx + 1][sqNode] = aasqParent[sqLogIdx][aasqParent[sqLogIdx][sqNode]];
                }
            }
        }
    }

    /**
     * calculate lca of U and V
     */
    SQWORD lca(SQWORD sqU, SQWORD sqV)
    {
        /* u, v が同じ深さになるまで親を辿る */
        if (asqDepth[sqU] > asqDepth[sqV]) {
            swap(sqU, sqV);
        }
        for (SQWORD sqLogIdx = 0; sqLogIdx < MAX_LOG_NODES; sqLogIdx++) {
            if (((asqDepth[sqV] - asqDepth[sqU]) >> sqLogIdx) & 0x1) {
                sqV = aasqParent[sqLogIdx][sqV];
            }
        }
        if (sqU == sqV) {
            return sqU;
        }

        /* 二分探索でLCAを求める */
        for (SQWORD sqLogIdx = MAX_LOG_NODES - 1; 0 <= sqLogIdx; sqLogIdx--) {
            if (aasqParent[sqLogIdx][sqU] != aasqParent[sqLogIdx][sqV]) {
                sqU = aasqParent[sqLogIdx][sqU];
                sqV = aasqParent[sqLogIdx][sqV];
            }
        }

        return aasqParent[0][sqU];
    }
};

int main(void)
{
    SQWORD sqInput_N;
    SQWORD sqInput_Q;
    cin >> sqInput_N >> sqInput_Q;
    static TREE    stTree(sqInput_N);

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N - 1; sqIdx++) {
        SQWORD sqInput_a, sqInput_b, sqInput_c, sqInput_d;
        cin >> sqInput_a >> sqInput_b >> sqInput_c >> sqInput_d;
        stTree.retistEdge(sqInput_a, sqInput_b, sqInput_c, sqInput_d);
    }
    stTree.init(1);

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqInput_Q; sqQueryIdx++) {
//        SQWORD sqLca = stTree.lca();
    }
}
