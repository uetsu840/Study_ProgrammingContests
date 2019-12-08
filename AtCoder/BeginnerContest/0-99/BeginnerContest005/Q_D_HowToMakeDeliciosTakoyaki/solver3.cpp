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

static inline SQWORD getScore(vector<vector<SDWORD>> &vvD, 
    SDWORD lR1,
    SDWORD lR2,
    SDWORD lC1,
    SDWORD lC2)
{
    SDWORD lRet = vvD[lR2][lC2] - vvD[lR1][lC2] - vvD[lR2][lC1] + vvD[lR1][lC1];

    return lRet;
}

int main()
{
    SDWORD lN = inputSDWORD();

    vector<vector<SDWORD>> vvDSum(lN + 1, vector<SDWORD>(lN + 1, 0));
    for (SQWORD sqR = 1; sqR <= lN; sqR++) {
        SQWORD lSum = 0;
        for (SQWORD sqC = 1; sqC <= lN; sqC++) {
            lSum += inputSQWORD();
            vvDSum[sqR][sqC] = lSum;
        }
    }
    for (SQWORD sqC = 1; sqC <= lN; sqC++) {
        SQWORD sqSum = 0;
        for (SQWORD sqR = 1; sqR <= lN; sqR++) {
            sqSum += vvDSum[sqR][sqC];
            vvDSum[sqR][sqC] = sqSum;
//            printf("%lld ", sqSum);
        }
//        printf("\n");
    }

    vector<SDWORD> vQueryAns(lN * lN + 1, -1);
    for (SQWORD sqR1 = 0; sqR1 < lN; sqR1++) {
        for (SQWORD sqR2 = sqR1 + 1; sqR2 <= lN; sqR2++) {
            for (SQWORD sqC1 = 0; sqC1 < lN; sqC1++) {
                for (SQWORD sqC2 = sqC1 + 1; sqC2 <= lN; sqC2++) {
                    SDWORD lA = (sqR2 - sqR1) * (sqC2 - sqC1);
                    SDWORD lS = getScore(vvDSum, sqR1, sqR2, sqC1, sqC2);
                    vQueryAns[lA] = max(vQueryAns[lA], lS);
                }                   
            }
        }
    }
    SDWORD lMax = 0;
    for (auto it = vQueryAns.begin(); it != vQueryAns.end(); ++it) {
        *it = max(lMax, *it);
        lMax = *it;
    }

    SQWORD sqQ = inputSQWORD();

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD lP = inputSDWORD();

        printf("%ld\n", vQueryAns[lP]);
    }

    return 0;
}