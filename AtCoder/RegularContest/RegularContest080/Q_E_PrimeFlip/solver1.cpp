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
static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; };

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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/

typedef struct {
    SDWORD lTo;
    SQWORD sqCap;
    SQWORD sqCost;
    SDWORD lRev;
} ST_EDGE;

#define MAX_V   (202)

struct Flow {
private:
    vector<ST_EDGE> avGraph[MAX_V];
    SDWORD  alDims[MAX_V];
    SQWORD  sqEdgeNum;
    SQWORD  asqDist[MAX_V];
    SDWORD  alPrevV[MAX_V];   
    SDWORD  alPrevEdge[MAX_V];      /*直前の頂点と辺 */

    SDWORD  alDinic_Level[MAX_V];
    SDWORD  alDinic_Iter[MAX_V];

public:
    Flow() {
        for (auto v: avGraph) {
            v.clear();
        }
        memset(alDims, 0, sizeof(alDims));
        memset(asqDist, 0, sizeof(asqDist));
        memset(alPrevV, 0, sizeof(alPrevV)); 
        memset(alPrevEdge, 0, sizeof(alPrevEdge));
        memset(alDinic_Level, 0, sizeof(alDinic_Level));
        memset(alDinic_Iter, 0, sizeof(alDinic_Iter));
    }
    void addEdge(SDWORD lFrom, SDWORD lTo, SQWORD sqCap, SQWORD sqCost) {
//        printf("%d ----[%lld] ----> %d\n", lFrom, sqCap, lTo);
        
        alDims[lFrom]++;
        avGraph[lFrom].push_back((ST_EDGE){lTo, sqCap, sqCost, (SDWORD)(avGraph[lTo].size())});
        avGraph[lTo].push_back((ST_EDGE){lFrom, 0, -sqCost, (SDWORD)(avGraph[lFrom].size() - 1)});
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

    SQWORD getFlow(SDWORD lPnt)
    {
        SQWORD sqCapSum = 0; 
        for (auto e: avGraph[lPnt]) {
            sqCapSum += e.sqCap;
        }
        return alDims[lPnt] - sqCapSum;
    }
};

/*----------------------------------------------*/

static void getPrimes(vector<SDWORD> &vlPrimes, SDWORD lMax)
{
    /* lSearchMax と ループ内の *2 の関係から、入力は3以上とすること */
    if (!(1 <= lMax)) {
        printf("Oops!");
        return;
    }

    /* 素数のリストを作る */
    vector<bool> vbIsPrime(lMax + 1, true);
    SDWORD lSearchMax = sqrt(lMax) + 1;
    vbIsPrime[0] = false;
    vbIsPrime[1] = false;
    for (SDWORD lPrime = 2; lPrime < vbIsPrime.size(); lPrime++) {
        if (vbIsPrime[lPrime]) {
            vlPrimes.emplace_back(lPrime);
            if (lPrime <= lSearchMax) {
                for (SDWORD lCurNum = lPrime * 2; lCurNum <= lMax; lCurNum += lPrime) {
                    vbIsPrime[lCurNum] = false;
                }
            }
        }
    }
}    

/*----------------------------------------------*/

#define PNT_NO_START    (0)
#define PNT_NO_OFFSET   (100)
#define PNT_NO_TARGET   (201)
#define FLOW_OFFSET     (1000000000)
#define CAPACITY_INF    (1000000001)
#define DUMMY_COST      (1)

int main(void)
{
    SQWORD sqN = inputSQWORD();

    Flow solverFlow;
    vector<SQWORD> vsqA;
    vector<SQWORD> vsqB;
    vector<SDWORD> vlPrimes;

    getPrimes(vlPrimes, 1e7 + 1);
    vlPrimes.erase(find(vlPrimes.begin(), vlPrimes.end(), 2));

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        vsqA.emplace_back(sqA);
    }

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        if (0 < sqIdx) {
            if (!(vsqA[sqIdx - 1] == vsqA[sqIdx] - 1)) {
                vsqB.emplace_back(vsqA[sqIdx]);
            }
        } else {
            vsqB.emplace_back(vsqA[sqIdx]);      
        }
        if (sqIdx < sqN - 1) {
            if (!(vsqA[sqIdx] + 1 == vsqA[sqIdx + 1])) {
                vsqB.emplace_back(vsqA[sqIdx] + 1);
            }
        } else {
            vsqB.emplace_back(vsqA[sqIdx] + 1);
        }
    }

    map<SQWORD, SQWORD> mapB;

    for (SQWORD sqIdx = 0; sqIdx < vsqB.size(); sqIdx++) {
        mapB[vsqB[sqIdx]] = sqIdx + 1;
    }

    /* 二部マッチング */
    for (auto b: vsqB) {
        solverFlow.addEdge(PNT_NO_START,            mapB[b],       1, DUMMY_COST);
        solverFlow.addEdge(mapB[b] + PNT_NO_OFFSET, PNT_NO_TARGET, 1, DUMMY_COST);
    }

    for (auto p: vlPrimes) {
        for (auto b: vsqB) {
            auto it = lower_bound(vsqB.begin(), vsqB.end(), b + p);
            if (it != vsqB.end()) {
                if (*it == b + p) {
                    if (b + p < 1e7 + 1) {
                        solverFlow.addEdge(mapB[b], mapB[b + p] + PNT_NO_OFFSET, 1, DUMMY_COST);
                    }
                }
            }
        }
    }

    SQWORD sqMaxFlow = solverFlow.MaxFlow(PNT_NO_START, PNT_NO_TARGET);
    vector<SQWORD> vsqBRestEven;
    vector<SQWORD> vsqBRestOdd;
    for (auto b: vsqB) {
        SQWORD sqFlow = solverFlow.getFlow(mapB[b]);

        if (0 == sqFlow) {
            if (0 == b % 2) {
                vsqBRestEven.emplace_back(b);
            } else {
                vsqBRestOdd.emplace_back(b);
            }
        }
    }

    SQWORD sqAns = sqMaxFlow;    
    sqAns += (vsqBRestEven.size() / 2) * 2;
    sqAns += (vsqBRestOdd.size() / 2) * 2;
    if (0 < (vsqBRestEven.size() %  2)) {
        sqAns += 3;
    }

    printf("%lld\n", sqAns);

    return 0;
}
/*------------------------------------------*/
