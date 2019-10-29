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

static void calcPrimeFactorication(
    SQWORD sqNum, 
    vector<pair<SQWORD, SQWORD>> &vlPrimes)
{
    if (1 == sqNum) {
        return;
    }
    if (2 == sqNum) {
        vlPrimes.emplace_back(make_pair(2, 1));
        return;
    }
    if (3 == sqNum) {
        vlPrimes.emplace_back(make_pair(3, 1));
        return;
    }

    SQWORD sqCur = sqNum;
    SQWORD sqUpper = sqrt(sqNum) + 1;
    for (SQWORD sqDiv = 2; sqDiv <= sqUpper; sqDiv++) {
        SDWORD lPowerCnt = 0;
        while(0 == sqCur % sqDiv) {
            sqCur /= sqDiv;
            lPowerCnt++;
        }
        if (0 < lPowerCnt) {
            vlPrimes.emplace_back(make_pair(sqDiv, lPowerCnt));
        }
        if (1 == sqCur) {
            break;
        }
    }
    if (1 < sqCur) {
        vlPrimes.emplace_back(make_pair(sqCur, 1));
    }
}

/*-----------------------------------------------------*/

class TREE {
public: 
    TREE(SQWORD sqNodeNum)
    {
        m_vvEdge.resize(sqNodeNum + 1);
    }
private:
    vector<vector<SQWORD>> m_vvEdge;
    /**
     *  target までのパスを探す
     */
    bool searchPathExec(
        SQWORD sqParent, 
        SQWORD sqCur,
        SQWORD sqTarget,
        vector<SQWORD> &vecsqPath) 
    {
        if (sqTarget == sqCur) {
            vecsqPath.emplace_back(sqCur);
            return true;
        }

        for (auto next: m_vvEdge[sqCur]) {
            if (sqParent != next) {
                if (searchPathExec(sqCur, next, sqTarget, vecsqPath)) {
                    vecsqPath.emplace_back(sqCur);
                    return true;
                }
            }
        }

        return false;
    }

    /**
     *  ノードの数を数える
     */
    SQWORD countNodeExec(
        SQWORD sqParent, 
        SQWORD sqCur)
    {
        SQWORD sqCnt = 1;
        for (auto next: m_vvEdge[sqCur]) {
            if (sqParent != next) {
                sqCnt += countNodeExec(sqCur, next);
            }
        }
        return sqCnt;
    }
public:
    void AddEdge(SQWORD sqA, SQWORD sqB)
    {
        m_vvEdge[sqA].emplace_back(sqB);
        m_vvEdge[sqB].emplace_back(sqA);
    }

    /* 
    * 辺を消す。エッジは隣接していること。遅いので注意。
    * また、当たり前だけど消した後は木じゃなくなる。
    */
    void EraseEdge(SQWORD sqA, SQWORD sqB)
    {
        m_vvEdge[sqA].erase(std::find(m_vvEdge[sqA].begin(), m_vvEdge[sqA].end(), sqB));
        m_vvEdge[sqB].erase(std::find(m_vvEdge[sqB].begin(), m_vvEdge[sqB].end(), sqA));

    }

    void SearchPath(SQWORD sqFrom, SQWORD sqTo, vector<SQWORD> &vecsqPath)
    {
        searchPathExec(-1, sqFrom, sqTo, vecsqPath);
    }

    SQWORD CountNode(SQWORD sqRoot)
    {
        return countNodeExec(-1, sqRoot);
    }
};


int main(void)
{
    SQWORD sqN = inputSQWORD();

    TREE tree(sqN);

    for (SQWORD sqIdx = 0; sqIdx < sqN - 1; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();
        tree.AddEdge(sqA, sqB);
    }

    /* node1 から nodeN を探す */
    vector<SQWORD> vecsqPath;
    tree.SearchPath(1, sqN, vecsqPath);
    reverse(vecsqPath.begin(), vecsqPath.end());

    /* 黒と白の境界を探し、間の辺を取り除く */
    SQWORD sqMidF = *(vecsqPath.begin() + ((vecsqPath.size() - 1) / 2));
    SQWORD sqMidS = *(vecsqPath.begin() + ((vecsqPath.size() + 1) / 2));

    tree.EraseEdge(sqMidF, sqMidS);

    /* 2個に分かれた木の、それぞれの頂点数を数える */
    SQWORD sqScoreFenec, sqScoreSnuke;
    sqScoreFenec = tree.CountNode(sqMidF);
    sqScoreSnuke = tree.CountNode(sqMidS);

    if (sqScoreFenec > sqScoreSnuke) {
        printf("Fennec\n");   
    } else {
        printf("Snuke\n");
    }

    return 0;
}