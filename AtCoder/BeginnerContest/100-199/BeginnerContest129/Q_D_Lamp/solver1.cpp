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

/*----------------------------------------------*/
#define N_MAX_ROWS  (2000)
#define N_MAX_COLS  (2000)

int main()
{
    SDWORD lInput_H = inputSDWORD();
    SDWORD lInput_W = inputSDWORD();

    static vector<SDWORD> s_avecObsRow[N_MAX_ROWS];
    static vector<SDWORD> s_avecObsCol[N_MAX_COLS];
    char acInput_Row[N_MAX_COLS + 1];
    static bool s_aabIsObs[N_MAX_ROWS][N_MAX_COLS];

    for (SDWORD lRowIdx = 0; lRowIdx < lInput_H; lRowIdx++) {
        inputString(acInput_Row);
        for (SDWORD lColIdx = 0; lColIdx < lInput_W; lColIdx++) {
            if (acInput_Row[lColIdx] == '#') {
                /* obstacles */
                s_avecObsRow[lRowIdx].emplace_back(lColIdx);
                s_avecObsCol[lColIdx].emplace_back(lRowIdx);
                s_aabIsObs[lRowIdx][lColIdx] = true;
            }
        }
    }
    SDWORD lAns = 0;
    for (SDWORD lRowIdx = 0; lRowIdx < lInput_H; lRowIdx++) {
        for (SDWORD lColIdx = 0; lColIdx < lInput_W; lColIdx++) {
            if (!s_aabIsObs[lRowIdx][lColIdx]) {
                auto &vecRow = s_avecObsRow[lRowIdx];
                auto &vecCol = s_avecObsCol[lColIdx];


                SDWORD lNumRow, lNumCol;
                if (0 == vecRow.size()) {
                    lNumRow = lInput_W - 1;
                } else {
                    auto it_row_upper = upper_bound(vecRow.begin(), vecRow.end(), lColIdx);
                    SDWORD lNumRowL, lNumRowR;
                    if (it_row_upper == vecRow.begin()) {
                        lNumRowL = 0;
                    } else {
                        auto it_row_lower = it_row_upper - 1;
                        lNumRowL = *it_row_lower + 1;
                    }
                    if (it_row_upper == vecRow.end()) {
                        lNumRowR = lInput_W - 1;
                    } else {
                        lNumRowR = *it_row_upper - 1;
                    }
                    lNumRow = lNumRowR - lNumRowL;
//                    printf("Row %d %d\n", lNumRowR, lNumRowL);
                }

                if (0 == vecCol.size()) {
                    lNumCol = lInput_H - 1;
                } else {
                    auto it_col_upper = upper_bound(vecCol.begin(), vecCol.end(), lRowIdx);
                    SDWORD lNumColL, lNumColR;
                    if (it_col_upper == vecCol.begin()) {
                        lNumColL = 0;
                    } else {
                        auto it_col_lower = it_col_upper - 1;
                        lNumColL = *it_col_lower + 1;
                    }
                    if (it_col_upper == vecCol.end()) {
                        lNumColR = lInput_H - 1;
                    } else {
                        lNumColR = *it_col_upper - 1;
                    }
                    lNumCol = lNumColR - lNumColL;
//                    printf("Col %d %d\n", lNumColR, lNumColL);
                }
                lAns = max(lAns, lNumCol + lNumRow + 1);

  //              printf("[%d %d] %d %d\n", lRowIdx, lColIdx, lNumRow, lNumCol);
            }
        }
    }


    printf("%d\n", lAns);

    return 0;
}