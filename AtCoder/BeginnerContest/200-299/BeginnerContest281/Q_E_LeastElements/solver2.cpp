#pragma GCC optimize ("O3")
using namespace std;

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
#include <cstdint>
#include <stack>

using QWORD  = uint64_t;
using SQWORD = int64_t;
using DWORD  = uint32_t;
using SDWORD = int32_t;
using WORD   = uint16_t;
using SWORD  = int16_t;
using BYTE   = uint8_t;
using SBYTE  = int8_t;
using DOUBLE = double;
using LDOUBLE = long double;
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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}
static inline SQWORD DIV_UP(SQWORD a, SQWORD x) {return (a + (x - 1)) / x;}

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

/*----------------------------------------------*/

/*----------------------------------------------*/


int main(void)
{
    map<SQWORD, SQWORD> mapSums;
    map<SQWORD, SQWORD> mapReserve;

    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqK = inputSQWORD();
    vector<SQWORD> vsqA;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vsqA.emplace_back(inputSQWORD());
    }

    SQWORD sqCurSum = 0;
    vector<SQWORD> vsqAns;

    /* initialize */
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        mapReserve[vsqA[sqIdx]]++;
    }
    for (SQWORD sqCnt = 0; sqCnt < sqK; sqCnt++) {
        auto it = mapReserve.begin();
        SQWORD sqCount = it->second;
        SQWORD sqNum = it->first;
        mapSums[sqNum]++;
        if (1 < mapReserve[sqNum]) {
            mapReserve[sqNum]--;
        } else {
            mapReserve.erase(it);
        }
        sqCurSum += sqNum;
    }
    vsqAns.emplace_back(sqCurSum);

    /* add */
    for (SQWORD sqAnsIdx = sqM; sqAnsIdx < sqN; sqAnsIdx++) {
        SQWORD sqAddA = vsqA[sqAnsIdx];
        SQWORD sqRemoveA = vsqA[sqAnsIdx - sqM];

        auto removeit = mapSums.find(sqRemoveA);

        if (removeit == mapSums.end()) {
//            printf("m");
            /* 外れたやつがもともと和に入ってない */
            mapReserve[sqRemoveA]--;
            if (0 == mapReserve[sqRemoveA]) {
                mapReserve.erase(sqRemoveA);
            }
            auto itSumEnd = mapSums.rbegin();
            SQWORD sqSumMax = itSumEnd->first;
            if (sqSumMax < sqAddA) {
                mapReserve[sqAddA]++;
            } else {
                mapReserve[sqSumMax]++;
                mapSums[sqSumMax]--;
                if (0 == mapSums[sqSumMax]) {
                    mapSums.erase(sqSumMax);
                }
                mapSums[sqAddA]++;
                sqCurSum += (sqAddA - sqSumMax);
            }
        } else {
            /* 外れたやつが和に含まれてた */
            sqCurSum -= sqRemoveA;
            if (mapReserve.size() == 0) {
                /* 特殊パターン： 全部使ってる */
//                printf(">");
                mapSums[sqRemoveA]--;
                if (0 == mapSums[sqRemoveA]) {
                    mapSums.erase(sqRemoveA);
                }
                mapSums[sqAddA]++;
                sqCurSum += sqAddA;
            } else {
                /* 通常パターン */
                mapSums[sqRemoveA]--;
                if (0 == mapSums[sqRemoveA]) {
                    mapSums.erase(sqRemoveA);
                }
                auto itResBegin = mapReserve.begin();
                SQWORD sqReserveMin = itResBegin->first;
                if (sqReserveMin < sqAddA) {
                    mapSums[sqReserveMin]++;
                    mapReserve[sqReserveMin]--;
                    if (0 == mapReserve[sqReserveMin]) {
                        mapReserve.erase(sqReserveMin);
                    }
                    mapReserve[sqAddA]++;
                    sqCurSum += sqReserveMin;
                } else {
                    mapSums[sqAddA]++;
                    sqCurSum += sqAddA;
                }            
            }
        }
        vsqAns.emplace_back(sqCurSum);
    }

    for (auto a: vsqAns) {
        printf("%lld ", a);
    }
    printf("\n");


    return 0;
}
