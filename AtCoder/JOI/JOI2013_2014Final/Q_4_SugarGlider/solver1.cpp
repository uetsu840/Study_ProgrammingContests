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

#define MAX_V           (100000)
#define SQWORD_INF      (1000000000000000000)

struct EDGE_ST {
    SQWORD sqTo;
    SQWORD sqCost;

    EDGE_ST(SQWORD to, SQWORD cost) {
        sqTo = to;
        sqCost = cost;
    };
};

int main()
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_M = inputSQWORD();
    SQWORD sqInput_X = inputSQWORD();
    vector<SQWORD> vsqInput_H;
    vector<EDGE_ST> vstEdges[MAX_V + 1];
    typedef pair<SDWORD, SDWORD> P;

//    printf("n:%lld m:%lld x:%lld\n", sqInput_N, sqInput_N, sqInput_X);

    vsqInput_H.emplace_back(0);
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        vsqInput_H.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqInput_M; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();
        SQWORD sqCost = inputSQWORD();

//        printf("from:%lld to:%lld cost:%lld h:%lld-%lld\n",  sqA, sqB, sqCost, vsqInput_H[sqA], vsqInput_H[sqB]);

        if (sqCost <= vsqInput_H[sqA]) {
            vstEdges[sqA].emplace_back(EDGE_ST(sqB, sqCost));
        }
        if (sqCost <= vsqInput_H[sqB]) {
            vstEdges[sqB].emplace_back(EDGE_ST(sqA, sqCost));
        }
    }

    static vector<SQWORD> cost(MAX_V + 1, SQWORD_INF);
    priority_queue<P, vector<P>, greater<P>> que;
    
    cost[1] = 0;
    que.push(P(0, 1));

    while (!que.empty()) {
        P p = que.top();
        que.pop();

        SDWORD v = p.second;
//        printf("v:%d size[%d]\n", v, vstEdges[v].size());
        if (p.first <= cost[v]) {
            for (SDWORD lIdx = 0; lIdx < vstEdges[v].size(); lIdx++) {
                EDGE_ST e = vstEdges[v][lIdx];

                SQWORD sqHeight;

                if (0 == v) {
                    sqHeight = sqInput_X;
                } else {
                    if (cost[v] <= sqInput_X) {
                        sqHeight = sqInput_X - cost[v];
                    } else {
                        sqHeight = 0;
                    }
                }

                SQWORD sqMoveCost;

                if (vsqInput_H[e.sqTo] < sqHeight - e.sqCost) {
                    /* 高すぎ */
                    sqMoveCost = sqHeight - vsqInput_H[e.sqTo];
                } else if (sqHeight - e.sqCost < 0) {
                    sqMoveCost = e.sqCost * 2 - sqHeight;
                } else {
                    sqMoveCost = e.sqCost;
                }

//                printf("to > %lld, cost[%lld]\n", e.sqTo, sqMoveCost);

                if (cost[e.sqTo] > cost[v] + sqMoveCost) {
                    cost[e.sqTo] = cost[v] + sqMoveCost;
                    que.push(P(cost[e.sqTo], e.sqTo));
                }
            }
        }
    }

    SQWORD sqLastHeight;
    if (cost[sqInput_N] <= sqInput_X) {
        sqLastHeight = sqInput_X - cost[sqInput_N];
    } else {
        sqLastHeight = 0;
    }

    if (SQWORD_INF == cost[sqInput_N]) {
        printf("-1\n");
    } else {
        printf("%lld\n", cost[sqInput_N] + (vsqInput_H[sqInput_N] - sqLastHeight));
    }

    return 0;
}