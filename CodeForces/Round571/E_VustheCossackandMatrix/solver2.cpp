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
#define N_MAX_ROWS      (1000)
#define N_MAX_COLS      (1000)

static SDWORD s_aalMatrixOrg[N_MAX_ROWS + 1][N_MAX_COLS + 1];
static SQWORD s_aasqMatrixSum[N_MAX_ROWS + 1][N_MAX_COLS + 1];
static SQWORD s_sqUnitCount = 0;
static SQWORD s_sqDoubleCount;

/**
 *  get input matrix
 *  calc cumsum
 */
static void inputMatrixAndGetCumSum(SQWORD sqInput_n, SQWORD sqInput_m)
{
    for (SQWORD sqRowNo = 1; sqRowNo <= sqInput_n; sqRowNo++) {
        string strRow;
        cin >> strRow;

        for (SQWORD sqColNo = 1; sqColNo <= sqInput_m; sqColNo++) {
            if ('0' == strRow[sqColNo - 1]) {
                s_aalMatrixOrg[sqRowNo][sqColNo] = 0;
            } else {
                s_aalMatrixOrg[sqRowNo][sqColNo] = 1;
            }
        } 
    }

    for (SQWORD sqRowNo = 1; sqRowNo <= sqInput_n; sqRowNo++) {
        SQWORD sqRowSum = 0;
        for (SQWORD sqColNo = 1; sqColNo <= sqInput_m; sqColNo++) {
            sqRowSum += s_aalMatrixOrg[sqRowNo][sqColNo];
            s_aasqMatrixSum[sqRowNo][sqColNo] = s_aasqMatrixSum[sqRowNo - 1][sqColNo] + sqRowSum;
//            printf("%lld ", s_aasqMatrixSum[sqRowNo][sqColNo]);
        }
//        printf("\n");
    }

    s_sqUnitCount = s_aasqMatrixSum[sqInput_n][sqInput_m];
    s_sqDoubleCount = sqInput_n * sqInput_m;
}

static void divideCells(
    SQWORD sqCellNum,
    SQWORD sqBase,
    SQWORD &sqPerfect,
    SQWORD &sqRemain)
{
    SQWORD sqRest = sqCellNum;

    sqPerfect = sqRest / sqBase;
    sqRest -= sqPerfect * sqBase;

    sqRemain = sqRest;
}

static SDWORD bitCount(SQWORD sqNum)
{
    SDWORD lCount = 0;
    for (QWORD qwCur = sqNum; qwCur != 0; qwCur>>=1) {
        if (qwCur & 0x1) {
            lCount++;
        }
    }
    return lCount;
}

static bool isNormalCells(SQWORD sqRowIdx, SQWORD sqColIdx)
{
    SDWORD lRowState, lColState;
    lRowState = bitCount(sqRowIdx) % 2;
    lColState = bitCount(sqColIdx) % 2;

    if (0 == (lRowState ^ lColState)) {
        return true;
    } else {
        return false;
    }
}

/**
 *  count f(p, q)
 */
static SQWORD CountNumber(
    SQWORD sqRowNo, 
    SQWORD sqColNo,
    SQWORD sqInput_n,
    SQWORD sqInput_m)
{
    SQWORD sqPerfectRow, sqPerfectCol, sqRemainRow, sqRemainCol;

    divideCells(sqRowNo, sqInput_n, sqPerfectRow, sqRemainRow);
    divideCells(sqColNo, sqInput_m, sqPerfectCol, sqRemainCol);

    SQWORD sqOneCount = 0;

    sqOneCount += s_sqDoubleCount * (sqPerfectRow * sqPerfectCol / 2);
    if (0 < ((sqPerfectRow * sqPerfectCol) % 2)) {
        if (isNormalCells(sqPerfectRow - 1, sqPerfectCol - 1)) {
            sqOneCount += s_sqUnitCount;
        } else {
            sqOneCount += (s_sqDoubleCount - s_sqUnitCount);
        }
    }
//    printf("<%lld %lld %lld>\n", sqPerfectRow, sqPerfectCol, sqOneCount);

    sqOneCount += (sqRemainRow * (sqPerfectCol / 2) * sqInput_m);
    if (0 < sqPerfectCol % 2) {
        if (isNormalCells(sqPerfectRow, sqPerfectCol - 1)) {
            /* a */
            sqOneCount += s_aasqMatrixSum[sqRemainRow][sqInput_m];
        } else {
            /* ^a */
            sqOneCount += (sqRemainRow * sqInput_m - s_aasqMatrixSum[sqRemainRow][sqInput_m]);
        }
    }

    sqOneCount += (sqRemainCol * (sqPerfectRow / 2) * sqInput_n);
    if (0 < sqPerfectRow % 2) {
        if (isNormalCells(sqPerfectRow - 1, sqPerfectCol)) {
            /* a */
            sqOneCount += s_aasqMatrixSum[sqInput_n][sqRemainCol];
        } else {
            /* ^a */
            sqOneCount += (sqInput_n * sqRemainCol - s_aasqMatrixSum[sqInput_n][sqRemainCol]);
        }
    }

    if (isNormalCells(sqPerfectRow, sqPerfectCol)) {
        sqOneCount += s_aasqMatrixSum[sqRemainRow][sqRemainCol];
    } else {
        sqOneCount += sqRemainRow * sqRemainCol - s_aasqMatrixSum[sqRemainRow][sqRemainCol];
    }

//    printf("=== %lld\n", sqOneCount);

    return sqOneCount;
}


int main(void)
{
    SQWORD sqInput_n = inputSQWORD();
    SQWORD sqInput_m = inputSQWORD();
    SQWORD sqInput_q = inputSQWORD();

    inputMatrixAndGetCumSum(sqInput_n, sqInput_m);

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqInput_q; sqQueryIdx++) {
        SQWORD sqInput_x1 = inputSQWORD() - 1;
        SQWORD sqInput_y1 = inputSQWORD() - 1;
        SQWORD sqInput_x2 = inputSQWORD();
        SQWORD sqInput_y2 = inputSQWORD();

        SQWORD S1 = CountNumber(sqInput_x1, sqInput_y1, sqInput_n, sqInput_m);
        SQWORD S2 = CountNumber(sqInput_x1, sqInput_y2, sqInput_n, sqInput_m);
        SQWORD S3 = CountNumber(sqInput_x2, sqInput_y1, sqInput_n, sqInput_m);
        SQWORD S4 = CountNumber(sqInput_x2, sqInput_y2, sqInput_n, sqInput_m);

        printf("%lld\n", S4 - S2- S3 + S1);
    }

    return 0;
}