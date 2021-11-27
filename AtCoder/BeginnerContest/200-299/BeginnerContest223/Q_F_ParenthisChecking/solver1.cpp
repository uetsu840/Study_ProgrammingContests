#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
#include <list>
using namespace std;

using QWORD  = uint64_t;
using SQWORD = int64_t;
using DWORD  = uint32_t;
using SDWORD = int32_t;
using WORD   = uint16_t;
using SWORD  = int16_t;
using BYTE   = uint8_t;
using SBYTE  = int8_t;
using DOUBLE = double;
using LDOUBLE = long double;
using FLOAT  = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0x7FFFFFFFFFFFFFFF)

#define MAX_QWORD  (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD  (0xFFFFFFFF)
#define MAX_WORD   (0xFFFF)
#define MAX_BYTE   (0xFF)

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) {return 0 < a ? a: -a;}

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

static inline void inputStringSpSeparated(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c) || (' ' == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}

static inline void inputString(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}


static inline SQWORD inputSQWORD(void)
{
    SQWORD sqNumber = 0;
    SQWORD sqMultiplier = 1;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                sqMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            sqNumber *= 10LL;
            sqNumber += (SQWORD)(c - '0');
            bRead = true;
        } else {
            if (bRead) {
                return sqNumber * sqMultiplier;
            }
        }
    }
}


static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    SDWORD lMultiplier = 1;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                lMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = true;
        } else {
            if (bRead) {
                return lNumber * lMultiplier;
            }
        }
    }
}

static inline DOUBLE inputFP(void)
{
    DOUBLE dInt = 0.0;
    DOUBLE dFrac = 0.0;
    DOUBLE dMultiplier = 1.0;
    DWORD dwFpCnt = 0;
    DOUBLE *pdCur = &dInt;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                dMultiplier = -1;
            }
        }
        if ('.' == c) {
            pdCur = &dFrac;
        } else if (('0' <= c) && (c <= '9')) {
            (*pdCur) *= 10;
            (*pdCur) += (DOUBLE)(c - '0');
            bRead = true;
            if (pdCur == &dFrac) {
                dwFpCnt++;
            }
        } else {
            if (bRead) {
                return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0 , (DOUBLE)dwFpCnt)));
            }
        }
    }
}

/*---------------------------------------------*/

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
        ret.sqVal = MIN(a.sqVal, b.sqVal);
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

/*----------------------------------------------*/

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


/*----------------------------------------------*/

/**
 *  BIT
 *      インデックスは 1 ～ N であることに注意する。
 * 
 */
template<typename T>
class BinaryIndexedTree {
    SDWORD lBitN;
    vector<T> _vecBit; 

public:
    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        _vecBit.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < _vecBit.size(); dwIdx++) {
            _vecBit[dwIdx] = 0;
        }
    }

    T Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        T _sum = 0;
        while (0 < lCur) {
            _sum += _vecBit[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return _sum;
    }

    void Add(SDWORD lIdx, T _x)
    {
        while (lIdx <= lBitN) {
            _vecBit[lIdx] += _x;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(T _sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (_sum < Sum(sqMid));
            } else {
                bJudge = (_sum <= Sum(sqMid));
            }

            if (bJudge) {
                sqUb = sqMid;
            } else {
                sqLb = sqMid;
            }
        }
        return sqUb;
    }


    /**
    *  累積和が指定した数より大きくなるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumUpperBound(T _sum) 
    {
        return binSearchExec(_sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(T _sum) 
    {
        return binSearchExec(_sum, false);
    }

    SDWORD End() {
        return lBitN;
    }

    void DebugPrint(SDWORD lMax) {
        for (SDWORD lIdx = 0; lIdx <= lMax; lIdx++) {
            printf("%lld ", Sum(lIdx));
        }
        printf("\n");
    }
};

/*----------------------------------------------*/

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    string strS;
    cin >> strS;

    BinaryIndexedTree<SQWORD> bit(sqN);
    vector<SEGMENT_NODE_ST> vstSegNodes(sqN, (SQWORD)0);
    LazySegmentTree seg(vstSegNodes);    

    /* Initialize */
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        if (strS[sqIdx] == '(') {
            bit.Add(sqIdx + 1, 1);
            seg.UpdateValue(sqIdx, sqN, 1);
        } else {
            bit.Add(sqIdx + 1, -1);
            seg.UpdateValue(sqIdx, sqN, -1);
        }
    }

    /* Query */
    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqType = inputSQWORD();
        SQWORD sqL = inputSQWORD();
        SQWORD sqR = inputSQWORD();

        switch (sqType) {
        case 1:
            {
                SQWORD sqNumL = bit.Sum(sqL) - bit.Sum(sqL - 1);
                SQWORD sqNumR = bit.Sum(sqR) - bit.Sum(sqR - 1);
                bit.Add(sqL, -1 * sqNumL);
                bit.Add(sqR, -1 * sqNumR);

                bit.Add(sqL, sqNumR);
                bit.Add(sqR, sqNumL);

                if (sqNumL == sqNumR) {
                    ;
                } else if (sqNumL == -1) {
                    /* L = -1, R = 1 */
                    seg.UpdateValue(sqL-1, sqR-1, 2);
                } else {
                    seg.UpdateValue(sqL-1, sqR-1, -2);
                }
            }
            break;
        case 2:
            {
                SQWORD sqSum = bit.Sum(sqR) - bit.Sum(sqL - 1);
                SQWORD sqStartHeight = bit.Sum(sqL - 1);
//                printf("%lld %lld\n", bit.Sum(sqR), bit.Sum(sqL - 1));
                SQWORD sqMin = seg.FindVal(sqL-1, sqR).stVal.sqVal;
//                printf("h: %lld min %lld\n", sqStartHeight,  sqMin);
                if ((0 == sqSum) && (sqStartHeight <= sqMin)) {
                    printf("Yes\n");
                } else {
                    printf("No\n");
                }
            }
            break;
        }
    }
    
    return 0;
}
