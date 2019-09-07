#include "common.h"

#define SQWORD_INF  (100100100100100100)

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqVal;     /* 値 (担当区間の最小値) */
    SQWORD sqLazy;    /* 遅延評価 (加算値)*/
    bool bLazyFlag;  /* 遅延評価フラグ */

public:
    void init(SQWORD sqInitVal) {
        sqVal = sqInitVal;
        sqLazy = 0;
        bLazyFlag = false;
    }
};

struct LazySegmentTree {
private:
    DWORD dwBaseSize;
    vector<SEGMENT_NODE_ST> vNode;

private:
    void debugPrint(void) {
        for (SQWORD sqIdx = 0; sqIdx < dwBaseSize * 2 - 1; sqIdx++) {
            printf("[%lld : v:%lld l:%lld] f[%d]\n", 
                sqIdx, vNode[sqIdx].sqVal, vNode[sqIdx].sqLazy, vNode[sqIdx].bLazyFlag);
        }
    }

    static inline SQWORD evaluateFunc(
        const SQWORD sqValL,
        const SQWORD sqValR)
    {
        return min(sqValL, sqValR);
    }

    static inline SQWORD getNodeVal(
        const SEGMENT_NODE_ST &stN)
    {
        if (stN.bLazyFlag) {
            return stN.sqLazy;
        } else {
            return stN.sqVal;          
        }
    }

    static inline void refreshLazyVal(
        SEGMENT_NODE_ST &stN)
    {
        stN.sqVal = stN.sqLazy;
        stN.bLazyFlag = false;
        stN.sqLazy    = 0;
    }

    static inline void updateLazyVal(
        SEGMENT_NODE_ST &stN,
        SQWORD sqX)
    {
        stN.sqLazy = sqX;
        stN.bLazyFlag = true;
    }

    void initSegmentTree(
        vector<SQWORD> vsqInitVal)
    {
        /**
         *  最下段のノード数は元配列のサイズ以上になる最小の 2 冪 -> これを n とおく
         * セグメント木全体で必要なノード数は 2n-1 個である
         */
        DWORD dwSize = vsqInitVal.size();
        dwBaseSize = 1; 
        while (dwBaseSize < dwSize) {
            dwBaseSize *= 2;
        }
        vNode.resize(2 * dwBaseSize - 1);

        /**
         *  最下段に値を入れたあとに、下の段から順番に値を入れる
         * 値を入れるには、自分の子の 2 値を参照すれば良い
         */
        for(SDWORD lIdx = 0; lIdx < vsqInitVal.size(); lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)].init(vsqInitVal[lIdx]);
        }
        for (SDWORD lIdx = dwBaseSize - 2; 0 <= lIdx; lIdx--) {
            vNode[lIdx].sqVal = evaluateFunc(vNode[lIdx*2+1].sqVal, vNode[lIdx*2+2].sqVal);
        }
    }

    void lazyEvaluate(int lNodeIdx, int lLeft, int lRight) {
        if (vNode[lNodeIdx].bLazyFlag) {
            if (lRight - lLeft > 1) {
                updateLazyVal(vNode[lNodeIdx*2+1], vNode[lNodeIdx].sqLazy);
                updateLazyVal(vNode[lNodeIdx*2+2], vNode[lNodeIdx].sqLazy);
            }
            refreshLazyVal(vNode[lNodeIdx]);
        }
    }

public:
    void UpdateValue(
        SDWORD lA, 
        SDWORD lB, 
        SDWORD lX,      /* update value */
        SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return;
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            updateLazyVal(vNode[lNodeIdx], lX);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);

            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            UpdateValue(lA, lB, lX, lNodeIdx_L, lLeft, lCenter);
            UpdateValue(lA, lB, lX, lNodeIdx_R, lCenter, lRight);
            vNode[lNodeIdx].sqVal = evaluateFunc(getNodeVal(vNode[lNodeIdx_L]), 
                                                 getNodeVal(vNode[lNodeIdx_R]));
        }
    };

    SQWORD FindMin(
        SDWORD lA, 
        SDWORD lB, 
        SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return SQWORD_INF;
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return getNodeVal(vNode[lNodeIdx]);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);
            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            SQWORD sqValL = FindMin(lA, lB, lNodeIdx_L, lLeft, lCenter);
            SQWORD sqValR = FindMin(lA, lB, lNodeIdx_R, lCenter, lRight);
            return evaluateFunc(sqValL, sqValR);
        }
    };

    /* 元配列 v をセグメント木で表現する */
    LazySegmentTree(vector<SQWORD> init) {
        initSegmentTree(init);
    }
};

