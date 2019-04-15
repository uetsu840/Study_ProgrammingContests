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

#define MAX_DIGITS  (10000)
#define MAX_D       (100)

#define DIV_MOD     (1000000007)

int main()
{
    char strInputN[MAX_DIGITS + 1];
    SDWORD lInputD;

    lInputD = inputSDWORD();
    inputString(strInputN);
    SDWORD lDigitNum = strlen(strInputN);

    printf("digit num = %s:%d\n", strInputN, lDigitNum);

    vector<SDWORD> vDigits;

    for (SDWORD lIdx = 0; lIdx < lDigitNum; lIdx++) {
        SDWORD lDigit = (SDWORD)(strInputN[lIdx] - '0');
        vDigits.emplace_back(lDigit);
    }
    reverse(vDigits.begin(), vDigits.end());

    /* Tbl[i] 0-9の数字を、Dで割った剰余がiとなる個数の和 */
    vector<SQWORD> vsqQuotTbl(lInputD, 0);
    for (SDWORD lIdx = 0; lIdx < 10; lIdx++) {
        vsqQuotTbl[lIdx % lInputD]++;
    }

    /* dp(i)[j] n桁目までの合計をDで割った剰余がjとなる数の個数 */
    SQWORD asqDpTbl[MAX_D];
    SQWORD asqDpTblNext[MAX_D];
    memset(asqDpTbl, 0, sizeof(asqDpTbl));
    memset(asqDpTblNext, 0, sizeof(asqDpTblNext));

    /* Initialize */
    for (SDWORD lNum = 0; lNum < lInputD; lNum++) {
        asqDpTbl[lNum] = vsqQuotTbl[lNum];
    }

    /* DP */
    for (SDWORD lDigitIdx = 1; lDigitIdx < lDigitNum; lDigitIdx++) {
        printf("---------------[%d]\n", lDigitIdx);

        for (SQWORD sqModIdxNext = 0; sqModIdxNext < lInputD; sqModIdxNext++) {
            SQWORD sqSum = 0;
            for (SQWORD sqModIdxCur = 0; sqModIdxCur < lInputD; sqModIdxCur++) {
                SQWORD sqCompIdx = (lInputD - sqModIdxNext + sqModIdxCur) % lInputD;
                SQWORD sqNum = asqDpTbl[sqModIdxCur] * vsqQuotTbl[sqCompIdx];
                printf("   >> %lld %lld\n", asqDpTbl[sqModIdxCur], vsqQuotTbl[sqCompIdx]);
                sqSum = (sqSum + sqNum) % DIV_MOD;
            }
            asqDpTblNext[sqModIdxNext] = sqSum;
            printf("%lld %lld\n", sqModIdxNext, sqSum);
        }
        memcpy(asqDpTbl, asqDpTblNext, sizeof(asqDpTbl));
    }

    printf("%lld\n", asqDpTbl[0]);

    return 0;
}