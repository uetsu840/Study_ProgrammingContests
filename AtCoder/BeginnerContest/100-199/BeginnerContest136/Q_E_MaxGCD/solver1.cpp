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

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

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

/*----------------------------------------------*/

static bool isDividable(
    const vector<SQWORD>& vecsqNums, 
    SQWORD sqDiffMax,
    SQWORD sqTest) 
{
    SQWORD sqN = vecsqNums.size();

    vector<SQWORD> vsqDiffs;
    
    for (auto testnum: vecsqNums) {
        SQWORD sqDiffAbs = min((testnum % sqTest), sqTest - (testnum % sqTest));
        vsqDiffs.emplace_back(testnum % sqTest);
    }
    sort(vsqDiffs.begin(), vsqDiffs.end());

    SQWORD sqDiffSum = accumulate(vsqDiffs.begin(), vsqDiffs.end(), 0);

    if (0 != (sqDiffSum % sqTest)) {
        return false;
    }

    SQWORD sqModSum = 0;
    SQWORD sqLowClipNum = sqN - (sqDiffSum / sqTest);

//    printf("test %lld sum %lld low clip %d\n", sqTest, sqDiffSum, sqLowClipNum);

    for (SQWORD sqIdx = 0; sqIdx < sqLowClipNum; sqIdx++) {
        sqModSum += vsqDiffs[sqIdx];
    }
    for (SQWORD sqIdx = sqLowClipNum; sqIdx < sqN; sqIdx++) {
        sqModSum += (sqTest - vsqDiffs[sqIdx]);
    }

//    printf("test %lld diff %lld max %lld\n", sqTest, sqModSum, sqDiffMax);

    if (sqDiffMax < sqModSum) {
        return false;
    }
    return true;
}

#define MAX_BOX_NUM (400000)
static SDWORD s_alBallCounts[MAX_BOX_NUM + 1];

static void calcPrimeFactorication(
    SQWORD sqNum, vector<pair<SQWORD, SQWORD>> &vlPrimes)
{
    if (1 == sqNum) {
        return;
    }

    SQWORD sqCur = sqNum;
    SQWORD sqUpper = sqrt(sqNum) + 1;
    for (SQWORD sqDiv = 2; sqDiv <= sqUpper; sqDiv++) {
        SDWORD lPowerCnt = 0;
        while(0 == sqCur % sqDiv) {
            sqCur /= sqDiv;
            lPowerCnt++;
        }
        if (0 < lPowerCnt) {
            vlPrimes.emplace_back(make_pair(sqDiv, lPowerCnt));
        }
        if (1 == sqCur) {
            break;
        }
    }
    if (1 < sqCur) {
        vlPrimes.emplace_back(make_pair(sqCur, 1));
    }
}

static void listupDivisorsOne(
    vector<pair<SQWORD, SQWORD>> vpairlPrimes, 
    vector<SQWORD> &sqDivisors,
    SQWORD sqCur)
{
    if (vpairlPrimes.empty()) {
        sqDivisors.emplace_back(sqCur);
        return;
    }

    auto prime = vpairlPrimes.back();
    vpairlPrimes.pop_back();
    SQWORD sqDiv = sqCur;

    for (SDWORD lPow = 0; lPow <= prime.second; lPow++) {
        listupDivisorsOne(vpairlPrimes, sqDivisors, sqDiv);
        sqDiv *= prime.first;
    }
}

static void listupDivisors(SQWORD sqNum, vector<SQWORD> &vecsqDivisors)
{
    vector<pair<SQWORD, SQWORD>> vpairlPrimes;

    calcPrimeFactorication(sqNum, vpairlPrimes);   

    listupDivisorsOne(vpairlPrimes, vecsqDivisors, 1);
}


int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_K = inputSQWORD();

    vector<SQWORD> vecsqA;

    SQWORD sqSum = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        vecsqA.emplace_back(sqA);
        sqSum += sqA;
    }

    vector<SQWORD> vecsqDivisors;
    listupDivisors(sqSum, vecsqDivisors);

    SQWORD sqAns = 0;
    sort(vecsqDivisors.begin(), vecsqDivisors.end(), greater<SQWORD>());
    for (auto divisor: vecsqDivisors) {
        if (isDividable(vecsqA, sqInput_K * 2, divisor)) {
            sqAns = divisor;
            break;
        }
    }

    printf("%lld\n", sqAns);
    return 0;
}