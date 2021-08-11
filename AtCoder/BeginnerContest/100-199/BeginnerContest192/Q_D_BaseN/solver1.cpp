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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
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

struct PROBLEM_ONE {
    string strS;
    SQWORD sqM;
};

#define MAX_M   ((SQWORD)1000000000000000000)

static bool isBaseNLower(SQWORD sqBase, const PROBLEM_ONE*pstProb)
{
    string strSCur(pstProb->strS.rbegin(), pstProb->strS.rend());
    SQWORD sqMaxCurNumber = (MAX_M * 2) / sqBase;

    SQWORD sqCurNumber = 1;
    SQWORD sqSum = 0;
    for(SQWORD sqDigit = 0; sqDigit < strSCur.size(); ++sqDigit) {
        SQWORD sqNum = strSCur[sqDigit] - '0';
        sqSum += sqNum * sqCurNumber;
        if ((sqMaxCurNumber < sqCurNumber) && (sqDigit + 1 != strSCur.size())) {
            return false;
        }
        sqCurNumber *= sqBase;
    }
    if (!(sqSum <= pstProb->sqM)) {
        return false;
    }
    return true;
}

static SQWORD binarySearch(
    bool (*pfJudge)(SQWORD, const PROBLEM_ONE*),
    SQWORD sqInitLower, 
    SQWORD sqInitUpper, 
    const PROBLEM_ONE *pstParam)
{
    SQWORD sqOk = sqInitLower;
    SQWORD sqNg = sqInitUpper;

    while (1 < sqNg - sqOk) {
        SQWORD sqMid = (sqNg + sqOk) / 2LL;
//        printf("judge %lld\n", sqMid);
        if (pfJudge(sqMid, pstParam)) {
            sqOk = sqMid;
        } else {
            sqNg = sqMid;
        }
    }
    return sqOk;
}

int main(void)
{
    PROBLEM_ONE stProb;
    cin >> stProb.strS;
    stProb.sqM = inputSQWORD();

    SQWORD sqD = 0;
    for(SQWORD sqDigit = 0; sqDigit <stProb.strS.size(); ++sqDigit) {
        sqD = MAX(sqD, (SQWORD)(stProb.strS[sqDigit] - '0'));
    }

    if (1 == stProb.strS.size()) {
        SQWORD sqNum = atoi(stProb.strS.c_str());
        if (stProb.sqM < sqNum) {
            printf("0\n");
        } else {
            printf("1\n");
        }
    } else {
        /* 2桁以上は基数が違うと必ず数値が違う */
        SQWORD sqUpperBound = binarySearch(isBaseNLower, 1,  MAX_M + 1LL, &stProb);
        if (sqUpperBound < sqD) {
            printf("0\n");
        } else {
            printf("%lld\n", sqUpperBound - sqD);
        }
    }

    return 0;
}

