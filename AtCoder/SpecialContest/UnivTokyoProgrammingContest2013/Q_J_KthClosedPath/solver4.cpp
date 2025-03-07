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

#define N_MAX_VERTICE   (25000)

struct PATH_INFO {
    map<SQWORD, SQWORD> mapDist;
    PATH_INFO (SQWORD sqVertice, SQWORD sqCnt) {
        mapDist[sqVertice] = sqCnt;
    }
    PATH_INFO () {
        ;
    }
};

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqDist;

    EDGE_ST (SQWORD to, SQWORD dist) : sqTo(to), sqDist(dist) {};
};

#define SQWORD_INF  (100100100100100100)

struct DIST_QUE_ENTRY {
    SQWORD sqCostStar;
    SQWORD sqCost;
    SQWORD sqPos;
};

bool operator> (const DIST_QUE_ENTRY &a, const DIST_QUE_ENTRY &b)
{
    return a.sqCostStar > b.sqCostStar;
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    vector<vector<EDGE_ST>> vvEdge(sqN);
    vector<vector<EDGE_ST>> vvRevEdge(sqN);
    map<SQWORD, PATH_INFO> mapPath;

    for (SQWORD sqEdgeIdx = 0; sqEdgeIdx < sqM; sqEdgeIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();
        SQWORD sqC = inputSQWORD();

        vvEdge[sqU].emplace_back(sqV, sqC);
        vvRevEdge[sqV].emplace_back(sqU, sqC);
    }

    typedef pair<SQWORD, SQWORD> P;
 
    /* get rev edge cost */
    vector<SQWORD> vRevcost(sqN, SQWORD_INF);
    {
        priority_queue<P, vector<P>, greater<P>> queRev;  
        vRevcost[0] = 0;
        queRev.push(make_pair(0, 0));
        while (!queRev.empty()) {
            P p = queRev.top();
            queRev.pop();

            SDWORD v = p.second;
            if (p.first <= vRevcost[v]) {
                for (SDWORD lIdx = 0; lIdx < vvRevEdge[v].size(); lIdx++) {
                    EDGE_ST e = vvRevEdge[v][lIdx];

                    if (vRevcost[e.sqTo] > vRevcost[v] + e.sqDist) {
                        vRevcost[e.sqTo] = vRevcost[v] + e.sqDist;
                        queRev.push(P(vRevcost[e.sqTo], e.sqTo));
                    }
                }
            }
        }
    }

    bool bAnsExist = false;
    for (auto n: vvEdge[0]) {
        if (SQWORD_INF != vRevcost[n.sqTo]) {
            bAnsExist = true;
        }
    }
    if (!bAnsExist) {
        for (SQWORD sqIdx = 0; sqIdx < sqK; sqIdx++) {
            printf("-1\n");
        }
        return 0;
    }

#if 0
    for (auto d: vRevcost) {
        printf("%lld ", d);
    }
    printf("\n");
#endif

    SQWORD sqCnt = 0;
    {
        priority_queue<DIST_QUE_ENTRY, vector<DIST_QUE_ENTRY>, greater<DIST_QUE_ENTRY>> que;  
        que.push(DIST_QUE_ENTRY{vRevcost[0], 0, 0});
        while (1) {
            DIST_QUE_ENTRY p = que.top();
            que.pop();

            SDWORD v = p.sqPos;
//            printf("==%lld %lld\n", v, p.sqCost);
            for (SDWORD lIdx = 0; lIdx < vvEdge[v].size(); lIdx++) {
                EDGE_ST e = vvEdge[v][lIdx];

                SQWORD sqNextCost = p.sqCost + e.sqDist;

                if (0 == e.sqTo) {
                    printf("%lld\n", sqNextCost);
                    sqCnt++;
                }
                if (sqK <= sqCnt) {
                    return 0;
                }

//                printf("%lld %lld %lld\n", sqNextCost + vRevcost[e.sqTo], sqNextCost, e.sqTo);
                que.push(DIST_QUE_ENTRY{sqNextCost + vRevcost[e.sqTo], sqNextCost, e.sqTo});
            }
        }
    }

    return 0;
}
