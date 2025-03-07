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

/**
 *  ・街 12個
 *  ・宝箱 5個
 *  ・街 + 宝箱 17個
 * 
 *   街番号n: 100n + 32  (状態)
 *   宝箱m:   100m + 50 + 32(状態)
 *   
 *  2^5 = 32
 */
struct POSITION {
    SQWORD sqX;
    SQWORD sqY;

    POSITION (SQWORD x, SQWORD y): sqX(x), sqY(y) {};
};

vector<SQWORD> vsqPopCount(32);


static DOUBLE getBoostRate(SQWORD sqIdx, SQWORD sqN)
{
    return pow(2.0, (vsqPopCount[sqIdx >> sqN]));
}


/* 距離を計算する */
DOUBLE getDistance(const POSITION &a, const POSITION &b)
{
    SQWORD sqDx = a.sqX - b.sqX;
    SQWORD sqDy = a.sqY - b.sqY;

    return sqrt((DOUBLE)sqDx * (DOUBLE)sqDx + (DOUBLE)sqDy * (DOUBLE)sqDy);
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<POSITION> vstPoints;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vstPoints.emplace_back(sqX, sqY);
    }

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vstPoints.emplace_back(sqX, sqY);
    }

    for (SQWORD sqIdx = 0; sqIdx < 32; sqIdx++) {
        SQWORD sqPopCount = __builtin_popcount(sqIdx);
        vsqPopCount[sqIdx] = sqPopCount;
    }

    SQWORD sqDpTblSize = (0x1 << (sqN + sqM));
    vector<vector<DOUBLE>> vdDpTbl(sqN + sqM, vector<DOUBLE>(sqDpTblSize, 1.0e+255));

    /* テーブルの初期化で、原点からの距離を入れておく */
    for (SQWORD sqDpToBit = 0; sqDpToBit < sqN + sqM; sqDpToBit++) {
        SQWORD sqDpTo = 0x1 << sqDpToBit;
        vdDpTbl[sqDpToBit][sqDpTo] = getDistance(vstPoints[sqDpToBit], POSITION(0, 0));
//        printf("===%d %d %0.3f\n", vstPoints[sqDpToBit].sqX, vstPoints[sqDpToBit].sqY, vdDpTbl[sqDpToBit][sqDpTo]);
    }

    /* DP */
    for (SQWORD sqDpFrom = 0; sqDpFrom < sqDpTblSize; sqDpFrom++) {
        for (SQWORD sqDpFromBit = 0; sqDpFromBit < sqN + sqM; sqDpFromBit++) {
            DOUBLE dBoost = getBoostRate(sqDpFrom, sqN);
            for (SQWORD sqDpToBit = 0; sqDpToBit < sqN + sqM; sqDpToBit++) {
                if ((sqDpFrom & (0x1 << sqDpFromBit))
                    && (!(sqDpFrom & (0x1 << sqDpToBit)))) {
                    SQWORD sqDpTo = sqDpFrom | (0x1 << sqDpToBit);
                    DOUBLE dNewCost = vdDpTbl[sqDpFromBit][sqDpFrom] + getDistance(vstPoints[sqDpFromBit], vstPoints[sqDpToBit]) / dBoost;
//                    printf("===>>>%d %d %d %0.3f %0.3f\n", sqDpFrom, sqDpFromBit, sqDpToBit, vdDpTbl[sqDpFromBit][sqDpFrom], dNewCost);
                    if (dNewCost < vdDpTbl[sqDpToBit][sqDpTo]) {
                        vdDpTbl[sqDpToBit][sqDpTo] = dNewCost;
                    }
                }
            }
        }
    }

    /* 原点まで戻る */ 
    DOUBLE dAns = 1.0e+255;
    SQWORD sqFullBit = (0x1 << sqN) - 1;
    for (SQWORD sqDpFromBit = 0; sqDpFromBit < sqN; sqDpFromBit++) {
//        printf("city %d\n", sqDpFromBit);
        for (SQWORD sqBoostBit = 0; sqBoostBit < (0x1 << sqM); sqBoostBit++) {
            SQWORD sqDpFrom = ((0x1 << sqN) - 1) | (sqBoostBit << sqN);
            DOUBLE dBoost = getBoostRate(sqDpFrom, sqN);
//            printf("F: %08x b: %1.0f c:%0.3f \n", sqDpFrom, dBoost, vdDpTbl[sqDpFromBit][sqDpFrom]);
            DOUBLE dCost =  vdDpTbl[sqDpFromBit][sqDpFrom] + getDistance(vstPoints[sqDpFromBit], POSITION(0, 0)) / dBoost; 
            if (dCost < dAns) {
                dAns = dCost;
            }
        }
    }
    printf("%0.10f\n", dAns);


    return 0;
}
