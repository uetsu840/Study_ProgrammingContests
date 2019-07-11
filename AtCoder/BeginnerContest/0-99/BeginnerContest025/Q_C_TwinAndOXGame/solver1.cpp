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


static SDWORD getScore(
    SDWORD lPos0, 
    SDWORD lPos1, 
    SDWORD lPos2, 
    SDWORD lPos3,
    const vector<vector<SDWORD>> &vvlScoreB,
    const vector<vector<SDWORD>> &vvlScoreC)
{
    vector<vector<SDWORD>> vvlCells(3, vector<SDWORD>(3, 0));
    vvlCells[lPos0 / 3][lPos0 % 3] = 1;
    vvlCells[lPos1 / 3][lPos1 % 3] = 1;
    vvlCells[lPos2 / 3][lPos2 % 3] = 1;
    vvlCells[lPos3 / 3][lPos3 % 3] = 1;

    printf("%d %d %d %d\n", lPos0, lPos1, lPos2, lPos3);

    SDWORD lScore = 0;
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            if ((0 < vvlCells[lRow][lCol]) 
                && (0 < vvlCells[lRow+1][lCol])) {
                lScore += vvlScoreB[lRow][lCol];
            }
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            if ((0 < vvlCells[lRow][lCol]) 
                && (0 < vvlCells[lRow][lCol+1])) {
                lScore += vvlScoreC[lRow][lCol];
            }
        }
    }

    return lScore;
}

int main(void)
{
    vector<vector<SDWORD>> vvlScoreB(2, vector<SDWORD>(3));
    vector<vector<SDWORD>> vvlScoreC(3, vector<SDWORD>(2));

    SDWORD lSum = 0;
    for (SDWORD lRow = 0; lRow < 2; lRow++) {
        for (SDWORD lCol = 0; lCol < 3; lCol++) {
            vvlScoreB[lRow][lCol] = inputSDWORD();
            lSum += vvlScoreB[lRow][lCol];
        }
    }
    for (SDWORD lRow = 0; lRow < 3; lRow++) {
        for (SDWORD lCol = 0; lCol < 2; lCol++) {
            vvlScoreC[lRow][lCol] = inputSDWORD();
            lSum += vvlScoreC[lRow][lCol];
        }
    }
    
    SDWORD lAns = 0;
    for (SDWORD lIdx0 = 0; lIdx0 <=5; lIdx0++) {
        for (SDWORD lIdx1 = lIdx0+1; lIdx1 <= 6; lIdx1++) {
            for (SDWORD lIdx2 = lIdx1+1; lIdx2 <=7; lIdx2++) {
                for (SDWORD lIdx3 = lIdx2+1; lIdx3 <= 8; lIdx3++) {
                    lAns = max(lAns, getScore(lIdx0, lIdx1, lIdx2, lIdx3, vvlScoreB, vvlScoreC));
                }
            }
        }
    }

    printf("%d\n", lSum - lAns);

    return 0;
}