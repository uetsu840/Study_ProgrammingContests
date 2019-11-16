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

    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqType = inputSQWORD();

        SQWORD sqA = 0;
        SQWORD sqD = 0;

        if (1 == sqType) {
            /* order */
            sqD = inputSQWORD();
            sqA = inputSQWORD();

        } else {
            /* Query */
            sqD = inputSQWORD();
        }

        vecQ.emplace_back(sqType, sqD, sqA);
        setOrderD.insert(sqD);
    }

    /* initialize lazy segment tree */
    vector<SEGMENT_NODE_ST> vecSegInit;
    SQWORD sqOrderIdx = 0;
    SQWORD sqDPrev = 0;
    for (auto d: setOrderD) {
        vecSegInit.emplace_back((d - sqDPrev) * sqK, (d - sqDPrev) * sqK);
        sqDPrev = d;

        mapOrderToIdx[d] = sqOrderIdx;
        sqOrderIdx++;
    }

    SegmentTree<SEGMENT_NODE_ST> seg(vecSegInit);

    for (auto query: vecQ) {
        if (1 == query.sqT) {
            /* order */
            SQWORD sqIdx = mapOrderToIdx[query.sqD];
            SEGMENT_NODE_ST stNode = seg.getNode(sqIdx);
            stNode.sqX -= query.sqA;
            stNode.sqY = max((SQWORD)0, stNode.sqY - query.sqA);
            seg.update(sqIdx, stNode);  
        } else {
            SQWORD sqIndex = mapOrderToIdx[query.sqD];
            SEGMENT_NODE_ST stNode = seg.GetValue(0, sqIndex + 1);
//            printf("%lld [%lld %lld]> \n", sqIndex+1, query.sqD, sqIndex);
            SQWORD sqRest = max(stNode.sqX, stNode.sqY);
            printf("%lld\n", sqK * query.sqD - sqRest);
        }
    }
    return 0;
}