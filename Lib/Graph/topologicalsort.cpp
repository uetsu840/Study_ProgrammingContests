#include "common.h"

#define MAX_B   (3000)

int main(void)
{
    SQWORD sqN;
    SQWORD sqM;

    vector<vector<SQWORD>> vvsqGraph(sqN + 1, vector<SQWORD>{});
    set<SQWORD> setFrontNodes;
    vector<SQWORD> vsqInCnt(sqN + 1, 0);

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqFrom;
        SQWORD sqTo;
        vvsqGraph[sqFrom].emplace_back(sqTo);
        vsqInCnt[sqTo]++;
    }

    /* 先頭ノードを求める */
    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        if (0 == vsqInCnt[sqNode]) {
            setFrontNodes.insert(sqNode);
        }
    }

    vector<SQWORD> vsqResult;
    bool bMultipoeOrder = false;
    for (;;) {
        /* remove front node */
        if (0 == setFrontNodes.size()) {
            break;
        }
        if (1 < setFrontNodes.size()) {
            bMultipoeOrder = true;
        }

        auto it = setFrontNodes.begin();
        SQWORD sqNode = *it;
        setFrontNodes.erase(*it);
        vsqResult.emplace_back(sqNode);
        for (auto to: vvsqGraph[sqNode]) {
            vsqInCnt[to]--;
            if (0 == vsqInCnt[to]) {
                setFrontNodes.insert(to);
            }
        }
    }

    for (auto node: vsqResult) {
        printf("%lld\n", node);
    }
    if (bMultipoeOrder) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    return 0;
}
