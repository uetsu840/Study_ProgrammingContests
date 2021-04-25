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

#define     SQWORD_INF      (100100100100100100)

struct NODE_COST_ST {
    SDWORD lSum;
    SDWORD lMod;
    SDWORD lDigit;

    NODE_COST_ST(SDWORD s, SDWORD m, SDWORD d) : lSum(s), lMod(m), lDigit(d) {};
};

bool operator< (const NODE_COST_ST &a, const NODE_COST_ST &b)
{
    return a.lSum < b.lSum;
}

bool operator> (const NODE_COST_ST &a, const NODE_COST_ST &b)
{
    return a.lSum > b.lSum;
}

#define         MAX_DIGITS  (30)
vector<SDWORD> power10;

void initPow10(SDWORD lK)
{
    SDWORD lPow = 1;
    for (SDWORD lIdx = 0; lIdx < MAX_DIGITS; lIdx++) {
        power10.emplace_back(lPow);
        lPow *= 10;
        lPow %= lK;
    }
}

SDWORD solveDijkstra(
    SDWORD lK,
    vector<vector<SDWORD>> &cost)
{
    priority_queue<NODE_COST_ST, vector<NODE_COST_ST>, greater<NODE_COST_ST>> que;
    
    cost[0][0] = 0;
    que.push(NODE_COST_ST(0, 0, 0));

    while (!que.empty()) {
        NODE_COST_ST node = que.top();
        que.pop();

        SDWORD mod = node.lMod;
        SDWORD digit = node.lDigit;
//        printf("%lld mod[%lld] digit[%lld]\n", node.lSum, mod, digit);
        if ((0 < node.lSum) && (0 == mod)) {
            return node.lSum;
        }

        SDWORD lCurCost = cost[mod][digit];


        if (node.lSum <= lCurCost) {
            SDWORD lMin = 0;
            for (SDWORD lSingleNum = lMin; lSingleNum < 10; lSingleNum++) {
                SDWORD lNextMod = (mod + lSingleNum * power10[digit]) % lK;
                SDWORD lNextCost = lCurCost + lSingleNum;
//                printf(" test c[%lld] - [%lld]\n", lNextCost, cost[lNextMod][digit + 1]);
                if (digit + 1 < MAX_DIGITS) {
                    if ((lNextCost < cost[lNextMod][digit + 1]) || (0 == lNextMod)) {
//                        printf(" push %lld mod[%lld] digit[%lld]\n", lNextCost, lNextMod, digit+1);
                        cost[lNextMod][digit + 1] = lNextCost;
                        que.emplace(lNextCost, lNextMod, digit + 1);
                    }
                }
            }
        }
    }

    return 0;
}

#define COST_INF    (100100100)

int main()
{
    SDWORD lK = inputSDWORD();
    vector<vector<SDWORD>> vvCost(lK, vector<SDWORD>(MAX_DIGITS, COST_INF));
    initPow10(lK);

    SDWORD lAns = solveDijkstra(lK, vvCost);

    printf("%d\n", lAns);

    return 0;
}