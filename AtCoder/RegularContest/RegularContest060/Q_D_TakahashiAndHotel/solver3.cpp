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

#define MAX_V   (500)
#define INF 2e9

/* 辺を表す構造体 */
struct edge {int to, cap ,rev; };
 
/* グラフの隣接リスト表現 */
vector<edge> G[MAX_V];
/* DFSですべに調べたかのグラフ */
bool used[MAX_V];
 
/* fromからtoへ向かう容量capの辺をグラフに追加する */
/* 相互参照できるようにしている */
void add_edge(int from, int to, int cap){
    G[from].push_back((edge){to, cap, (int)G[to].size()});
    G[to].push_back((edge){from, 0, (int)G[from].size() - 1});    
}
 
/* 増加パスをdfsで探す */
int dfs(int v, int t, int f){
    if(v==t) return f;
    used[v] = true;
    for(int i=0; i<G[v].size(); i++){
        edge &e = G[v][i];
        if(!used[e.to] && e.cap>0){
            int d = dfs(e.to, t, min(f, e.cap));
            if(d>0){
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}
 
int max_flow(int s, int t){
    int flow = 0;
    for(;;){
        memset(used, 0, sizeof(used));
        int f = dfs(s, t, INF);
        if(f==0) return flow;
        flow += f;
    }
}
/*----------------------------------------------*/

class PrimeTable {
    vector<bool> vbIsPrime;

public:
    PrimeTable(SDWORD lMax) {
        /* lSearchMax と ループ内の *2 の関係から、入力は3以上とすること */
        if (!(1 <= lMax)) {
            printf("Oops!");
            return;
        }

        /* 素数のリストを作る */
        vbIsPrime.resize(lMax + 1, true);
        SDWORD lSearchMax = sqrt(lMax) + 1;
        vbIsPrime[0] = false;
        vbIsPrime[1] = false;
        for (SDWORD lPrime = 2; lPrime < lSearchMax; lPrime++) {
            if (vbIsPrime[lPrime]) {
                for (SDWORD lCurNum = lPrime * 2; lCurNum <= lMax; lCurNum += lPrime) {
                    vbIsPrime[lCurNum] = false;
                }
            }
        }
    };

    bool IsPrime(SQWORD sqN)
    {
#if 0
        return vbIsPrime[sqN];
#else
        if(sqN <= 2) return false;
        for (int i = sqrt(sqN)+1; 2 < i; i--) if(sqN % i == 0) return false;
    
        return true;
#endif
    }
};


/*----------------------------------------------*/

#define PNT_NO_START    (0)
#define PNT_NO_OFFSET   (200)
#define PNT_NO_TARGET   (401)
#define FLOW_OFFSET     (1000000000)
#define CAPACITY_INF    (1000000001)
#define DUMMY_COST      (1)

int main(void)
{
    SQWORD sqN = inputSQWORD();

    vector<SQWORD> vsqB;

    PrimeTable prime(1e7 + 1);
 
    map<SQWORD, SQWORD> mapB;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        mapB[sqA] ^= 1;
        mapB[sqA + 1] ^= 1;
    }

    vector<SQWORD> vsqBEven, vsqBOdd;

    for (auto b: mapB) {
        if (0 < b.second) {
            if (0 == (b.first % 2)) {
                vsqBEven.emplace_back(b.first);
            } else {
                vsqBOdd.emplace_back(b.first);
            }
        }
    }

    SQWORD sqNodeEven = 1;
    for (auto e: vsqBEven) {
        SQWORD sqNodeOdd = 1;
        for (auto o: vsqBOdd) {
            if (prime.IsPrime(abs(e - o))) {
                add_edge(sqNodeEven, sqNodeOdd + PNT_NO_OFFSET, 1);
            }
            sqNodeOdd++;
        }
        sqNodeEven++;
    }
    for (SQWORD sqIdx = 1; sqIdx <= vsqBEven.size(); sqIdx++) {
        add_edge(PNT_NO_START, sqIdx, 1);
    }
    for (SQWORD sqIdx = 1; sqIdx <= vsqBOdd.size(); sqIdx++) {
        add_edge(sqIdx + PNT_NO_OFFSET, PNT_NO_TARGET, 1);
    }



    SQWORD sqMaxFlow = max_flow(PNT_NO_START, PNT_NO_TARGET);
    SQWORD vsqCRestEven = vsqBEven.size() - sqMaxFlow;
    SQWORD vsqCRestOdd  = vsqBOdd.size() - sqMaxFlow;

    SQWORD sqAns = sqMaxFlow;    
    sqAns += ((vsqCRestEven / 2) * 2);
    sqAns += ((vsqCRestOdd / 2) * 2);
    if (0 < (vsqCRestEven %  2)) {
        sqAns += 3;
    }

    printf("%lld\n", sqAns);

    return 0;
}
/*------------------------------------------*/
