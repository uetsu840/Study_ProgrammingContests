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
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

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

/*-----------------------------------------------------*/

#define NODE_NUM    (9)

/* 頂点j に居るコマの番号。0はコマがいないものとする。 */
static SQWORD serialize(const vector<SQWORD> &vsqPieceOnNode)
{
    SQWORD sqSerilizedNum = 0;
    SQWORD sqOfs = 1;
    for (SQWORD sqIdx = 0; sqIdx < NODE_NUM; sqIdx++) {
        sqSerilizedNum += sqOfs * vsqPieceOnNode[sqIdx];
        sqOfs *= 10;
    }
    return sqSerilizedNum;
}

static void deSerialize(SQWORD sqSerial, vector<SQWORD> &vsqPieceOnNode, SQWORD &sqEmptyNode)
{
    for (SQWORD sqIdx = 0; sqIdx < NODE_NUM; sqIdx++) {
        SQWORD sqMod = sqSerial % 10;
        if (0 == sqMod) {
            sqEmptyNode = sqIdx;
        }
        vsqPieceOnNode[sqIdx] = sqMod;
        sqSerial -= sqMod;
        sqSerial /= 10;
    }
}



/*----------------------------------------------*/
#define     SQWORD_INF      (100100100100100100)


/*----------------------------------------------*/
int main(void)
{
    SQWORD sqM = inputSQWORD();

    vector<vector<SQWORD>> vvEdge(NODE_NUM);
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();

        sqU--;
        sqV--;

        vvEdge[sqU].emplace_back(sqV);
        vvEdge[sqV].emplace_back(sqU);
    }

    vector<SQWORD> vvInitPos(NODE_NUM);
    for (SQWORD sqIdx = 1; sqIdx < NODE_NUM; sqIdx++) {
        SQWORD sqPos = inputSQWORD();
        sqPos--;
        vvInitPos[sqPos] = sqIdx;
    }


    typedef pair<SQWORD, SQWORD> P;
    priority_queue<P, vector<P>, greater<P>> que;
    map<SQWORD, SQWORD> mapCost;

    SQWORD sqFrom = serialize(vvInitPos);
    mapCost[sqFrom] = 0;
    que.push(P(0, sqFrom));

    while (!que.empty()) {
        P p = que.top();
        que.pop();

        SDWORD v = p.second;
        SQWORD sqCurCost = SQWORD_INF;
        if (mapCost.find(v) != mapCost.end()) {
            sqCurCost = mapCost[v];
        }
        if (p.first <= sqCurCost) {
            vector<SQWORD> vsqCurPos(NODE_NUM);
            SQWORD sqEmptyNode;
            deSerialize(v, vsqCurPos, sqEmptyNode);

            for (auto moveFrom: vvEdge[sqEmptyNode]) {
                vector<SQWORD> vsqNextPos = vsqCurPos;
                vsqNextPos[sqEmptyNode] = vsqCurPos[moveFrom];
                vsqNextPos[moveFrom]    = 0; 

                SQWORD to = serialize(vsqNextPos);

                SQWORD sqToCost = SQWORD_INF;
                if (mapCost.find(to) != mapCost.end()) {
                    sqToCost = mapCost[to];
                }
                if (sqToCost > mapCost[v] + 1) {
                    mapCost[to] = mapCost[v] + 1;
                    que.push(P(mapCost[to], to));
                }
            }
        }
    }

    vector<SQWORD> vsqLastPos = {1, 2, 3, 4, 5, 6, 7, 8, 0};

    SQWORD sqLastCost = -1;
    if (mapCost.find(serialize(vsqLastPos)) != mapCost.end()) {
        sqLastCost = mapCost[serialize(vsqLastPos)];
    }
    printf("%lld\n", sqLastCost);
}

