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

#define MAX_M            (50000)
#define PRIME_TABLE_SIZE (MAX_M * 2 + 1)
int main()
{
    /**
     *  N = 2m-1   (i.e. m = (N+1)/2) として mを考える。
     *  0 < m < 50000
     */
    static bool s_abIsPrime[PRIME_TABLE_SIZE]; 
    static bool s_abIsLikeNumber[PRIME_TABLE_SIZE];
    static SDWORD s_alLikeNumberCumSum[PRIME_TABLE_SIZE];

    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_abIsPrime); lIdx++) {
        s_abIsPrime[lIdx] = true;
    }
    s_abIsPrime[0] = false;
    s_abIsPrime[1] = false;

    /* 素数テーブルを作る */
    SDWORD lMaxDiv = sqrt(PRIME_TABLE_SIZE) + 1;
    for (SDWORD lDivisor = 2; lDivisor <= lMaxDiv; lDivisor++) {
        if (s_abIsPrime[lDivisor]) {
            for (SDWORD lCurNum = lDivisor * 2; lCurNum < PRIME_TABLE_SIZE; lCurNum += lDivisor) {
                s_abIsPrime[lCurNum] = false;
            }
        }
    }

    /* m と 2m-1 が素数のテーブルを作る */
    s_abIsLikeNumber[0] = 0;
    for (SDWORD lIdx = 1; lIdx < MAX_M; lIdx++) {
        if (s_abIsPrime[lIdx] && s_abIsPrime[2 * lIdx - 1]) {
            s_abIsLikeNumber[lIdx] = true;
        } else {
            s_abIsLikeNumber[lIdx] = false;
        }
    }
    
    /* 累積和を作る */
    SDWORD lCumSum = 0;
    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_alLikeNumberCumSum); lIdx++) {
        if (s_abIsLikeNumber[lIdx])  {
            lCumSum++;
        }
//        printf("%d %d\n", lIdx, lCumSum);
        s_alLikeNumberCumSum[lIdx] = lCumSum;
    }

    /* クエリ */
    SQWORD lInput_Q = inputSDWORD();
    for (SDWORD lQueryIdx = 0; lQueryIdx < lInput_Q; lQueryIdx++) {
        SDWORD lInput_l = inputSDWORD();
        SDWORD lInput_r = inputSDWORD();

        SDWORD lInput_lm = (lInput_l + 1) / 2;
        SDWORD lInput_rm = (lInput_r + 1) / 2;
        printf("%d\n", s_alLikeNumberCumSum[lInput_rm] - s_alLikeNumberCumSum[lInput_lm - 1]);
    }

    return 0;
}
