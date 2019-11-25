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
/**
 *  mod による操作ライブラリ
 */
#define ANS_MOD (1000000007)

class MODINT {
    static SQWORD MOD;
    SQWORD m_x;

public:
    MODINT(SQWORD val) {
        m_x = (val % MOD + MOD) % MOD;
    };
    MODINT() {
        m_x = 0;
    }
    static void Init(SQWORD sqMod) {
        MOD = sqMod;
    }

	MODINT& operator+= (const MODINT a)
    {
        m_x = (m_x + a.m_x) % MOD; 
        return *this;
    };
	MODINT& operator-= (const MODINT a)
    { 
        m_x = (m_x - a.m_x + MOD) % MOD; 
        return *this;
    };
	MODINT& operator*= (const MODINT a)
    {
        m_x = (m_x * a.m_x) % MOD;
        return *this;
    };
    MODINT pow(SQWORD t) const {
        if (!t) return 1;
        MODINT a = pow(t>>1);
        a *= a;
        if (t&1) a *= *this;
        return a;
    }
	MODINT operator+ (const MODINT a) const {
		MODINT res(*this);
		return (res += a);
	}
	MODINT operator- (const MODINT a) const {
		MODINT res(*this);
		return (res -= a);
	}
	MODINT operator* (const MODINT a) const {
		MODINT res(*this);
		return (res *= a);
	}
	MODINT operator/ (const MODINT a) const {
		MODINT res(*this);
		return (res /= a);
	}

    /* 逆元 */
    MODINT inv() const {
        return pow(MOD-2);
    }

    /* 除算 */
    MODINT& operator/=(const MODINT a) {
        return (*this) *= a.inv();
    } 

    /* 整数版 */
	MODINT& operator+= (const SQWORD a) {*this += MODINT(a); return *this;};
	MODINT& operator-= (const SQWORD a) {*this -= MODINT(a); return *this;};
	MODINT& operator*= (const SQWORD a) {*this *= MODINT(a); return *this;};
	MODINT& operator/= (const SQWORD a) {*this /= MODINT(a); return *this;};

    SQWORD getVal() { return m_x; };
};
SQWORD MODINT::MOD = ANS_MOD;


/*----------------------------------------------*/

struct SEGMENT_VAL_ST {
    SQWORD sqVal;

    SEGMENT_VAL_ST (SQWORD val) {
        sqVal = val;
    }
    SEGMENT_VAL_ST () {
        sqVal = 0;
    }
    void init(void) {
        sqVal = 0;
    }
    void setVal(SQWORD val) {
        sqVal = val;
    }
    void clear() {
        sqVal = 0;
    }
    static SEGMENT_VAL_ST evaluate(
        const SEGMENT_VAL_ST &a, 
        const SEGMENT_VAL_ST &b)
    {
        SEGMENT_VAL_ST ret;
        ret.sqVal = max(a.sqVal, b.sqVal);
        return ret;
    }
    static SEGMENT_VAL_ST update(
        const SEGMENT_VAL_ST &a, 
        const SEGMENT_VAL_ST &b)
    {
        SEGMENT_VAL_ST ret;
        ret.sqVal = a.sqVal + b.sqVal;
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
            return SEGMENT_NODE_ST::evaluateFunc(stValL, stValR);
        }
    };

    /* 元配列 v をセグメント木で表現する */
    LazySegmentTree(vector<SEGMENT_NODE_ST> init) {
        initSegmentTree(init);
    }
};


/*----------------------------------------------*/
static bool IsCoverOk(
    SQWORD sqTime,
    const vector<SQWORD> &vecsqX,
    SQWORD sqN)
{
    SQWORD sqLCur = 1;
    SQWORD sqM = vecsqX.size();
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqStartPos = vecsqX.at(sqIdx);
        if (sqTime < sqStartPos - sqLCur) {
            return false;
        }
        if (sqStartPos < sqLCur) {
            sqLCur = sqStartPos + sqTime + 1;
        } else {
            SQWORD sqLCur1 = sqStartPos + max((SQWORD)0, (sqTime - 2 * (sqStartPos - sqLCur)));
            SQWORD sqLCur2 = sqStartPos + (sqTime - (sqStartPos - sqLCur)) / 2;
            sqLCur = max(sqLCur1, sqLCur2) + 1;
        }
    }

    if (sqLCur < sqN + 1) {
        return false;
    }

    return true;
}



static SQWORD binarySearch(
    bool (*pfJudge)(SQWORD, const vector<SQWORD>&, SQWORD), 
    SQWORD sqInitLower, 
    SQWORD sqInitUpper, 
    const vector<SQWORD> &vecsqX,
    SQWORD sqN)
{
    SQWORD sqOk = sqInitUpper;
    SQWORD sqNg = sqInitLower;

    while (1LL < sqOk - sqNg) {
        SQWORD sqMid = (sqNg + sqOk) / 2LL;
        if (pfJudge(sqMid, vecsqX, sqN)) {
            sqOk = sqMid;
        } else {
            sqNg = sqMid;
        }
    }
    return sqOk;
}

#define SQWORD_INF  (100100100100)

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<SQWORD> vsqX;

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        vsqX.emplace_back(sqX);
    }

    SQWORD sqAns = binarySearch(IsCoverOk, -1, SQWORD_INF, vsqX, sqN);

    printf("%lld\n", sqAns);

    return 0;
}
