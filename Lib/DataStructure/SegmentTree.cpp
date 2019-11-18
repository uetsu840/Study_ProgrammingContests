#include "common.h"

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqX;
    SQWORD sqY;

public:
    SEGMENT_NODE_ST(SQWORD x, SQWORD y) : sqX(x), sqY(y) {};
    SEGMENT_NODE_ST() : sqX(0), sqY(0) {};
    static SEGMENT_NODE_ST getInvalidVal()
    {
        return SEGMENT_NODE_ST{0, 0};
    }
    static SEGMENT_NODE_ST uniteNode(SEGMENT_NODE_ST nodeA,  SEGMENT_NODE_ST nodeB)
    {
        SEGMENT_NODE_ST stRet;

        stRet.sqX = nodeA.sqX + nodeB.sqX;
        stRet.sqY = max(nodeA.sqY + nodeB.sqX, nodeB.sqY);
        return stRet;
    }
    void init() {
        sqX = 0;
        sqY = 0;
    }
    void debugPrint() {
        printf("<%lld %lld> ", sqX, sqY);
    }
};


template<typename T>
class SegmentTree {
private:
    DWORD dwBaseSize;
    vector<T> vNode;

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

    static inline T updateNode(
        const T &stL,
        const T &stR)
    {
        T stRet;
        
        stRet = T::uniteNode(stL, stR);
        return stRet;
    }

    void initSegmentTree(vector<T> &v) {
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
    SegmentTree(vector<T> v) {
        initSegmentTree(v);
    }

    T getNode(DWORD dwNodeIdx) {
        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);
        return vNode[lTreeIdx];
    }

    void update(DWORD dwNodeIdx, const T &stNewVal) {
        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);

        // 最下段のノードを更新したら、あとは親に上って更新していく
        vNode[lTreeIdx] = stNewVal;
        while(lTreeIdx > 0) {
            lTreeIdx = (lTreeIdx - 1) / 2;
            vNode[lTreeIdx] = updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2]);
        }
    }

    T GetValue(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return T::getInvalidVal();
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx];
        }

        T sqResL, sqResR;
        SQWORD lCenter = (lLeft + lRight) / 2;
        sqResL = GetValue(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter);
        sqResR = GetValue(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight);
        return T::uniteNode(sqResL, sqResR);
    }
};
