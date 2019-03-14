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

#define     N_MAX_SHRINES   (100000)
#define     N_MAX_TEMPLES   (100000)
#define     DIST_INF        (1000000000000)


static void getDist(
    SQWORD sqPosFrom,
    vector<SQWORD> &sqPosTo, 
    SDWORD &lIdxU, 
    SQWORD &sqDistW,
    SQWORD &sqDistE)
{
    auto itNearestTo = upper_bound(sqPosTo.begin(), sqPosTo.end(), sqPosFrom);
    lIdxU = itNearestTo - sqPosTo.begin();

    if (itNearestTo == sqPosTo.begin()) {
        sqDistW = DIST_INF;
    } else {
        sqDistW = sqPosFrom - *(itNearestTo - 1);
    }
    if (itNearestTo == sqPosTo.end()) {
        sqDistE = DIST_INF;
    } else {
        sqDistE = *itNearestTo - sqPosFrom;
    }

    //printf("F:%lld %lld\n", sqDistW, sqDistE);
}

static void getNearestAll(vector<SQWORD> &vecPosFrom, vector<SQWORD> &vecPosTo, vector<SQWORD> &vecDist)
{
    SDWORD lIdxU;

    for (DWORD dwIdx = 0; dwIdx < vecPosFrom.size(); dwIdx++) {
        SQWORD sqPosFrom = vecPosFrom[dwIdx];
        SQWORD sqDistE, sqDistW;
        getDist(sqPosFrom, vecPosTo, lIdxU, sqDistW, sqDistE);
        //printf("s:%lld %lld\n", sqDistW, sqDistE);
        vecDist[dwIdx] = min(sqDistW, sqDistE);
    }
}


static SQWORD getDistFromIdx(vector<SQWORD> &vecDist, SDWORD lIdx)
{
    if (lIdx < 0) {
        return DIST_INF;
    }
    if (vecDist.size() <= lIdx) {
        return DIST_INF;
    }

    return vecDist.at(lIdx);
}


int main()
{
    DWORD   dwNumShrine;
    DWORD   dwNumTemple;
    DWORD   dwNumAns;
    static vector<SQWORD> vecShrines;
    static vector<SQWORD> vecTemples;
    static vector<SQWORD> vecDistShrineFromTemple;
    static vector<SQWORD> vecDistTempleFromShrine;

    dwNumShrine = inputSDWORD();
    dwNumTemple = inputSDWORD();
    dwNumAns = inputSDWORD();

    vecDistShrineFromTemple.resize(dwNumTemple);
    vecDistTempleFromShrine.resize(dwNumShrine);

    for (DWORD dwIdx = 0; dwIdx < dwNumShrine; dwIdx++) {
        SQWORD sqShrine = inputSQWORD();
        vecShrines.emplace_back(sqShrine);
    }
    for (DWORD dwIdx = 0; dwIdx < dwNumTemple; dwIdx++) {
        SQWORD sqTemple = inputSQWORD();
        vecTemples.emplace_back(sqTemple);
    }

    sort(vecShrines.begin(), vecShrines.end());
    sort(vecTemples.begin(), vecTemples.end());

    /* get nearest temples and shrines */
    getNearestAll(vecShrines, vecTemples, vecDistTempleFromShrine);
    getNearestAll(vecTemples, vecShrines, vecDistShrineFromTemple);

    /* calc nearest distance */
    for (DWORD dwIdx = 0; dwIdx < dwNumAns; dwIdx++) {
        SQWORD sqFrom = inputSQWORD();
        DWORD dwShrineIdx, dwTempleIdx;
        SQWORD sqDistE, sqDistW;
        SDWORD lIdxU;

        getDist(sqFrom, vecShrines, lIdxU, sqDistW, sqDistE);
        SQWORD sqDistST1 = sqDistE + getDistFromIdx(vecDistTempleFromShrine, lIdxU);
        SQWORD sqDistST2 = sqDistW + getDistFromIdx(vecDistTempleFromShrine, lIdxU - 1);

        getDist(sqFrom, vecTemples, lIdxU, sqDistW, sqDistE);
        SQWORD sqDistTS1 = sqDistE + getDistFromIdx(vecDistShrineFromTemple, lIdxU);
        SQWORD sqDistTS2 = sqDistW + getDistFromIdx(vecDistShrineFromTemple, lIdxU - 1);

        SQWORD sqAns = DIST_INF;
        sqAns = min(sqAns, sqDistST1);
        sqAns = min(sqAns, sqDistST2);
        sqAns = min(sqAns, sqDistTS1);
        sqAns = min(sqAns, sqDistTS2);
        printf("%lld\n", sqAns);
    }
    return 0;
}