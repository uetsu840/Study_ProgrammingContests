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

static inline bool DoubleIsZero(const DOUBLE &a)
{
    return abs(a) < DOUBLE_EPS;
}

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
struct ST_AXIS_MOTION {
    SQWORD sqInit;
    SQWORD sqDir;
};

/**
 *  最大と最小を取り出してリストに格納する。
 */
static void putMinMax(
    vector<ST_AXIS_MOTION> &vecstTgt,
    vector<SQWORD> &vecsqPos,
    SQWORD sqDir)
{
    sort(vecsqPos.begin(), vecsqPos.end());
    if (0 < vecsqPos.size()) {
        auto it = vecsqPos.begin();
        vecstTgt.emplace_back(ST_AXIS_MOTION{*it, sqDir});
    }
    if (1 < vecsqPos.size()) {
        auto it_e = vecsqPos.end() - 1;
        vecstTgt.emplace_back(ST_AXIS_MOTION{*it_e, sqDir});
    }
}

/**
 *  入力を取得する
 */
static void getInput(
    vector<ST_AXIS_MOTION> &vecstX,
    vector<ST_AXIS_MOTION> &vecstY)
{
    SQWORD sqInput_N = inputSQWORD();
    char acDir[2];

    vector<SQWORD> vecsqX_R;
    vector<SQWORD> vecsqX_Stop;
    vector<SQWORD> vecsqX_L;
    vector<SQWORD> vecsqY_U;
    vector<SQWORD> vecsqY_Stop;
    vector<SQWORD> vecsqY_D;

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqInput_x = inputSQWORD();
        SQWORD sqInput_y = inputSQWORD();
        inputStringSpSeparated(acDir);

        switch (acDir[0]) {
        case 'U':
            vecsqX_Stop.emplace_back(sqInput_x);
            vecsqY_U.emplace_back(sqInput_y);
            break;
        case 'D':
            vecsqX_Stop.emplace_back(sqInput_x);
            vecsqY_D.emplace_back(sqInput_y);
            break;
        case 'R':
            vecsqX_R.emplace_back(sqInput_x);
            vecsqY_Stop.emplace_back(sqInput_y);
            break;
        case 'L':
            vecsqX_L.emplace_back(sqInput_x);
            vecsqY_Stop.emplace_back(sqInput_y);
            break;
        default:
            printf("oops!\n");
        }
    }

    /**
     * 最大値と最小値だけを抽出する 
     */
    /* X軸方向 */
    putMinMax(vecstX, vecsqX_R, 1);
    putMinMax(vecstX, vecsqX_Stop, 0);
    putMinMax(vecstX, vecsqX_L, -1);

    /* Y方向 */
    putMinMax(vecstY, vecsqY_U, 1);
    putMinMax(vecstY, vecsqY_Stop, 0);
    putMinMax(vecstY, vecsqY_D, -1); 
}

void pushCrossTime(
    vector<ST_AXIS_MOTION> &vecstMotion,
    vector<DOUBLE> &vecdCrossTime)
{
    for (auto it1 = vecstMotion.begin(); it1 != vecstMotion.end(); ++it1) {
        for (auto it2 = vecstMotion.begin(); it2 != vecstMotion.end(); ++it2) {
            if (it1->sqDir != it2->sqDir) {
                DOUBLE dCorssTime = (DOUBLE)(it2->sqInit - it1->sqInit) / (DOUBLE)(it1->sqDir - it2->sqDir);
                if (0.0 < dCorssTime) {
                    vecdCrossTime.emplace_back(dCorssTime);
                }
            }
        }
    }
}

DOUBLE getBoundingBoxMinMax(
    const vector<ST_AXIS_MOTION> &vecstAxis,
    DOUBLE dCrossTime) 
{
    DOUBLE dMax = MIN_DOUBLE_N;
    DOUBLE dMin = MAX_DOUBLE;
    for (auto pnt_ax: vecstAxis) {
        DOUBLE dPos = (DOUBLE)pnt_ax.sqInit + (DOUBLE)pnt_ax.sqDir * dCrossTime;
        dMax = MAX(dMax, dPos);
        dMin = MIN(dMin, dPos);
    }
    return (dMax - dMin);
}

DOUBLE getBoudingBox(
    const vector<ST_AXIS_MOTION> &vecstX,
    const vector<ST_AXIS_MOTION> &vecstY,
    DOUBLE dCrossTime)
{
    DOUBLE dWidthX = getBoundingBoxMinMax(vecstX, dCrossTime);
    DOUBLE dWidthY = getBoundingBoxMinMax(vecstY, dCrossTime);

//    printf("%f %f\n", dWidthX, dWidthY);

    return dWidthX * dWidthY;
}


int main(void)
{
    vector<ST_AXIS_MOTION> vecstX;
    vector<ST_AXIS_MOTION> vecstY;
    vector<DOUBLE> vecCrossTime;
    getInput(vecstX, vecstY);
    vecCrossTime.emplace_back(0.0);
    pushCrossTime(vecstX, vecCrossTime);
    pushCrossTime(vecstY, vecCrossTime);

    DOUBLE dMinBoudingBox = MAX_DOUBLE;
    for (auto time: vecCrossTime) {
        dMinBoudingBox = min(dMinBoudingBox, getBoudingBox(vecstX, vecstY, time));
    }
    printf("%.8f\n", dMinBoudingBox);

    return 0;
}