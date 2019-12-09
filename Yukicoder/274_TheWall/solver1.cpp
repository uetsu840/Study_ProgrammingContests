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

struct EDGE_ST {
    SQWORD sqTo;

    EDGE_ST(SQWORD to) {
        sqTo = to;
    };
};

#define N_MAX_NODES     (100000)
#define MAX_LOG_NODES   (20)

class SCC_Graph {
    vector<vector<EDGE_ST>> vvstEdge;
    vector<vector<EDGE_ST>> vvstRevEdge;
    vector<SQWORD> vsqInOrderFwd;
    SQWORD  sqNumNode;
    vector<SQWORD>  vsqOrder;
    vector<SQWORD>  vsqComp;

    void dfs(SQWORD sqNode, vector<bool> &vbIsVisited)
    {
        if (vbIsVisited[sqNode]) {
            return;
        }
        vbIsVisited[sqNode] = true; 
        for (auto e: vvstEdge[sqNode]) {
            dfs(e.sqTo, vbIsVisited);
        }
        vsqOrder.emplace_back(sqNode);
    }

    void rdfs(SQWORD sqNode, SQWORD sqCnt, vector<vector<SQWORD>> &rnodes)
    {
        if (vsqComp[sqNode] != -1) {
            return;
        }
        vsqComp[sqNode] = sqCnt;
        rnodes[sqCnt].push_back(sqNode);
        for(auto to : vvstRevEdge[sqNode]) {
            rdfs(to.sqTo, sqCnt, rnodes);
        }
    }

public:
    SCC_Graph(SQWORD sqN) {
        sqNumNode = sqN;
        vvstEdge.resize(sqNumNode + 1, vector<EDGE_ST>{});
        vvstRevEdge.resize(sqNumNode + 1, vector<EDGE_ST>{});
        vsqComp.resize(sqNumNode + 1, -1);
    }

    void RegistEdge(SQWORD sqA, SQWORD sqB)
    {
        vvstEdge[sqA].emplace_back(sqB);
        vvstRevEdge[sqB].emplace_back(sqA);
    }
        
    SQWORD operator[](SQWORD k) {
        return vsqComp[k];
    }

    /**
     *  t:      強連結成分を、1から番号を振りなおしたグラフ 
     *  rnodes: 強連結成分からもとのグラフへの逆引きテーブル。
     * 
     *  強連結成分のグラフも、ノード番号は1はじまりとしてるので注意。
     */

    void Build(
        vector<vector<EDGE_ST>> &t,
        vector<vector<SQWORD>> &rnodes)
    {
        vector<bool>  vIsVisitedFwd(sqNumNode, false);
        vector<bool>  vIsVisitedRev(sqNumNode, false);
        rnodes.resize(sqNumNode + 1);
        t.resize(sqNumNode + 1);

        for (SQWORD sqStart = 1; sqStart < sqNumNode + 1; sqStart++) {
            dfs(sqStart, vIsVisitedFwd);
        }

        reverse(vsqOrder.begin(), vsqOrder.end());
        SQWORD ptr = 1;
        for (auto rStart: vsqOrder) {
            if (vsqComp[rStart] == -1) {
                rdfs(rStart, ptr, rnodes);
                ptr++;
            }
        }

        rnodes.resize(ptr);
        t.resize(ptr);
        for(SQWORD sqNode = 1; sqNode < sqNumNode + 1; sqNode++) {
            for(auto &to : vvstEdge[sqNode]) {
                SQWORD sqX = vsqComp[sqNode], sqY = vsqComp[to.sqTo];
                if (sqX != sqY) {
                    t[sqX].push_back(EDGE_ST{sqY});
                }
            }
        }
    }
};


/*----------------------------------------------*/

/*----------------------------------------------*/
#define N_MAX_BITS    (60)

struct COLOR_RANGE {
    SQWORD sqL;
    SQWORD sqR;
};

static bool IsWallOnNml(
    const COLOR_RANGE &c,
    SQWORD sqM,
    SQWORD sqPos)
{
//    printf("   tst [%lld %lld] %lld\n", c.sqL, c.sqR, sqPos);
    if ((c.sqL <= sqPos) && (sqPos <= c.sqR)) {
        return true;
    }
    return false;
}

static bool IsWallOnRev(
    const COLOR_RANGE &c,
    SQWORD sqM,
    SQWORD sqPos)
{
    SQWORD sqRevL = (sqM - 1) - c.sqR;
    SQWORD sqRevR = (sqM - 1) - c.sqL;
//    printf("   revtst [%lld %lld][%lld %lld] %lld\n", c.sqR, c.sqL, sqRevL, sqRevR, sqPos);
    if ((sqRevL <= sqPos) && (sqPos <= sqRevR)) {
        return true;
    }
    return false;
}


int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<COLOR_RANGE> vstColorRange;
    SQWORD sqSum = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqL = inputSQWORD();
        SQWORD sqR = inputSQWORD();
        vstColorRange.emplace_back(COLOR_RANGE{sqL, sqR});
        sqSum += (sqR - sqL + 1);
    }
    if (sqM < sqSum) {
        printf("NO\n");
        return 0;
    }

    /**
     * 2_SATのグラフを作る 
     *      Rj : j番目のブロックがそのままの場合TRUE、反転する場合FALSE
     *
     *      各列について、
     *      色がついている列の状態(反転・非反転)
     *      色がついている状態の全ての反転を論理式で並べる。
     * 
     *      グラフの頂点番号
     *      2*j  : Rj
     *      2*j+1: not Rj
     * 
     *      a , b, c のいずれか一つまたは0個が1
     *      ⇔ not(a and b) and not(b and c) and not(c and a)
     *      ⇔ (not a or not b) and (not b or not c) and (not c or not a)
     *      ⇔ (a → not b) and (b -> not a) and (b -> not c) and (c -> not b)
     */
    vector<vector<bool>> vvEdgeMatrix(sqN * 2, vector<bool>(sqN * 2, false));
    for (SQWORD sqCol = 0; sqCol < sqM; sqCol++) {
        vector<SQWORD> vsqConds;
        for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
            if (IsWallOnNml(vstColorRange[sqRow], sqM, sqCol)) {
                vsqConds.emplace_back(sqRow * 2);
            }
            if (IsWallOnRev(vstColorRange[sqRow], sqM, sqCol)) {
                vsqConds.emplace_back(sqRow * 2 + 1);
            }
        }

        SQWORD sqCondNum = vsqConds.size();
        for (SQWORD sqIdxI = 0; sqIdxI < sqCondNum - 1; sqIdxI++) {
            for (SQWORD sqIdxJ = sqIdxI + 1; sqIdxJ < sqCondNum; sqIdxJ++) {
                SQWORD sqNode1 = vsqConds[sqIdxI];
                SQWORD sqNode2 = vsqConds[sqIdxJ];
                SQWORD sqNotNode1 = (sqNode1 % 2) ? sqNode1 - 1 : sqNode1 + 1;
                SQWORD sqNotNode2 = (sqNode2 % 2) ? sqNode2 - 1 : sqNode2 + 1;

                /* p1 or p2   <========>   not p1 -> p2 , not p2 -> p1  */
//                printf("%lld:%lld\n", sqNotNode1, sqNode2);
//                printf("%lld:%lld\n", sqNotNode2, sqNode1);

//                vvEdgeMatrix[sqNotNode1][sqNode2] = true;
//                vvEdgeMatrix[sqNotNode2][sqNode1] = true;
                vvEdgeMatrix[sqNode1][sqNotNode2] = true;
                vvEdgeMatrix[sqNode2][sqNotNode1] = true;
            }
        }
    }


    SCC_Graph scc(sqN * 2);
    for (SQWORD sqIdxF = 0; sqIdxF < sqN * 2; sqIdxF++) {
        for (SQWORD sqIdxT = 0; sqIdxT < sqN * 2; sqIdxT++) {
            if (vvEdgeMatrix[sqIdxF][sqIdxT]) {
                scc.RegistEdge(sqIdxT + 1, sqIdxF + 1); /* 1 -indexed */
            }
        }
    }

    vector<vector<EDGE_ST>> vvstCompEdge;
    vector<vector<SQWORD>> vvCompNodes;
    scc.Build(vvstCompEdge, vvCompNodes);

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        if (scc[sqIdx * 2 + 1] == scc[sqIdx * 2 + 2]) {
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    
    return 0;
}