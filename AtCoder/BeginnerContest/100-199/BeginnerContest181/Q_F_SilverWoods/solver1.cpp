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
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

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

/*----------------------------------------------*/

#define SQWORD_INF  (100100100100100100)

/**
 *  遅延評価セグメント木
 */
struct SEGMENT_VAL_ST {
    SQWORD sqVal;

    SEGMENT_VAL_ST (SQWORD val) {
        sqVal = val;
    }
    SEGMENT_VAL_ST () {
        sqVal = MAX_SQWORD;
    }
    void init(void) {
        sqVal = MAX_SQWORD;
    }
    void setVal(SQWORD val) {
        sqVal = val;
    }
    void clear() {
        sqVal = MAX_SQWORD;
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
        const SEGMENT_VAL_ST &a, 
        const SEGMENT_VAL_ST &b)
    {
        SEGMENT_VAL_ST ret;
        ret.sqVal = min(a.sqVal, b.sqVal);
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
        stVal.init();
        stLazy.init();
        bLazyFlag = false;
    }

    SEGMENT_NODE_ST (SQWORD v) {
        stVal.setVal(v);
        stLazy.init();
        bLazyFlag = false;
    }

    SEGMENT_NODE_ST (SQWORD v, SQWORD l) {
        stVal.setVal(v);
        stLazy.setVal(l);
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
        const SEGMENT_NODE_ST &stN)
    {
        SEGMENT_NODE_ST stRet;
        if (stN.bLazyFlag) {
            stRet.stVal = SEGMENT_VAL_ST::update(stN.stLazy, stN.stVal);
        } else {
            stRet.stVal = stN.stVal;          
        }

        return stRet;
    }

    static void refreshLazyVal(
        SEGMENT_NODE_ST &stN)
    {
        stN.stVal  = SEGMENT_VAL_ST::update(stN.stVal, stN.stLazy);
        stN.bLazyFlag = false;
        stN.stLazy.clear();
    }

    static void updateLazyVal(
        SEGMENT_NODE_ST &stN,
        const SEGMENT_NODE_ST &stX)
    {
        stN.stLazy = SEGMENT_VAL_ST::update(stN.stLazy, stX.stLazy);
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
        vector<SEGMENT_NODE_ST> &vstInitVal)
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
            SEGMENT_NODE_ST::refreshLazyVal(vNode[lNodeIdx]);
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
            vNode[lNodeIdx] = SEGMENT_NODE_ST::evaluateFunc(SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx_L]), 
                                                            SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx_R]));
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
            return SEGMENT_NODE_ST::getNodeVal(vNode[lNodeIdx]);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);
            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            SEGMENT_NODE_ST stValL = FindVal(lA, lB, lNodeIdx_L, lLeft, lCenter);
            SEGMENT_NODE_ST stValR = FindVal(lA, lB, lNodeIdx_R, lCenter, lRight);
//            printf("LR : %lld %lld\n", stValL.stVal.sqVal, stValR.stVal.sqVal);
            return SEGMENT_NODE_ST::evaluateFunc(stValL, stValR);
        }
    };

    /* 元配列 v をセグメント木で表現する */
    LazySegmentTree(vector<SEGMENT_NODE_ST> init) {
        initSegmentTree(init);
    }
};


/*----------------------------------------------*/

struct EDGE {
    SQWORD sqTo;
    DOUBLE dDist;

    EDGE (SQWORD t, DOUBLE d) : sqTo(t), dDist(d) {};
};

#define MAX_N   (100)

struct PROBLEM_ONE {
    SQWORD sqN;
    vector<vector<EDGE>> vvEdge;
};


static bool bPassable(DOUBLE dJudgeDist, const PROBLEM_ONE *problem)
{
    queue<SQWORD> queSearch;

    queSearch.push(problem->sqN);

    vector<bool> vbReachable(problem->sqN + 2, false);
    while (!queSearch.empty()) {
        SQWORD sqPnt = queSearch.front();
        queSearch.pop();
        for (auto e: problem->vvEdge[sqPnt]) {
            if ((e.dDist < dJudgeDist) && (!(vbReachable[e.sqTo]))) {
                vbReachable[e.sqTo] = true;
                queSearch.push(e.sqTo);
            }
        }
    }

    if (!vbReachable[problem->sqN + 1]) {
        return true;
    }

    return false;
}

static DOUBLE binarySearch(
    bool (*pfJudge)(DOUBLE, const PROBLEM_ONE*),
    DOUBLE dInitLower, 
    DOUBLE dInitUpper, 
    const PROBLEM_ONE *pstParam)
{
    DOUBLE dOk = dInitLower;
    DOUBLE dNg = dInitUpper;

    while (1e-10 < abs(dOk - dNg)) {
        DOUBLE dMid = (dNg + dOk) / 2LL;
//        printf("judge %f\n", dMid);
        if (pfJudge(dMid, pstParam)) {
            dOk = dMid;
        } else {
            dNg = dMid;
        }
    }
    return dOk;
}

struct POINT {
    SQWORD sqX;
    SQWORD sqY;

    POINT(SQWORD x, SQWORD y): sqX(x), sqY(y) {};
};

int main(void)
{
    SQWORD sqN = inputSQWORD();
    vector<POINT> vPnts;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vPnts.emplace_back(sqX, sqY);
    }

    PROBLEM_ONE prob;

    prob.sqN = sqN;
    prob.vvEdge.resize(sqN + 2);

    /* 
    * pnt N: start (y = -100)
    * pnt N+1: goal (y = 100)
    */
    for (SQWORD sqPntNo = 0; sqPntNo < sqN; sqPntNo++) {
        prob.vvEdge[sqN].emplace_back(sqPntNo, (DOUBLE)(100 + vPnts[sqPntNo].sqY));
        prob.vvEdge[sqPntNo].emplace_back(sqN, (DOUBLE)(100 + vPnts[sqPntNo].sqY));
        prob.vvEdge[sqN+1].emplace_back(sqPntNo, (DOUBLE)(100 - vPnts[sqPntNo].sqY));
        prob.vvEdge[sqPntNo].emplace_back(sqN+1, (DOUBLE)(100 - vPnts[sqPntNo].sqY));
    }

    for (SQWORD sqPntNo = 0; sqPntNo < sqN; sqPntNo++) {
        for (SQWORD sqPntNoTo = sqPntNo + 1; sqPntNoTo < sqN; sqPntNoTo++) {
            SQWORD sqDiffX = vPnts[sqPntNo].sqX - vPnts[sqPntNoTo].sqX;
            SQWORD sqDiffY = vPnts[sqPntNo].sqY - vPnts[sqPntNoTo].sqY; 
            DOUBLE dDist = sqrt(sqDiffX * sqDiffX + sqDiffY * sqDiffY);
            prob.vvEdge[sqPntNo].emplace_back(sqPntNoTo, dDist);
            prob.vvEdge[sqPntNoTo].emplace_back(sqPntNo, dDist);
        }
    }
    
    DOUBLE dAns = binarySearch(bPassable, 0.0, 200.0, &prob);
 
    printf("%0.11f\n", dAns / 2);

    return 0;
}
