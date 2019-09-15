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

struct ST_EDGE {
    SDWORD lTo;
    SQWORD sqCap;
    SQWORD sqMaxCap;
    SQWORD sqCost;
    SDWORD lRev;
    bool bIsFwd;

    ST_EDGE(SDWORD to, SQWORD cap, SQWORD cost, SDWORD rev, bool bFwd)
    {
        lTo = to;
        sqCap = cap;
        sqMaxCap = cap;
        sqCost = cost;
        lRev = rev;
        bIsFwd = bFwd;
    };
};

#define MAX_V   (20002)

struct Flow {
private:
    vector<ST_EDGE> avGraph[MAX_V];
    SQWORD  sqEdgeNum;
    SQWORD  asqDist[MAX_V];
    SDWORD  alPrevV[MAX_V];   
    SDWORD  alPrevEdge[MAX_V];      /*直前の頂点と辺 */

    SDWORD  alDinic_Level[MAX_V];
    SDWORD  alDinic_Iter[MAX_V];

public:
    void addEdge(SDWORD lFrom, SDWORD lTo, SQWORD sqCap, SQWORD sqCost) {
//        printf("%d ----[%lld] ----> %d\n", lFrom, sqCap, lTo);

        avGraph[lFrom].emplace_back(lTo, sqCap, sqCost, (SDWORD)(avGraph[lTo].size()), true);
        avGraph[lTo].emplace_back(lFrom, 0, -sqCost, (SDWORD)(avGraph[lFrom].size() - 1), false);
    }

    SDWORD minCostFlow(SDWORD lStart, SDWORD lTarget, SQWORD sqFlow)
    {
        SQWORD sqRes = 0;
        SQWORD sqFlowRest = sqFlow;
        while (0 < sqFlowRest) {
            /* Bellman-Ford Method */
            fill(asqDist, asqDist + ArrayLength(asqDist), MAX_SDWORD);
            asqDist[lStart] = 0;
            bool bIsUpdate = true;
            while (bIsUpdate) {
                bIsUpdate = false;
                for (SDWORD lV = 0; lV < MAX_V; lV++) {
                    for (SDWORD lIdx = 0; lIdx < avGraph[lV].size(); lIdx++) {
                        ST_EDGE &stE = avGraph[lV][lIdx];
                        if ((asqDist[lV] != MAX_SDWORD)
                                && (0 < stE.sqCap)
                                && (asqDist[lV] + stE.sqCost < asqDist[stE.lTo])) {
                            asqDist[stE.lTo] = asqDist[lV] + stE.sqCost;
                            alPrevV[stE.lTo] = lV;
                            alPrevEdge[stE.lTo] = lIdx;
                            bIsUpdate = true;
                        }
                    }
                }
            }
            if (asqDist[lTarget] == MAX_SDWORD) {
                return -1;
            }

            SQWORD sqAddFlow = MAX_SQWORD;
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                sqAddFlow = min(sqAddFlow, avGraph[alPrevV[lV]][alPrevEdge[lV]].sqCap);
            }
            sqFlowRest -= sqAddFlow;
            sqRes += sqAddFlow * asqDist[lTarget];      // 後で目的変数は書き換える
            for (SDWORD lV = lTarget; lV != lStart; lV = alPrevV[lV]) {
                ST_EDGE &stE = avGraph[alPrevV[lV]][alPrevEdge[lV]];
                stE.sqCap -= sqAddFlow;
                avGraph[lV][stE.lRev].sqCap += sqAddFlow;
            }
        }
        return sqRes;
    }

private:
    SQWORD maxFlowDinicBfs(SDWORD lS)
    {
        memset(alDinic_Level, -1, sizeof(alDinic_Level));
        queue<SDWORD> queSearch;
        alDinic_Level[lS] = 0;
        queSearch.push(lS);
        while (!queSearch.empty()) {
            SDWORD lV = queSearch.front();
            queSearch.pop();
            for (SDWORD lIdx = 0; lIdx < avGraph[lV].size(); lIdx++) {
                ST_EDGE &stE = avGraph[lV][lIdx];
                if ((0 < stE.sqCap) && (alDinic_Level[stE.lTo] < 0)) {
                    alDinic_Level[stE.lTo] = alDinic_Level[lV] + 1;
                    queSearch.push(stE.lTo);
                }
            }
        }

        return 0;
    }

    SQWORD maxFlowDinicDfs(SDWORD lV, SDWORD lT, SQWORD sqF)
    {
        if (lV == lT) {
            return sqF;
        }
        for (SDWORD &lI = alDinic_Iter[lV]; lI < avGraph[lV].size(); lI++) {
            ST_EDGE &stE = avGraph[lV][lI];
            if ((0 < stE.sqCap) && (alDinic_Level[lV] < alDinic_Level[stE.lTo])) {
                SQWORD sqDiffFlow = maxFlowDinicDfs(stE.lTo, lT, min(sqF, stE.sqCap));
                if (0 < sqDiffFlow) {
                    stE.sqCap -= sqDiffFlow;
                    avGraph[stE.lTo][stE.lRev].sqCap += sqDiffFlow;
                    return sqDiffFlow;
                }
            }
        }
        return 0;
    }

public:
    SQWORD MaxFlow(SDWORD lStart, SDWORD lTarget)
    {
        SQWORD sqFlow = 0LL;
        for (;;) {
            maxFlowDinicBfs(lStart);
            if (alDinic_Level[lTarget] < 0) {
                return sqFlow;
            }
            memset(alDinic_Iter, 0, sizeof(alDinic_Iter));
            while (1) {
                SQWORD sqDiffFlow = maxFlowDinicDfs(lStart, lTarget, MAX_SQWORD);
                sqFlow += sqDiffFlow;
                if (0LL == sqDiffFlow) {
                    break;
                }
            }
        }
    }

    SQWORD GetNodeFlow(SDWORD lNode)
    {
        SQWORD sqFlow = 0;
        for (auto e: avGraph[lNode]) {
            if (e.bIsFwd) {
                sqFlow += (e.sqMaxCap - e.sqCap);
            }
        }
        return sqFlow;
    }
};

/*----------------------------------------------*/

#define PNT_NO_START    (10200)
#define PNT_NO_TARGET   (10201)
#define PNT_NO_OFS_ROW  (10000)
#define PNT_NO_OFS_COL  (10100)
#define MAX_N           (100)

int main(void)
{
    SQWORD sqN = inputSQWORD();

    Flow solverFlow;

    SQWORD s_aasqW[MAX_N][MAX_N];
    SQWORD s_aasqE[MAX_N][MAX_N];
    bool s_aabOrgState[MAX_N][MAX_N];

    for (SQWORD sqRowIdx = 0; sqRowIdx < sqN; sqRowIdx++) {
        for (SQWORD sqColIdx = 0; sqColIdx < sqN; sqColIdx++) {
            SQWORD sqW = inputSQWORD();
            s_aasqW[sqRowIdx][sqColIdx] = sqW;
        }
    }
    for (SQWORD sqRowIdx = 0; sqRowIdx < sqN; sqRowIdx++) {
        for (SQWORD sqColIdx = 0; sqColIdx < sqN; sqColIdx++) {
            SQWORD sqE = inputSQWORD();
            s_aasqE[sqRowIdx][sqColIdx] = sqE;
        }
    }

    /* 
    *   (N) ----->  10000 <row0> -----> [0, 0] ---->    10100 <col0>     -----> (N)
    *               10001 <row1>        [0, 1]          10101 <col1>
    *               10002 <row2>        [0, 2]          10102 <col2>
    *               10003 <row3>        [0, 3]          10103 <col3>
    *                                   [1, 0]
    *                                   [1, 1]
    */

    for (SQWORD sqRowIdx = 0; sqRowIdx < sqN; sqRowIdx++) {
        string str;
        cin >> str;
        for (SQWORD sqRestCol = 0; sqRestCol < sqN; sqRestCol++) {
            if ('o' == str[sqRestCol]) {
                s_aabOrgState[sqRowIdx][sqRestCol] = true;
            } else {
                s_aabOrgState[sqRowIdx][sqRestCol] = false;
            }

            /* col番目だけを o にするコストを求める */
            SQWORD sqCost = 0;
            for (SQWORD sqColCur = 0; sqColCur < sqN; sqColCur++) {
                if (sqRestCol == sqColCur) {
                    if ('.' == str[sqColCur]) {
                        sqCost += s_aasqW[sqRowIdx][sqColCur];
                    }
                } else {
                    if ('o' == str[sqColCur]) {
                        sqCost += s_aasqE[sqRowIdx][sqColCur];
                    }
                }
            }
            SQWORD sqPntNoCell = sqRowIdx * sqN + sqRestCol;
            solverFlow.addEdge(PNT_NO_OFS_ROW + sqRowIdx, sqPntNoCell,                1, sqCost);
            solverFlow.addEdge(sqPntNoCell,               PNT_NO_OFS_COL + sqRestCol, 1, 0);
        }
    }
    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        solverFlow.addEdge(PNT_NO_START, PNT_NO_OFS_ROW + sqRow, 1, 0);
    }
    for (SQWORD sqCol = 0; sqCol < sqN; sqCol++) {
        solverFlow.addEdge(PNT_NO_OFS_COL + sqCol, PNT_NO_TARGET, 1, 0);
    }

    SQWORD sqMinCost = solverFlow.minCostFlow(PNT_NO_START, PNT_NO_TARGET, sqN);

    /* minimum cost */
    printf("%lld\n", sqMinCost);

    vector<pair<SQWORD, SQWORD>> vecErasePair;
    vector<pair<SQWORD, SQWORD>> vecWritePair;

    for (SQWORD sqRowIdx = 0; sqRowIdx < sqN; sqRowIdx++) {
        for (SQWORD sqColIdx = 0; sqColIdx < sqN; sqColIdx++) {
            SQWORD sqPntNoCell = sqRowIdx * sqN + sqColIdx;
            SQWORD sqNodeFlow = solverFlow.GetNodeFlow(sqPntNoCell);

            if (0 == sqNodeFlow) {
                if (s_aabOrgState[sqRowIdx][sqColIdx]) {
                    vecErasePair.emplace_back(make_pair(sqRowIdx + 1, sqColIdx + 1));
                }
            } else {
                if (!(s_aabOrgState[sqRowIdx][sqColIdx])) {
                    vecWritePair.emplace_back(make_pair(sqRowIdx + 1, sqColIdx + 1));
                }
            }
        }
    }
    printf("%lld\n", vecErasePair.size() + vecWritePair.size());
    for (auto e: vecErasePair) {
        printf("%lld %lld erase\n", e.first, e.second);
    }
    for (auto w: vecWritePair) {
        printf("%lld %lld write\n", w.first, w.second);
    }

    return 0;
}