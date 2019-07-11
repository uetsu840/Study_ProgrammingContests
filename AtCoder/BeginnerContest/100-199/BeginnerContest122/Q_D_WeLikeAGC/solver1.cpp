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

static SQWORD calcZeroToNumXor(SQWORD sqNum)
{
    SQWORD sqRet = 0;
    for (DWORD dwBit = 1; dwBit < 64; dwBit++) {
        SQWORD sqMask = 0x1LL << dwBit;
        if (sqMask & sqNum) {
            if (0 == (sqNum & 0x1)) {
                sqRet |= sqMask;
            }
        }
    }

    /* lowest bit */
    if ((sqNum % 4 == 1) || (sqNum % 4 == 2)) {
        sqRet |= 1;
    }

    return sqRet;
}

static SQWORD s_asqPatternTbl[104];
static SQWORD s_asqAnswerTbl[104];
static SQWORD s_asqNgTbl[104];

#define ANS_MOD (1000000007)

int main()
{
    SQWORD sqInput_N = inputSQWORD();

    s_asqPatternTbl[0] = 1;
    for (DWORD dwIdx = 1; dwIdx < sqInput_N + 1; dwIdx++) {
        s_asqPatternTbl[dwIdx] = (s_asqPatternTbl[dwIdx - 1] * 4) % ANS_MOD;
//        printf("%d %lld\n", dwIdx, s_asqPatternTbl[dwIdx]);
    }
  
    s_asqAnswerTbl[0] = 1;
    s_asqAnswerTbl[1] = 4;
    s_asqAnswerTbl[2] = 16;
    s_asqNgTbl[0] = 0;
    s_asqNgTbl[1] = 0;
    s_asqNgTbl[2] = 0;
    SQWORD sqNgPairs = 0;
    for (DWORD dwIdx = 3; dwIdx <= sqInput_N; dwIdx++) {
        sqNgPairs = 0;

        /* OKな文字列 + [AC + [C] | GA + [C] | AC + [G]] */
        sqNgPairs = (sqNgPairs + (s_asqAnswerTbl[dwIdx - 3] * 3)) % ANS_MOD;
//        printf(" ng1 %d %lld\n", dwIdx, s_asqAnswerTbl[dwIdx - 3] * 3);

        /* OKな文字列 A [T, G] G  + [C] */
        if (4 <= dwIdx) {
            sqNgPairs = (sqNgPairs + (s_asqAnswerTbl[dwIdx - 4] * 2)) % ANS_MOD;
//            printf(" ng2 %d %lld\n", dwIdx, s_asqAnswerTbl[dwIdx - 4] * 2);
        }

        /* すでにNGな文字列 + [A, G, C, T]  */
        sqNgPairs = (sqNgPairs + (s_asqNgTbl[dwIdx - 1] * 4)) % ANS_MOD;
//        printf(" ng3 %d %lld\n", dwIdx, s_asqNgTbl[dwIdx - 1] * 4);

        s_asqNgTbl[dwIdx] = sqNgPairs;
        s_asqAnswerTbl[dwIdx] = (s_asqPatternTbl[dwIdx] + ANS_MOD - sqNgPairs) % ANS_MOD;
    }

    SQWORD sqFinalAns = s_asqAnswerTbl[sqInput_N];
    printf("%lld\n",sqFinalAns);


    return 0;
}