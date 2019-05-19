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

//#define MAX_N   (1000000LL)
#define MAX_N   (200LL)

int main(void)
{
    static set<pair<SQWORD, SQWORD>> vPrimeNums;
    static SDWORD s_alPrimeNum[MAX_N + 1];
    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_alPrimeNum); lIdx++) {
        s_alPrimeNum[lIdx] = 1;
    }

    SDWORD lDbgLoopCnt = 0;

    vPrimeNums.insert(make_pair(1, 1));
    static pair<SQWORD, SQWORD> apairNextPrimeNums[MAX_N];
    for (SQWORD sqCurNum = 2; sqCurNum < MAX_N; sqCurNum++) {
        lDbgLoopCnt++;
        if (1 == s_alPrimeNum[sqCurNum]) {
            /* 素数 */
            SDWORD lAddCnt = 0;
            auto it = vPrimeNums.begin();
            while (it != vPrimeNums.end()) {
                SQWORD sqNextNum = it->first * sqCurNum;
                for (SQWORD sqPow = 1; sqNextNum <= (SQWORD)MAX_N; sqNextNum *= sqCurNum, sqPow++) {
                    SDWORD lFacoryCnt = it->second * (sqPow + 1);
                    s_alPrimeNum[sqNextNum] = lFacoryCnt;
                    apairNextPrimeNums[lAddCnt] = make_pair((SDWORD)sqNextNum, lFacoryCnt);
                    lAddCnt++;
                }
                if (MAX_N / sqCurNum < it->first) {
                    it = vPrimeNums.erase(it);
                } else {
                    ++it;
                }
            }
            for (SDWORD lIdx = 0; lIdx < lAddCnt; lIdx++) {
                vPrimeNums.insert(apairNextPrimeNums[lIdx]);
            }
        }
    }

    printf("===\n");
    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_alPrimeNum); lIdx++) {
        printf("p: %d : %d\n", lIdx, s_alPrimeNum[lIdx]);
    }

    /* Cumsum */
    static SQWORD s_asqAnswers[MAX_N + 1];

    s_asqAnswers[1] = 2;
    for (SQWORD sqNum = 2; sqNum < ArrayLength(s_asqAnswers); sqNum++) {
        s_asqAnswers[sqNum] = s_asqAnswers[sqNum - 1] + (sqNum - s_alPrimeNum[sqNum] + 1);
    }

    /* Query */
    SDWORD lInput_Q = inputSDWORD();
    for (SDWORD lQueryIdx = 0; lQueryIdx < lInput_Q; lQueryIdx++) {
        SDWORD lInput_N = inputSDWORD();
        printf("%lld\n", s_asqAnswers[lInput_N]);
    }
    return 0;
}