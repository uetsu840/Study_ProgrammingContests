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


/**
 *  セグメント木
 */
struct SEGMENT_NODE_MAX_ST {
    SQWORD sqVal;

public:
    SEGMENT_NODE_MAX_ST(SQWORD a) : sqVal(a) {};
    SEGMENT_NODE_MAX_ST() : sqVal(0) {};
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

struct SEGMENT_NODE_MIN_ST {
    SQWORD sqVal;

public:
    SEGMENT_NODE_MIN_ST(SQWORD a) : sqVal(a) {};
    SEGMENT_NODE_MIN_ST() : sqVal(0) {};

    static SQWORD getInvalidVal()
    {
        return MAX_SQWORD;
    }
    static SQWORD uniteVal(SQWORD sqValA, SQWORD sqValB)
    {
        return min(sqValA, sqValB);
    }
    void init() {
        sqVal = getInvalidVal();
    }
    void debugPrint() {
        printf("%lld ", sqVal);
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
        
        stRet.sqVal = T::uniteVal(stR.sqVal, stL.sqVal);
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

    SQWORD GetValue(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return T::getInvalidVal();
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx].sqVal;
        }

        SQWORD sqResL, sqResR;
        SQWORD lCenter = (lLeft + lRight) / 2;
        sqResL = GetValue(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter);
        sqResR = GetValue(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight);
        return T::uniteVal(sqResL, sqResR);
    }
};

/*----------------------------------------------*/


int main(void)
{
    vector<SEGMENT_NODE_MIN_ST> vstSegNodeMin;
    vector<SEGMENT_NODE_MAX_ST> vstSegNodeMax;

    SQWORD sqN = inputSQWORD();
    SQWORD sqK = inputSQWORD();
    vector<SQWORD> vecsqA;
    vector<bool> vecIsSorted(sqN, false);
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        vstSegNodeMin.emplace_back(sqA);
        vstSegNodeMax.emplace_back(sqA);
        vecsqA.emplace_back(sqA);
    }

    SQWORD sqContCnt = 1;
    for (SQWORD sqIdx = 1; sqIdx < sqN; sqIdx++) {
        bool bIsSorted = false;
        if (vecsqA[sqIdx - 1] < vecsqA[sqIdx]) {
            sqContCnt++;
            if (sqK <= sqContCnt) {
                vecIsSorted[sqIdx - (sqK-1)] = true;
//                printf("sorted %lld\n", sqIdx - (sqK-1));
            }
        } else {
            sqContCnt = 1;
        }
    }

    SegmentTree<SEGMENT_NODE_MIN_ST> minSegmentTree(vstSegNodeMin);
    SegmentTree<SEGMENT_NODE_MAX_ST> maxSegmentTree(vstSegNodeMax);

    SQWORD sqAns = 1;
    bool bSortedPattern = false;
    if (vecIsSorted[0]) {
        bSortedPattern = true;
    }

    for (SQWORD sqIdx = 0; sqIdx < sqN - sqK; sqIdx++) {
//        printf("%lld\n", sqIdx);
        SQWORD sqMin = minSegmentTree.GetValue(sqIdx, sqIdx + sqK);
        SQWORD sqMax = maxSegmentTree.GetValue(sqIdx + 1, sqIdx + 1 + sqK);
        bool bIsSame = false;
        if ((sqMin == vecsqA[sqIdx]) && (sqMax == vecsqA[sqIdx + sqK])) {
            bIsSame = true;
        }

        if (vecIsSorted[sqIdx + 1]) {
            if (!bSortedPattern) {
                sqAns++;
                bSortedPattern = true;
            }     
        } else {
            if (!bIsSame) {
                sqAns++;
            }
        }
    }
    printf("%lld\n", sqAns);



    return 0;
}