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
/*----------------------------------------------*/


/*----------------------------------------------*/

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;
    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D() : dX(0), dY(0) {};

	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    const VECTOR_2D operator- (const VECTOR_2D a) const {
		return VECTOR_2D(dX - a.dX, dY - a.dY);
    };

    DOUBLE norm(void)
    {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void)
    {
        return sqrt(norm());
    }

    DOUBLE angle(void)
    {
        return atan2(dY, dX);
    }
};


struct PROBLEM_ONE {
    SQWORD sqN;
    vector<VECTOR_2D> vecCenter;
};

const DOUBLE d_PI = 3.14159265358979;

static DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    while ((2.0 * d_PI) < dRet) {
        dRet -= (2.0 * d_PI);
    }

    return dRet;
}

/*----------------------------------------------*/

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqX;

public:
    SEGMENT_NODE_ST(SQWORD x) : sqX(x) {};
    SEGMENT_NODE_ST() : sqX(0) {};
    static SEGMENT_NODE_ST getInvalidVal()
    {
        return SEGMENT_NODE_ST{0};
    }
    static SEGMENT_NODE_ST uniteNode(SEGMENT_NODE_ST nodeA,  SEGMENT_NODE_ST nodeB)
    {
        SEGMENT_NODE_ST stRet;

        stRet.sqX = max(nodeA.sqX, nodeB.sqX);
        return stRet;
    }
    void init() {
        sqX = 0;
    }
    void debugPrint() {
        printf("<%lld> ", sqX);
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

static void execProc(SQWORD &sqX, SQWORD &sqY, char cTarget, vector<char> &vcSeq)
{
//    printf("proc %c\n", cTarget);
    sqX++;
    sqY--;
    vcSeq.emplace_back(cTarget);
}


static void execProcAny(SQWORD &sqX, SQWORD &sqY, char cX, char cY, vector<char> &vcSeq)
{
//    printf("proc any %c %c\n", cX, cY);
    if (0 == sqX) {
        sqX++;
        sqY--;
        vcSeq.emplace_back(cX);
    } else {
        sqX--;
        sqY++;
        vcSeq.emplace_back(cY);
    }
}

/**
 *  合計が2個以上のとき
 */
static bool solveMany(
    vector<string> vstrProc,
    SQWORD sqN, 
    SQWORD sqA,
    SQWORD sqB,
    SQWORD sqC,
    vector<char> &vcSeq)
{
    for (SQWORD sqProcIdx = 0; sqProcIdx < vstrProc.size(); sqProcIdx++) {
        string proc = vstrProc.at(sqProcIdx);
        string nextProc = "";
        if (sqProcIdx + 1 < vstrProc.size()) {
            nextProc = vstrProc.at(sqProcIdx + 1);
        }
        if ("AB" == proc) {
            if (0 == sqA + sqB) {
                return false;
            }
            if (("AC" == nextProc) && (0 < sqB)) {
                execProc(sqA, sqB, 'A', vcSeq);
            } else if (("BC" == nextProc) && (0 < sqA)) {
                execProc(sqB, sqA, 'B', vcSeq);
            } else {
                execProcAny(sqA, sqB, 'A', 'B', vcSeq);
            }
        } else if ("AC" == proc) {
            if (0 == sqA + sqC) {
                return false;
            }
            if (("AB" == nextProc) && (0 < sqC)){
                execProc(sqA, sqC, 'A', vcSeq);
            } else if (("BC" == nextProc) && (0 < sqA)) {
                execProc(sqC, sqA, 'C', vcSeq);
            } else {
                execProcAny(sqA, sqC, 'A', 'C', vcSeq);
            }
        } else if ("BC" == proc) {
            if (0 == sqB + sqC) {
                return false;
            }
            if (("AB" == nextProc) && (0 < sqC)) {
                execProc(sqB, sqC, 'B', vcSeq);
            } else if (("AC" == nextProc) && (0 < sqB)) {
                execProc(sqC, sqB, 'C', vcSeq);
            } else {
                execProcAny(sqB, sqC, 'B', 'C', vcSeq);
            }
        } else {
            exit(-1);
        }
    }
    return true;
}

/**
 *  合計が1個のとき
 */
static bool solveSingle(
    vector<string> vstrProc,
    SQWORD sqN,
    SQWORD sqA,
    SQWORD sqB,
    SQWORD sqC,
    vector<char> &vcSeq)
{
    for (auto proc: vstrProc) {
        if ("AB" == proc) {
            if (0 == sqA + sqB) {
                return false;
            }
            if (0 == sqA) {
                execProc(sqA, sqB, 'A', vcSeq);
            } else if (0 == sqB) {
                execProc(sqB, sqA, 'B', vcSeq);
            } else {
                return false;
            }
        } else if ("AC" == proc) {
            if (0 == sqA + sqC) {
                return false;
            }
            if (0 == sqA) {
                execProc(sqA, sqC, 'A', vcSeq);
            } else if (0 == sqC) {
                execProc(sqC, sqA, 'C', vcSeq);
            } else {
                return false;
            }
        } else if ("BC" == proc) {
            if (0 == sqB + sqC) {
                return false;
            }
            if (0 == sqB) {
                execProc(sqB, sqC, 'B', vcSeq);
            } else if (0 == sqC) {
                execProc(sqC, sqB, 'C', vcSeq);
            } else {
                return false;
            }
        } else {
            exit(-1);
        }
    }
    return true;
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqA = inputSQWORD();
    SQWORD sqB = inputSQWORD();
    SQWORD sqC = inputSQWORD();
    bool bAns = false;

    vector<string> vstrProc;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        string strS;
        cin >> strS;
        vstrProc.emplace_back(strS);
    }

    vector<char> vcSeq;
    if (sqA + sqB + sqC < 2) {
        bAns = solveSingle(vstrProc, sqN, sqA, sqB, sqC, vcSeq);
    } else {
        bAns = solveMany(vstrProc, sqN, sqA, sqB, sqC, vcSeq);
    }

    if (bAns) {
        printf("Yes\n");
        for (auto s: vcSeq) {
            printf("%c\n", s);
        }
    } else {
        printf("No\n");
    }

    return 0;
}