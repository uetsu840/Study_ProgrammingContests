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

#define SQWORD_INF  (100100100100100100)

/**
 *  遅延評価セグメント木
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
            return max((SQWORD)0, stN.sqLazy + stN.sqVal);
        } else {
            return stN.sqVal;          
        }
    }

    static inline void refreshLazyVal(
        SEGMENT_NODE_ST &stN)
    {
        stN.sqVal += stN.sqLazy;
        stN.bLazyFlag = false;
        stN.sqLazy    = 0;
    }

    static inline void updateLazyVal(
        SEGMENT_NODE_ST &stN,
        SQWORD sqX)
    {
        stN.sqLazy += sqX;
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
        SQWORD sqX,      /* update value */
        SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return;
        }

        SQWORD sqNodeVal = getNodeVal(vNode[lNodeIdx]);

        if (lLeft + 1 == lRight) {
            /* 末端ノード */
            updateLazyVal(vNode[lNodeIdx], max(sqX, -sqNodeVal));
        } else if (((lA <= lLeft) && (lRight <= lB)) && (0 <= sqNodeVal + sqX)) {
            updateLazyVal(vNode[lNodeIdx], max(sqX, -sqNodeVal));
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);

            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;

            SQWORD sqNextLTmp = getNodeVal(vNode[lNodeIdx_L]) + sqX;
            SQWORD sqUpdateL, sqUpdateR;
            if (lCenter <= lA) {
                sqUpdateL = 0;
                sqUpdateR = sqX;
            } else if (sqNextLTmp < 0) {
                sqUpdateL = sqX;
                sqUpdateR = -(getNodeVal(vNode[lNodeIdx_L]));
            } else {
                sqUpdateL = sqX;
                sqUpdateR = sqX;
            }
            UpdateValue(lA, lB, sqUpdateL, lNodeIdx_L, lLeft, lCenter);
            UpdateValue(lA, lB, sqUpdateR, lNodeIdx_R, lCenter, lRight);
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

    SQWORD GetValue(
        SDWORD lA,
        SDWORD lNodeIdx = 0, SDWORD lLeft=0, SDWORD lRight=-1)    /* 自分のノードが対象とする区間 */
    {
        if (lRight < 0) {
            lRight = dwBaseSize;
        }

        if (lLeft + 1 == lRight) {
            return getNodeVal(vNode[lNodeIdx]);
        } else {
            lazyEvaluate(lNodeIdx, lLeft, lRight);
            SDWORD lCenter = (lLeft + lRight) / 2;
            SDWORD lNodeIdx_L = (lNodeIdx * 2) + 1; 
            SDWORD lNodeIdx_R = (lNodeIdx * 2) + 2;
            SQWORD sqRet = 0;
            if (lA < lCenter) {
                sqRet = GetValue(lA, lNodeIdx_L, lLeft, lCenter);
            } else {
                sqRet = GetValue(lA, lNodeIdx_R, lCenter, lRight);
            }
            return sqRet;
        }
    };

    /* 元配列 v をセグメント木で表現する */
    LazySegmentTree(vector<SQWORD> init) {
        initSegmentTree(init);
    }
};

/*----------------------------------------------*/

struct ORDER_QUERY {
    SQWORD sqT;
    SQWORD sqD;
    SQWORD sqA;

    ORDER_QUERY(SQWORD t, SQWORD d, SQWORD a) : sqT(t), sqD(d), sqA(a) {};
};

int main(void)
{
    SQWORD sqQ = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    vector<ORDER_QUERY> vecQ;
    map<SQWORD, SQWORD> mapOrderToIdx;
    set<SQWORD> setOrderD;

    SQWORD sqOrderIdx = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqType = inputSQWORD();

        SQWORD sqA = 0;
        SQWORD sqD = 0;

        if (1 == sqType) {
            /* order */
            sqD = inputSQWORD();
            sqA = inputSQWORD();

            setOrderD.insert(sqD);
        } else {
            /* Query */
            sqD = inputSQWORD();
        }

        vecQ.emplace_back(sqType, sqD, sqA);
    }

    /* initialize lazy segment tree */
    vector<SQWORD> vecSegInit;

    for (auto d: setOrderD) {
        vecSegInit.emplace_back(d * sqK);

        mapOrderToIdx[d] = sqOrderIdx;
        sqOrderIdx++;
    }

    LazySegmentTree seg(vecSegInit);
    SQWORD sqSegTreeEnd = vecSegInit.size();

    for (auto query: vecQ) {
        if (1 == query.sqT) {
            /* order */
            SQWORD sqIdx = mapOrderToIdx[query.sqD];
            seg.UpdateValue(sqIdx, sqSegTreeEnd, -(query.sqA));  
        } else {
            /* Query */
            if (query.sqD < *(setOrderD.begin())) {
                printf("0\n");
            } else {
                auto it = setOrderD.upper_bound(query.sqD);
                it--;
                SQWORD sqIndex = mapOrderToIdx[*it];
                SQWORD sqGoodsCnt = seg.GetValue(sqIndex);
                SQWORD sqAns = sqK * (*it) - sqGoodsCnt;
                printf("%lld\n", sqAns);
            }
        }
    }
    return 0;
}