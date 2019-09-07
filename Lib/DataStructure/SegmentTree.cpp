#include "common.h"

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqVal;

public:
    static SQWORD getInvalidVal()
    {
        return 0;
    }
    static SQWORD uniteVal(SQWORD sqValA, SQWORD sqValB)
    {
        return max(sqValA, sqValB);
    }
    void init() {
        sqVal = getInvalidVal();
    }
    void debugPrint() {
        printf("%lld ", sqVal);
    }
};

struct SegmentTree {
private:
    DWORD dwBaseSize;
    vector<SEGMENT_NODE_ST> vNode;

private:
    void debugPrint(void)
    {
        printf("====\n");
        for (SDWORD lIdx = 0; lIdx < vNode.size(); lIdx++) {
            printf(" [%d] ", lIdx);
            vNode[lIdx].debugPrint();
            printf("\n");
        }
    }

    static inline SEGMENT_NODE_ST updateNode(
        const SEGMENT_NODE_ST &stL,
        const SEGMENT_NODE_ST &stR)
    {
        SEGMENT_NODE_ST stRet;
        
        stRet.sqVal = SEGMENT_NODE_ST::uniteVal(stR.sqVal, stL.sqVal);
        return stRet;
    }

    void initSegmentTree(vector<SEGMENT_NODE_ST> &v) {
        /**
         *  最下段のノード数は元配列のサイズ以上になる最小の 2 冪 -> これを n とおく
         * セグメント木全体で必要なノード数は 2n-1 個である
         */
        DWORD dwSize = v.size();
        dwBaseSize = 1; 
        while (dwBaseSize < dwSize) {
            dwBaseSize *= 2;
        }
        vNode.resize(2 * dwBaseSize - 1);

        /**
         *  最下段に値を入れたあとに、下の段から順番に値を入れる
         * 値を入れるには、自分の子の 2 値を参照すれば良い
         */
        /* 最下段 */
        for (SDWORD lIdx = 0; lIdx < dwSize; lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)] = v[lIdx];
        }
        for(SDWORD lIdx = dwSize; lIdx < dwBaseSize; lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)].init();
        }
        /* 2段目以降 */
        for (SDWORD lIdx = dwBaseSize - 2; 0 <= lIdx; lIdx--) {
            vNode[lIdx] = updateNode(vNode[lIdx*2+1], vNode[lIdx*2+2]);
        }
    }

public:
    /* 元配列 v をセグメント木で表現する */
    SegmentTree(vector<SEGMENT_NODE_ST> v) {
        initSegmentTree(v);
    }

    void update(DWORD dwNodeIdx, const SEGMENT_NODE_ST &stNewVal) {

        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);

        // 最下段のノードを更新したら、あとは親に上って更新していく
        vNode[lTreeIdx] = stNewVal;
        while(lTreeIdx > 0) {
            lTreeIdx = (lTreeIdx - 1) / 2;
            vNode[lTreeIdx] = updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2]);
        }
    }

    SQWORD GetMax(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return SEGMENT_NODE_ST::getInvalidVal();
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx].sqVal;
        }

        SQWORD sqResL, sqResR;
        SQWORD lCenter = (lLeft + lRight) / 2;
        sqResL = GetMax(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter);
        sqResR = GetMax(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight);
        return SEGMENT_NODE_ST::uniteVal(sqResL, sqResR);
    }
};
