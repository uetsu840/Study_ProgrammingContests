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


#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)
#define DIVISOR (1000000007)

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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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

#define MAX_NODE    (1000)
static bool s_abIsVisited[MAX_NODE + 1];
static vector<SDWORD> s_avlEdge[MAX_NODE + 1];
static vector<vector<pair<SDWORD, SDWORD>>>  s_vvpMemo(MAX_NODE + 1, vector<pair<SDWORD, SDWORD>>{MAX_NODE+1, make_pair(0, 0)});

static void getPatternNum(
    SDWORD lPrevNode,
    SDWORD lNodeNo,
    SDWORD &lPattern,
    SDWORD &lEdge) 
{
    SDWORD lPatternNum = 1;
    SDWORD lEdgeNum = 0;
    s_abIsVisited[lNodeNo] = true;

    if (0 < s_vvpMemo[lPrevNode][lNodeNo].first) {
        lPattern = s_vvpMemo[lPrevNode][lNodeNo].first;
        lEdge = s_vvpMemo[lPrevNode][lNodeNo].second;
        return;
    }

    for (auto lNextNode: s_avlEdge[lNodeNo]) {
        if (!s_abIsVisited[lNextNode]) {
            SDWORD lNextNodePattern;
            SDWORD lNextNodeEdge;
            getPatternNum(lNodeNo, lNextNode, lNextNodePattern, lNextNodeEdge);

            lPatternNum = mulMod(lPatternNum, lNextNodePattern);
            lPatternNum = mulMod(lPatternNum, combMod(addMod(lEdgeNum, lNextNodeEdge), lNextNodeEdge));
            lEdgeNum += lNextNodeEdge;
        }
    }
    lEdgeNum++;

//    printf("node %d p:%lld e:%lld\n", lNodeNo, sqPatternNum, sqEdgeNum);
    lEdge = lEdgeNum;
    lPattern = lPatternNum;
    s_vvpMemo[lPrevNode][lNodeNo] = make_pair(lPattern, lEdge);
}

int main()
{
    SDWORD  lInput_N = inputSDWORD();
    vector<pair<SDWORD, SDWORD>> vEdges;

    for (SDWORD lEdgeIdx = 0; lEdgeIdx < lInput_N - 1; lEdgeIdx++) {
        SDWORD lInput_a = inputSDWORD();
        SDWORD lInput_b = inputSDWORD();

//        printf("input <%d - %d>\n", lInput_a, lInput_b);

        s_avlEdge[lInput_a].emplace_back(lInput_b);
        s_avlEdge[lInput_b].emplace_back(lInput_a);
        vEdges.emplace_back(make_pair(lInput_a, lInput_b));
    }

    SDWORD lAns = 0;
    for (auto startEdge:vEdges) {
        SDWORD lPatternNumL, lEdgeNumL;
        SDWORD lPatternNumR, lEdgeNumR;

        memset(s_abIsVisited, 0, sizeof(s_abIsVisited));
        s_abIsVisited[startEdge.second] = true;
        getPatternNum(startEdge.second, startEdge.first, lPatternNumL, lEdgeNumL);

        memset(s_abIsVisited, 0, sizeof(s_abIsVisited));
        s_abIsVisited[startEdge.first] = true;
        getPatternNum(startEdge.first, startEdge.second, lPatternNumR, lEdgeNumR);

        SDWORD lPatternOne = 1;
        lPatternOne = mulMod(lPatternOne, lPatternNumL);
        lPatternOne = mulMod(lPatternOne, lPatternNumR);
        lPatternOne = mulMod(lPatternOne, combMod(lEdgeNumL - 1 + lEdgeNumR - 1, lEdgeNumR - 1));        
        lAns = addMod(lAns, lPatternOne);
    }
    printf("%d\n", lAns);
    return 0;
}