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

#define DIVISOR     (1000000007)

#define DOUBLE_EPS  (1.0e-15)
static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a;}

static inline bool doubleIsEqual(DOUBLE a, DOUBLE b)
{
    return ABS(a-b) < DOUBLE_EPS;
}

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
struct POSITION {
    SDWORD lX;
    SDWORD lY;
};

#define MAX_POINTS  (2000)
static POSITION s_astPosition[MAX_POINTS];

static void getAngleNum(
    SDWORD lPointIdx,
    SDWORD lNumPoints,
    SQWORD &sqDullAngleNum,
    SQWORD &sqRightAngleNum)
{
//    printf("points > [%d] %d %d\n", lPointIdx, s_astPosition[lPointIdx].lX, s_astPosition[lPointIdx].lY);

    vector<DOUBLE> vecdAngle;
    sqDullAngleNum = 0;
    sqRightAngleNum = 0;

    for (SDWORD lIdx = 0; lIdx < lNumPoints; lIdx++) {
        if (lIdx != lPointIdx) {
            POSITION stRelPos;
            SDWORD lRelX = s_astPosition[lIdx].lX - s_astPosition[lPointIdx].lX;
            SDWORD lRelY = s_astPosition[lIdx].lY - s_astPosition[lPointIdx].lY;

            DOUBLE dAngle = atan2((DOUBLE)lRelY, (DOUBLE)lRelX);
            vecdAngle.push_back(dAngle);
        }
    }
    sort(vecdAngle.begin(), vecdAngle.end());
    SQWORD sqRelNum = lNumPoints - 1;

    for (SDWORD lIdx = 0; lIdx < sqRelNum; lIdx++) {
        vecdAngle.push_back(vecdAngle[lIdx] + M_PI * 2);
    }

    SQWORD sqFirstIdx = 0;
    SQWORD sqLastIdx = 1;
    SQWORD sqKRAngleNum = 0;

    for (sqFirstIdx = 0; sqFirstIdx < sqRelNum; sqFirstIdx++) {
        while(1) {
            if (doubleIsEqual(vecdAngle[sqLastIdx] - (M_PI / 2.0), vecdAngle[sqFirstIdx])) {
                sqRightAngleNum++;
                sqLastIdx++;
            } else if (vecdAngle[sqLastIdx] < vecdAngle[sqFirstIdx] + (M_PI / 2.0)) {
                sqLastIdx++;
            } else {
                break;
            }
        }
//        printf("[%d %d]\n", lLastIdx, lFirstIdx);
        sqKRAngleNum += (sqLastIdx - sqFirstIdx - 1);
    }
    sqDullAngleNum = sqRelNum * (sqRelNum - 1) / 2 - sqKRAngleNum;
//    printf("%d | R:%d D:%d\n", lKRAngleNum,  lRightAngleNum, lDullAngleNum);
    return;
}



int main(void)
{
    SQWORD sqInput_N = inputSDWORD();

    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        SDWORD lInput_x = inputSDWORD();
        SDWORD lInput_y = inputSDWORD();

        s_astPosition[lIdx].lX = lInput_x;
        s_astPosition[lIdx].lY = lInput_y;
    }

    SQWORD sqRightAngleNumTtl = 0;
    SQWORD sqDullANgleNumTtl = 0;

    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        SQWORD sqRightAngleNum;
        SQWORD sqDullAngleNum;
        getAngleNum(lIdx, sqInput_N, sqDullAngleNum, sqRightAngleNum);

        sqRightAngleNumTtl += sqRightAngleNum;
        sqDullANgleNumTtl  += sqDullAngleNum;
    }

    SQWORD sqTriangleTtl = (sqInput_N * (sqInput_N - 1) * (sqInput_N - 2)) / 6;
    SQWORD lKeenAngleTriNum = sqTriangleTtl - sqRightAngleNumTtl - sqDullANgleNumTtl;

    printf("%lld %lld %lld\n", lKeenAngleTriNum, sqRightAngleNumTtl, sqDullANgleNumTtl);
    return 0;
}