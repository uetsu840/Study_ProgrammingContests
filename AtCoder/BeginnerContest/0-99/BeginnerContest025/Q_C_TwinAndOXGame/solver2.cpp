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


/**
 * cellState 
 *   CELL_D: 直大
 *   CELL_N: 直子
 *   CELL_NONE: 置かれていない
 */

typedef struct  {
    SDWORD lD;
    SDWORD lN;

    void SCORES(SDWORD d, SDWORD n) {
        lD = d;
        lN = n;
    }
} SCORES;

static vector<vector<SDWORD>> s_vvlScoreB(3, vector<SDWORD>(3, 0));
static vector<vector<SDWORD>> s_vvlScoreC(3, vector<SDWORD>(3, 0));

#define SDWORD_INF          (1000000000)
#define MAX_MEMO_TBL_SIZE   (1024 * 1024)       /* 4^9 ＝ 2^9 * 2^9 ≒ 1024 × 1024 */

static vector<bool> vbResultValid(MAX_MEMO_TBL_SIZE, false);
static vector<SCORES> vlResultMemo(MAX_MEMO_TBL_SIZE, SCORES{0, 0});

static SDWORD getMemoIdx(const vector<vector<SDWORD>> vvlCells)
{
    SDWORD lIdx = 0;
    for (auto row: vvlCells) {
        for (auto cell: row) {
            lIdx <<= 2;
            switch (cell) {
            case CELL_D:
                lIdx += 0;
                break;
            case CELL_N:
                lIdx += 1;
                break;
            case CELL_NONE:
                lIdx += 2;
                break;
            }
        }
    }
    return lIdx;
}

static void printCells(
    const vector<vector<SDWORD>> &vvlCells)
{
    for (auto row: vvlCells) {
        for (auto cell: row) {
            if (CELL_D == cell) {
                printf("o");
            } else if (CELL_N == cell) {
                printf("x");
            } else {
                printf("-");
            }
        }
        printf("\n");
    }
}

static SDWORD getScore(
    const vector<vector<SDWORD>> &vvlCells,
    SCORES &stScore)
{
    stScore.lD = 0;
    stScore.lN = 0;
//    printCells(vvlCells);
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            if (vvlCells[lRow][lCol] == vvlCells[lRow+1][lCol]) {
                stScore.lD += s_vvlScoreB[lRow][lCol];
            } else {
                stScore.lN += s_vvlScoreB[lRow][lCol];
            }
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            if (vvlCells[lRow][lCol] == vvlCells[lRow][lCol+1]) {
                stScore.lD += s_vvlScoreC[lRow][lCol];
            } else {
                stScore.lN += s_vvlScoreC[lRow][lCol];
            }
        }
    }
}


/**
 *  calc scores
 */
static void execSingleStep(
    vector<vector<SDWORD>> vvlCells,
    SDWORD lTurn,
    SCORES &stRsltScores)
{
    /* lookup memo */
    SDWORD lIdx = getMemoIdx(vvlCells);
    if (vbResultValid[lIdx]) {
        stRsltScores = vlResultMemo[lIdx];
//        printf("hit %d %d:%x\n", stRsltScores.lD, stRsltScores.lN, lIdx);
        return;
    }

    /* filled all cells */
    if (9 == lTurn) {
        getScore(vvlCells, stRsltScores);
//        printf("calc %d %d\n", stRsltScores.lD, stRsltScores.lN);
        return;
    }


    /* exec search */
    SDWORD lCell;
    if ((lTurn % 2) == 0) {
        lCell = CELL_D;
    } else {
        lCell = CELL_N;
    }

    stRsltScores.lD = 0;
    stRsltScores.lN = 0;

    for (SDWORD lRow = 0; lRow < vvlCells.size(); lRow++) {
        for (SDWORD lCol = 0; lCol < vvlCells[lRow].size(); lCol++) {
            auto cell_prev = vvlCells[lRow][lCol];
            SCORES stScores;
            if (CELL_NONE == cell_prev) {
                vvlCells[lRow][lCol] = lCell;
                execSingleStep(vvlCells, lTurn + 1, stScores);
                if (CELL_D == lCell) {
                    if (stRsltScores.lD < stScores.lD) {
                        stRsltScores = stScores;
                    }
                } else {
                    if (stRsltScores.lN < stScores.lN) {
                        stRsltScores = stScores;
                    }
                }
            }
            vvlCells[lRow][lCol] = cell_prev;
//            printf("%d %d\n", stScores.lD, stScores.lN);
        }
    }

    vlResultMemo[lIdx] = stRsltScores;
    vbResultValid[lIdx] = true;
//    printf("score : %d %d %x\n", stRsltScores.lD, stRsltScores.lN, getMemoIdx(vvlCells));
}



int main(void)
{
    SDWORD lSum = 0;
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            s_vvlScoreB[lRow][lCol] = inputSDWORD();
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            s_vvlScoreC[lRow][lCol] = inputSDWORD();
        }
    }

    vector<vector<SDWORD>> vvlCells(3,vector<SDWORD>(3, CELL_NONE));
    SCORES stScores;
    execSingleStep(vvlCells, 0, stScores);

    printf("%d\n", stScores.lD);
    printf("%d\n", stScores.lN);

    return 0;
}