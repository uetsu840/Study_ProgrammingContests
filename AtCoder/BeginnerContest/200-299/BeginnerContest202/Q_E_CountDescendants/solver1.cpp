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
using LDOUBLE = long double;
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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) { return 0 < a ? a : -a; }

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

struct ST_QUERY {
    SQWORD sqQueryIdx;
    SQWORD sqU;
    SQWORD sqD;

    ST_QUERY(SQWORD idx, SQWORD u, SQWORD d) : sqQueryIdx(idx), sqU(u), sqD(d) {};
};

static SQWORD sqMaxDepth = 0;
static vector<vector<ST_QUERY>> vvsqQueries;
static vector<SQWORD> vsqAns;

static map<SQWORD, SQWORD> dfs_depth(
    SQWORD sqCurNode, 
    SQWORD sqCurDepth,
    const vector<vector<SQWORD>> &vvsqChilds)
{
    sqMaxDepth = MAX(sqCurDepth, sqMaxDepth);
    map<SQWORD, SQWORD> mapDepthList;
    mapDepthList[sqCurDepth] = 1;

    for (auto c: vvsqChilds[sqCurNode]) {
        map<SQWORD, SQWORD> mapSiblings;
        mapSiblings = dfs_depth(c, sqCurDepth + 1, vvsqChilds);

        if (mapDepthList.size() < mapSiblings.size()) {
            for (auto m: mapDepthList) {
                if (mapSiblings.find(m.first) != mapSiblings.end()) {
                    mapSiblings[m.first] += m.second;
                } else {
                    mapSiblings[m.first] = m.second;
                }
            }
            swap(mapSiblings, mapDepthList);
        } else {
            for (auto m: mapSiblings) {
                if (mapDepthList.find(m.first) != mapDepthList.end()) {
                    mapDepthList[m.first] += m.second;
                } else {
                    mapDepthList[m.first] = m.second;
                }
            }
        }
    }

    for (auto q: vvsqQueries[sqCurNode]) {
        SQWORD sqAns = mapDepthList[q.sqD];
        vsqAns[q.sqQueryIdx] = sqAns;
    }

#if 0
    printf("cur: %lld\n", sqCurNode);
    for (auto m: mapDepthList) {
        printf("[%lld %lld]\n", m.first, m.second);
    }
#endif
    return mapDepthList;
}


int main(void)
{
    SQWORD sqN = inputSQWORD();
    vector<vector<SQWORD>> vvsqChilds(sqN);
    for (SQWORD sqNodeIdx = 1; sqNodeIdx < sqN; sqNodeIdx++) {
        SQWORD sqP = inputSQWORD();
        sqP--;
        vvsqChilds[sqP].emplace_back(sqNodeIdx);
    }
    SQWORD sqQ = inputSQWORD();
    vvsqQueries.resize(sqN);
    vsqAns.resize(sqQ);
    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqU = inputSQWORD(); /* 頂点 */
        SQWORD sqD = inputSQWORD();

        sqU--;

        vvsqQueries[sqU].emplace_back(sqQueryIdx, sqU, sqD);
    }

    /* first dfs */
    dfs_depth(0, 0, vvsqChilds);

    for (auto a: vsqAns) {
        printf("%lld\n", a);
    }

    return 0;
}
