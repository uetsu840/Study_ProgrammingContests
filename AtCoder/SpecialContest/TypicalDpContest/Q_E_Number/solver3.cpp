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

static SQWORD addMod(SQWORD sqA, SQWORD sqB)
{
    return ((sqA + sqB) % DIV_MOD);
}

static SQWORD mulMod(SQWORD sqA, SQWORD sqB)
{
    return ((sqA * sqB) % DIV_MOD);
}

int main()
{
    char strInputN[MAX_DIGITS + 1];
    SDWORD lInputD;

    lInputD = inputSDWORD();
    inputString(strInputN);
    SDWORD lDigitNum = strlen(strInputN);

//    printf("digit num = %s:%d\n", strInputN, lDigitNum);

    vector<SDWORD> vDigits;

    for (SDWORD lIdx = 0; lIdx < lDigitNum; lIdx++) {
        SDWORD lDigit = (SDWORD)(strInputN[lIdx] - '0');
        vDigits.emplace_back(lDigit);
    }

    /* Tbl[i] 0-9の数字を、Dで割った剰余がiとなる個数の和 */
    vector<SQWORD> vsqQuotTbl(lInputD, 0);
    for (SDWORD lIdx = 0; lIdx < 10; lIdx++) {
        vsqQuotTbl[lIdx % lInputD]++;
    }

    /* dp(i)[j] 上からn桁目までの合計をDで割った剰余がjとなる数の個数 */
    SQWORD asqDpTbl[MAX_D];
    SQWORD asqDpTblNext[MAX_D];
    memset(asqDpTbl, 0, sizeof(asqDpTbl));
    memset(asqDpTblNext, 0, sizeof(asqDpTblNext));

    /* DP */
    SDWORD lStartDigit = 0;
    for (SDWORD lDigitIdx = 0; lDigitIdx < lDigitNum; lDigitIdx++) {
        for (SQWORD sqModIdxCur = 0; sqModIdxCur < lInputD; sqModIdxCur++) {
            SQWORD sqSum = 0;
            for (SQWORD sqMod = 0; sqMod < lInputD; sqMod++) {
                if (0 < vsqQuotTbl[sqMod]) {
                    SQWORD sqNextIdx = (sqModIdxCur + sqMod) % lInputD;
                    SQWORD sqPattern = mulMod(asqDpTbl[sqModIdxCur], vsqQuotTbl[sqMod]);
                    asqDpTblNext[sqNextIdx] = addMod(asqDpTblNext[sqNextIdx], sqPattern);
                }
            }
        }

        /* それまでの桁が最大だった場合を足す */
        SDWORD lDigitUpper = vDigits[lDigitIdx];
        for (SDWORD lNum = 0; lNum < lDigitUpper; lNum++) {
            SDWORD lModIdx = (lStartDigit + lNum) % lInputD;
            asqDpTblNext[lModIdx] = addMod(asqDpTblNext[lModIdx], 1);
        }
        lStartDigit = (lStartDigit + vDigits[lDigitIdx]) % lInputD;

        memcpy(asqDpTbl, asqDpTblNext, sizeof(asqDpTbl));
        memset(asqDpTblNext, 0, sizeof(asqDpTblNext));
    }
    /* N分を加算する。 */
    asqDpTbl[lStartDigit] = addMod(asqDpTbl[lStartDigit], 1LL);

    /* 0を除外 */
    asqDpTbl[0] = addMod(asqDpTbl[0], -1LL);

    printf("%lld\n", asqDpTbl[0]);

    return 0;
}