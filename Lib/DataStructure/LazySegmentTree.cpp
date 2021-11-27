#include "common.h"

#define SQWORD_INF                  (100100100100100100)
#define SEGMENT_INIT_VAL            (0)
#define UPDATE_INIT_VAL             (0)
#define SEGMENT_INVALID_EVAL_VAL    (SQWORD_INF)

/**
 *  遅延評価セグメント木
 */
struct SEGMENT_VAL_ST {
    SQWORD sqVal;

    SEGMENT_VAL_ST (SQWORD val) {
        sqVal = val;
    }
    SEGMENT_VAL_ST () {
        sqVal = SEGMENT_INIT_VAL;
    }
    void init(void) {
        sqVal = SEGMENT_INIT_VAL;
    }
    void updateVal(SQWORD val, SQWORD sqMultiplier = 1) {
        sqVal += val;
    }
    void clear() {
        sqVal = 0;
    }
    static SEGMENT_VAL_ST getInvalidVal () {
        SEGMENT_VAL_ST ret;
        ret.sqVal = SEGMENT_INVALID_EVAL_VAL;
        return ret;
    }
    static SEGMENT_VAL_ST evaluate(
        const SEGMENT_VAL_ST &a, 
        const SEGMENT_VAL_ST &b)
    {
        SEGMENT_VAL_ST ret;
        ret.sqVal = min(a.sqVal, b.sqVal);
        return ret;
    }
    static SEGMENT_VAL_ST update(
        const SEGMENT_VAL_ST &lazy, 
        const SEGMENT_VAL_ST &val,
        SQWORD sqMultiplier)
    {
        SEGMENT_VAL_ST ret = val;
        ret.updateVal(lazy.sqVal, sqMultiplier);
        return ret;
    }
};


/**
 *  遅延評価セグメント木
 */
struct SEGMENT_NODE_ST {
    SEGMENT_VAL_ST stVal;     /* 値 (担当区間の最小値) */
    SEGMENT_VAL_ST stLazy;    /* 遅延評価 (加算値)*/
    bool bLazyFlag;  /* 遅延評価フラグ */

public:
    SEGMENT_NODE_ST () {
        stVal = SEGMENT_VAL_ST::getInvalidVal();
        stLazy = SEGMENT_VAL_ST(UPDATE_INIT_VAL);
        bLazyFlag = false;
    }

    SEGMENT_NODE_ST (SQWORD v) {
        stVal = SEGMENT_VAL_ST(v);
        stLazy = SEGMENT_VAL_ST(UPDATE_INIT_VAL);
        bLazyFlag = false;
    }

    SEGMENT_NODE_ST (SQWORD v, SQWORD l) {
        stVal = SEGMENT_VAL_ST(v);
        stLazy = SEGMENT_VAL_ST(l);
        bLazyFlag = false;
    }

    static SEGMENT_NODE_ST getInvalidVal() {
        SEGMENT_NODE_ST stRet;
        return stRet;
    }

    static SEGMENT_NODE_ST evaluateFunc(
        const SEGMENT_NODE_ST &stNodeL,
        const SEGMENT_NODE_ST &stNodeR)
    {
        SEGMENT_NODE_ST stRet;
        stRet.stVal = SEGMENT_VAL_ST::evaluate(stNodeL.stVal, stNodeR.stVal);
        return stRet;
    };

    static SEGMENT_NODE_ST getNodeVal(
        const SEGMENT_NODE_ST &stN,
        SQWORD sqMemberNum)
    {
        SEGMENT_NODE_ST stRet;
        if (stN.bLazyFlag) {
            stRet.stVal = SEGMENT_VAL_ST::update(stN.stLazy, stN.stVal, sqMemberNum);
        } else {
            stRet.stVal = stN.stVal;          
        }

        return stRet;
    }

    static void refreshLazyVal(
        SEGMENT_NODE_ST &stN,
        SQWORD sqMemberNum)
    {
        stN.stVal  = SEGMENT_VAL_ST::update(stN.stLazy, stN.stVal, sqMemberNum);
        stN.bLazyFlag = false;
        stN.stLazy.clear();
    }

    static void updateLazyVal(
        SEGMENT_NODE_ST &stN,
        const SEGMENT_NODE_ST &stX)
    {
        /* lazy同氏の足し算は要素数を掛けない */
        stN.stLazy = SEGMENT_VAL_ST::update(stX.stLazy, stN.stLazy, 1);
        stN.bLazyFlag = true;
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
                sqIdx, vNode[sqIdx].stVal.sqVal, vNode[sqIdx].stLazy.sqVal, vNode[sqIdx].bLazyFlag);
        }
    }


    void initSegmentTree(
        const vector<SEGMENT_NODE_ST> &vstInitVal)
    {
        /**
         *  最下段のノード数は元配列のサイズ以上になる最小の 2 冪 -> これを n とおく
         * セグメント木全体で必要なノード数は 2n-1 個である
         */
        DWORD dwSize = vstInitVal.size();
        dwBaseSize = 1; 
        while (dwBaseSize < dwSize) {
            dwBaseSize *= 2;
        }
        vNode.resize(2 * dwBaseSize - 1);

        /**
         *  最下段に値を入れたあとに、下の段から順番に値を入れる
         * 値を入れるには、自分の子の 2 値を参照すれば良い
         */
        for(SDWORD lIdx = 0; lIdx < vstInitVal.size(); lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)] = vstInitVal[lIdx];
        }
        for (SDWORD lIdx = dwBaseSize - 2; 0 <= lIdx; lIdx--) {
            vNode[lIdx] = SEGMENT_NODE_ST::evaluateFunc(vNode[lIdx*2+1], vNode[lIdx*2+2]);
        }
    }

    void lazyEvaluate(int lNodeIdx, int lLeft, int lRight) {
        if (vNode[lNodeIdx].bLazyFlag) {
            if (lRight - lLeft > 1) {
                SEGMENT_NODE_ST::updateLazyVal(vNode[lNodeIdx*2+1], vNode[lNodeIdx]);
                SEGMENT_NODE_ST::updateLazyVal(vNode[lNodeIdx*2+2], vNode[lNodeIdx]);
            }
            SEGMENT_NODE_ST::refreshLazyVal(vNode[lNodeIdx], lRight - lLeft);
        }
    }

public:
    void UpdateValue(
        SDWORD lA, 
        SDWORD lB, 
        SEGMENT_VAL_ST stX,      
        SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return;
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            SEGMENT_NODE_ST stUpdate;
            stUpdate.stLazy = stX;
            SEGMENT_NODE_ST::updateLazyVal(vNode[lNodeIdx], stUpdate);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);

            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            UpdateValue(lA, lB, stX, lNodeIdx_L, lLeft, lCenter);
            UpdateValue(lA, lB, stX, lNodeIdx_R, lCenter, lRight);
            vNode[lNodeIdx] = SEGMENT_NODE_ST::evaluateFunc(SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx_L], lRight - lCenter), 
                                                            SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx_R], lCenter - lLeft));
        }
    };

    SEGMENT_NODE_ST FindVal(
        SDWORD lA, 
        SDWORD lB, 
        SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return SEGMENT_NODE_ST::getInvalidVal();
        }
        
        if ((lA <= lLeft) && (lRight <= lB)) {
            return SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx], lRight - lLeft);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);
            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            SEGMENT_NODE_ST stValL = FindVal(lA, lB, lNodeIdx_L, lLeft, lCenter);
            SEGMENT_NODE_ST stValR = FindVal(lA, lB, lNodeIdx_R, lCenter, lRight);
            return SEGMENT_NODE_ST::evaluateFunc(stValL, stValR);
        }
    };

    /* 元配列 v をセグメント木で表現する */
    LazySegmentTree(const vector<SEGMENT_NODE_ST> &init) {
        initSegmentTree(init);
    }
};

