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

#define CELL_NONE       (10000)
#define CELL_D          (0)
#define CELL_N          (1)

#define SDWORD_INF      (1000000000)

static SDWORD s_aalScoreB[2][3];
static SDWORD s_aalScoreC[3][2];

static SDWORD getScore(
    const SDWORD aalCells[3][3])
{
    SDWORD lScore = 0;
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            if (aalCells[lRow][lCol] == aalCells[lRow+1][lCol]) {
                lScore += s_aalScoreB[lRow][lCol];
            }
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            if (aalCells[lRow][lCol] == aalCells[lRow][lCol+1]) {
                lScore += s_aalScoreC[lRow][lCol];
            }
        }
    }

//    printf("s: %d\n", lScore);

    return lScore;
}

#define N_ROWS  (3)
#define N_COLS  (3)

/**
 *  calc scores
 */
static SDWORD execSingleStep(
    SDWORD aalCells[3][3],
    SDWORD lTurn)
{
    /* filled all cells */
    if (9 == lTurn) {
        return getScore(aalCells);
    }

    /* exec search */
    SDWORD lCell;
    SDWORD lRsltScore;
    if ((lTurn % 2) == 0) {
        lRsltScore = 0;
        for (SDWORD lRow = 0; lRow < N_ROWS; lRow++) {
            for (SDWORD lCol = 0; lCol < N_COLS; lCol++) {
                if (CELL_NONE == aalCells[lRow][lCol]) {
                    aalCells[lRow][lCol] = CELL_D;
                    lRsltScore = max(lRsltScore, execSingleStep(aalCells, lTurn + 1));
                    aalCells[lRow][lCol] = CELL_NONE;
                }
            }
        }
    } else {
        lRsltScore = SDWORD_INF;
        for (SDWORD lRow = 0; lRow < N_ROWS; lRow++) {
            for (SDWORD lCol = 0; lCol < N_COLS; lCol++) {
                if (CELL_NONE == aalCells[lRow][lCol]) {
                    aalCells[lRow][lCol] = CELL_N;
                    lRsltScore = min(lRsltScore, execSingleStep(aalCells, lTurn + 1));
                    aalCells[lRow][lCol] = CELL_NONE;
                }
            }
        }
    }


    return lRsltScore;
}



int main(void)
{
    SDWORD lSum = 0;
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            SDWORD lScore = inputSDWORD();
            s_aalScoreB[lRow][lCol] = lScore;
            lSum += lScore;
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            SDWORD lScore = inputSDWORD();
            s_aalScoreC[lRow][lCol] = lScore;
            lSum += lScore;
        }
    }

    SDWORD aalCells[N_ROWS][N_COLS];
    for (SDWORD lRowIdx = 0; lRowIdx < N_ROWS; lRowIdx++) {
        for (SDWORD lColIdx = 0; lColIdx < N_COLS; lColIdx++) {
            aalCells[lRowIdx][lColIdx] = CELL_NONE;
        }
    }

    SDWORD lScoreD, lScoreN;
    lScoreD = execSingleStep(aalCells, 0);
    lScoreN = lSum - lScoreD;

    printf("%d\n", lScoreD);
    printf("%d\n", lScoreN);

    return 0;
}