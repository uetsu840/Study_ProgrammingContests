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
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }

static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }

static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }

static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; }
static inline bool DoubleIsZero(const DOUBLE &a)
{
    return ABS(a) < DOUBLE_EPS;
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
//#define N_ROW   (3)
//#define N_COL   (3)

#define N_ROW   (5)
#define N_COL   (5)
#define MAX_NUM (N_ROW * N_COL)
static SDWORD s_aalNumbers[N_ROW][N_COL];


static inline bool checkBoard(SQWORD sqBitMap, SQWORD sqPos)
{
    SQWORD sqRowIdx = sqPos / N_COL;
    SQWORD sqColIdx = sqPos % N_COL;

    if ((0 < sqRowIdx) && (sqRowIdx < (N_ROW - 1))) {
        bool bExistT = sqBitMap & (0x1 << (sqPos + N_COL));
        bool bExistB = sqBitMap & (0x1 << (sqPos - N_COL));
        if (bExistT && (!bExistB)) {
            return false;
        }
        if ((!bExistT) && bExistB) {
            return false;
        }
    }
    if ((0 < sqColIdx) && (sqColIdx < (N_COL - 1))) {
        bool bExistL = sqBitMap & (0x1 << (sqPos + 1));
        bool bExistR = sqBitMap & (0x1 << (sqPos - 1));
        if (bExistL && (!bExistR)) {
            return false;
        }
        if ((!bExistL) && bExistR) {
            return false;
        }
    }

    return true;
}

static SDWORD bitCount (SDWORD lVal) {
    SDWORD lRet = 0;
    while (0 < lVal) {
        lRet += (lVal & 1);
        lVal >>= 1;
    }
    return lRet;
}

int main(void)
{
    SDWORD alFixedPos[MAX_NUM + 1];
    bool abFixedNum[MAX_NUM + 1];
    for (SQWORD sqIdx = 0; sqIdx < ArrayLength(alFixedPos); sqIdx++) {
        alFixedPos[sqIdx] = -1;
        abFixedNum[sqIdx] = false;
    }

    for (SDWORD lRow = 0; lRow < N_ROW; lRow++) {
        for (SDWORD lCol = 0; lCol < N_COL; lCol++) {
            SDWORD lX = inputSDWORD();
            s_aalNumbers[lRow][lCol] = lX;
            if (0 != lX) {
                alFixedPos[lX] = lRow * N_COL + lCol;
                abFixedNum[lRow * N_COL + lCol] = true;
            }
        }
    }

    /**
     *  1から順に置いてゆく 
     *      dp[i][j]    i番目までを置いたとき、盤面の状態がj (25bit) に対応する場合の数
     */
    #define DPTBL_SIZE  ((0x1 << MAX_NUM) - 1)
    static vector<SDWORD> veclDp(DPTBL_SIZE);

    veclDp[0] = 1;
    for (SQWORD sqBitMap = 0; sqBitMap < veclDp.size(); sqBitMap++) {
        if (0 < veclDp[sqBitMap]) {
            SDWORD lNum = bitCount(sqBitMap) + 1;
            if (-1 != alFixedPos[lNum]) {
                /* 場所が固定 */
                if (checkBoard(sqBitMap, alFixedPos[lNum])) {
                    veclDp[sqBitMap | (0x1 << alFixedPos[lNum])] = addMod(veclDp[sqBitMap | (0x1 << alFixedPos[lNum])],
                                                                        veclDp[sqBitMap]);
                }
            } else {
                /* 置ける場所を探す */
                for (SQWORD sqPos = 0; sqPos < MAX_NUM; sqPos++) {
                    if ((!abFixedNum[sqPos]) && !(sqBitMap & (1 << sqPos))) {
                        if (checkBoard(sqBitMap, sqPos)) {                            
                            veclDp[sqBitMap | (0x1 << sqPos)] = addMod(veclDp[sqBitMap | (0x1 << sqPos)], 
                                                                        veclDp[sqBitMap]);
                        }
                    }
                }
            }
        }
    }

    printf("%lld\n", veclDp[(0x1 << MAX_NUM) - 1]);
}