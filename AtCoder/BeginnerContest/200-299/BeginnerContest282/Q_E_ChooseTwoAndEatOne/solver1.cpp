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

SQWORD power_mod(SQWORD x, SQWORD y, SQWORD div)
{
    if (y == 0) {
        return 1;
    } else if (y == 1) {
        return x % div;
    } else if ((y % 2) == 0) {
        DWORD tmp = power_mod(x, y/2, div);
        return (tmp * tmp) % div;
    } else {
        DWORD tmp = power_mod(x, y/2, div);
        return ((tmp * tmp) * x) % div;
    }
}

/*----------------------------------------------*/

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<SQWORD> vsqA;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vsqA.emplace_back(inputSQWORD());
    }

    map<pair<SQWORD, SQWORD>, SQWORD> mapScores;
    vector<SQWORD> vsqMaxTos(sqN);
    for (SQWORD sqFrom = 0; sqFrom < sqN; sqFrom++) {
        SQWORD sqMaxScore = -1;
        SQWORD sqMaxTo = -1;
        for (SQWORD sqTo = 0; sqTo < sqN; sqTo++) {
            if (sqFrom != sqTo) {
                SQWORD sqScore = (power_mod(vsqA[sqFrom], vsqA[sqTo], sqM) 
                                   + power_mod(vsqA[sqTo], vsqA[sqFrom], sqM)) % sqM;
//                printf("--- %lld %lld %lld\n", sqFrom, sqTo, sqScore);
                if (sqMaxScore < sqScore) {
                    sqMaxTo = sqTo;
                    sqMaxScore = sqScore;
                }
                if (sqFrom < sqTo) {
                    mapScores[make_pair(sqFrom, sqTo)] = sqScore;
                }
            }
        }
        vsqMaxTos[sqFrom] = sqMaxTo;
    }

    SQWORD sqAns = 0;
    SQWORD sqCnt = 0;
    for (SQWORD sqFrom = 0; sqFrom < sqN; sqFrom++) {
        SQWORD sqTo = vsqMaxTos[sqFrom];

        SQWORD sqSrc = MIN(sqFrom, sqTo);
        SQWORD sqDst = MAX(sqFrom, sqTo);

        if (mapScores.find(make_pair(sqSrc, sqDst)) != mapScores.end()) {
//            printf("%lld %lld (%lld)\n", sqSrc, sqDst, mapScores[make_pair(sqSrc, sqDst)]);
            sqAns += mapScores[make_pair(sqSrc, sqDst)];
            sqCnt++;
            mapScores.erase(mapScores.find(make_pair(sqSrc, sqDst)));
        }
    }

    vector<SQWORD> vsqRestScores;
    for (auto s: mapScores) {
        vsqRestScores.emplace_back(s.second);
    }
    sort(vsqRestScores.begin(), vsqRestScores.end());
    reverse(vsqRestScores.begin(), vsqRestScores.end());

    printf(">>> %lld\n", sqCnt);
    for (auto r: vsqRestScores) {
        if (sqN - 1 <= sqCnt) {
            break;
        }
        printf("%lld\n", r);
        sqCnt++;
        sqAns += r;
    }

    printf("%lld\n", sqAns);
    return 0;
}
