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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }

static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; }

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

struct ST_SECTION {
    SQWORD sqT;
    SQWORD sqV;
//    ST_SECTION(SQWORD t, SQWORD v) : sqT(t), sqV(v) {};
};

struct ST_SPEED {
    SQWORD sqBegin;
    SQWORD sqEnd;
};

int main()
{
    SQWORD sqN = inputSQWORD();

    vector<ST_SECTION> vstSection(sqN);
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqT = inputSQWORD();
        vstSection[sqIdx].sqT = sqT;
    }
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqV = inputSQWORD();
        vstSection[sqIdx].sqV = sqV;
    }

    vector<ST_SPEED> vstSpeed(sqN);
    SQWORD sqCurV = 0;
    for (SQWORD sqSection = 0; sqSection < sqN; sqSection++) {
        SQWORD sqT = vstSection[sqSection].sqT;
        SQWORD sqV = vstSection[sqSection].sqV;

        vstSpeed[sqSection].sqBegin = MIN(sqV, sqCurV);
        vstSpeed[sqSection].sqEnd = MIN(sqV, sqCurV + sqT);

        sqCurV = vstSpeed[sqSection].sqEnd;
    }

    sqCurV = 0;
    for (SQWORD sqSection = sqN - 1; 0 <= sqSection; sqSection--) {
        SQWORD sqT = vstSection[sqSection].sqT;
        SQWORD sqV = vstSection[sqSection].sqV;

        vstSpeed[sqSection].sqEnd = MIN(MIN(sqV, sqCurV), vstSpeed[sqSection].sqEnd);
        vstSpeed[sqSection].sqBegin = MIN(MIN(sqV, sqCurV + sqT), vstSpeed[sqSection].sqBegin);

        sqCurV = vstSpeed[sqSection].sqBegin;
    }

    DOUBLE dAns = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        DOUBLE dSpedBegin = vstSpeed[sqIdx].sqBegin;
        DOUBLE dSpedEnd = vstSpeed[sqIdx].sqEnd;
        DOUBLE dLimit = vstSection[sqIdx].sqV;
        DOUBLE dTime = vstSection[sqIdx].sqT;

        DOUBLE dTopSped = ((dTime - ABS(dSpedBegin - dSpedEnd)) / 2.0 + max(dSpedBegin, dSpedEnd));
        dTopSped = MIN(dTopSped, dLimit);

        DOUBLE dAccTime = dTopSped - dSpedBegin;
        DOUBLE dDecTime = dTopSped - dSpedEnd;
        DOUBLE dCnstTime = dTime - dAccTime - dDecTime;


        DOUBLE dDist = (dSpedBegin + dTopSped) * dAccTime / 2.0;
        dDist += (dSpedEnd + dTopSped) * dDecTime / 2.0;
        dDist += (dCnstTime) * dTopSped;

        dAns += dDist;
    }

    printf("%0.10f\n", dAns);
    return 0;
}