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

    void rdfs(SQWORD sqNode, SQWORD sqCnt)
    {
        if (vsqComp[sqNode] != -1) {
            return;
        }
        vsqComp[sqNode] = sqCnt;
        for(auto to : vvstRevEdge[sqNode]) {
            rdfs(to.sqTo, sqCnt);
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

    void Build(vector<vector<EDGE_ST>> t) {
        vector<bool>  vIsVisitedFwd(sqNumNode + 1, false);
        vector<bool>  vIsVisitedRev(sqNumNode + 1, false);

        for (SQWORD sqStart = 1; sqStart < sqNumNode + 1; sqStart++) {
            dfs(sqStart, vIsVisitedFwd);
        }

        reverse(vsqOrder.begin(), vsqOrder.end());
        SQWORD ptr = 0;
        for (auto rStart: vsqOrder) {
            if (vsqComp[rStart] == -1) {
                rdfs(rStart, ptr);
                ptr++;
            }
        }

        t.resize(ptr);
        for(SQWORD sqNode = 1; sqNode < sqNumNode; sqNode++) {
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
#define N_MAX_BITS    (60)

static bool s_bAnsFound = false;
vector<SQWORD> vsqLoop;

static void FindLoop(
    SQWORD sqNode,
    const vector<vector<SQWORD>> &vvstEdge,
    vector<SQWORD> &vsqCurPath,
    vector<SQWORD> &vsqVisitedDepth)
{
//    printf("[%lld] %lld\n", vsqCurPath.size(), sqNode);
    vsqVisitedDepth[sqNode] = vsqCurPath.size();

    for (SQWORD sqSrchDepth = vsqCurPath.size() - 1; 0 <= sqSrchDepth; sqSrchDepth--) {
        for (auto e: vvstEdge[sqNode]) {
            if (e == vsqCurPath[sqSrchDepth]) {
//                printf("---> %lld\n", e);
                /* found */
                for (SQWORD sqAnsIdx = sqSrchDepth; sqAnsIdx < vsqCurPath.size(); ++sqAnsIdx) {
                    vsqLoop.emplace_back(vsqCurPath[sqAnsIdx]);
                }
                s_bAnsFound = true;
                return;
            }
        }
    }


    /* 訪問済みの頂点につながってない */
    for (auto e: vvstEdge[sqNode]) {
        vsqCurPath.emplace_back(e);
        FindLoop(e, vvstEdge, vsqCurPath, vsqVisitedDepth);
        vsqCurPath.pop_back();
        if (s_bAnsFound) {
            break;
        }
    }
}

static void RemoveDuplicateEdge(
    const vector<SQWORD> &vsqLoop,
    const vector<vector<SQWORD>> &vvstEdge,
    vector<SQWORD> &vsqShortest)
{
//    printf("remove duplicate size[%lld]\n", vsqLoop.size());
    vector<SQWORD> vsqRemoveIdx;
    for (SQWORD sqStartIdx = 0; sqStartIdx < vsqLoop.size(); sqStartIdx++) {
        SQWORD sqRemoveOffset = 0;
        for (SQWORD sqOffset = 2; sqOffset < vsqLoop.size(); sqOffset++) {
            SQWORD sqEndIdx = (sqStartIdx + sqOffset) % vsqLoop.size();
            bool bFound = false;
//            printf("----- start %lld siz(%lld)\n", vsqLoop[sqStartIdx], vvstEdge[vsqLoop[sqStartIdx]].size());
            for (auto e: vvstEdge[vsqLoop[sqStartIdx]]) {
//                printf("<< %lld %lld>>\n", e, vsqLoop[sqEndIdx]);
                if (e == vsqLoop[sqEndIdx]) {
                    bFound = true;
                }
            }
            if (bFound) {
//                printf(">>> %lld\n", vsqLoop[sqEndIdx]);
                sqRemoveOffset = sqOffset;
            }
        }
        for (SQWORD sqRemovePtr = 1; sqRemovePtr < sqRemoveOffset; ++sqRemovePtr) {
            SQWORD sqRemoveIdx = (sqStartIdx + sqRemovePtr) % vsqLoop.size();
            vsqRemoveIdx.emplace_back(sqRemoveIdx);
        }
        /* 飛ばす */
        sqStartIdx += sqRemoveOffset;
        break;
    }

//    printf("size[%lld]\n", vsqLoop.size());
//    printf("remove[%lld]\n", vsqRemoveIdx.size());
    for (SQWORD sqIdx = 0; sqIdx < vsqLoop.size(); sqIdx++) {
        if (!(binary_search(vsqRemoveIdx.begin(), vsqRemoveIdx.end(), sqIdx))) {
            vsqShortest.emplace_back(vsqLoop[sqIdx]);
        }
    }

}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    SCC_Graph scc(sqN);

    vector<vector<SQWORD>> vvEdge(sqN + 1);
    for (SQWORD sqEdge = 0; sqEdge < sqM; sqEdge++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();
        scc.RegistEdge(sqA, sqB);
        vvEdge[sqA].emplace_back(sqB);
    }

    vector<vector<EDGE_ST>> vvstCompEdge;
    scc.Build(vvstCompEdge);

    vector<SQWORD> vsqCompCnt(sqN + 1, 0);
    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        vsqCompCnt[scc[sqNode]]++;
    }

    SQWORD sqStartNode;
    bool bAnsExist;
    set<SQWORD> setAnsNodes;
    for (SQWORD sqComp = 0; sqComp <= sqN; sqComp++) {
        if (2 < vsqCompCnt[sqComp]) {
            for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
                if (sqComp == scc[sqNode]) {
                    sqStartNode = sqNode;
                    setAnsNodes.insert(sqNode);
                }
            }
            break;
        }
    }

    if (0 == setAnsNodes.size()) {
        printf("-1\n");
        return 0;
    }

    vector<vector<SQWORD>> vvstSccEdges(sqN + 1);
    for (SQWORD sqIdx = 1; sqIdx <= sqN; sqIdx++) {
        for (auto e: vvEdge[sqIdx]) {
            if ((setAnsNodes.find(sqIdx) != setAnsNodes.end())
                 && (setAnsNodes.find(e) != setAnsNodes.end())) {
                vvstSccEdges[sqIdx].emplace_back(e);
            }
        }
    }
    
    vector<SQWORD> vsqVisitDepth(sqN + 1, MAX_SQWORD);
    vector<SQWORD> vsqSearchPath;
    vsqSearchPath.emplace_back(sqStartNode);
    FindLoop(sqStartNode, vvstSccEdges, vsqSearchPath, vsqVisitDepth);

    vector<SQWORD> vsqAns;
    RemoveDuplicateEdge(vsqLoop, vvstSccEdges, vsqAns);

    printf("%lld\n", vsqAns.size());
    for (auto n: vsqAns) {
        printf("%lld\n", n);
    }

    return 0;
}