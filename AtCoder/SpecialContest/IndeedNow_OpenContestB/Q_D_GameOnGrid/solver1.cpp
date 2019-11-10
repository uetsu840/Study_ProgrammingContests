#pragma GCC optimize ("O3")

#include <iostream>
#include <cstddef>
#include <numeric>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using BYTE = unsigned char;
using SBYTE = char;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MAX_QWORD (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFF)
#define MAX_WORD  (0xFFFF)
#define MAX_BYTE  (0xFF)


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

static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber;
            }
        }
    }
}

#define DIST_INF        (MAX_DWORD)
#define MAX_N_ROWS      (100)
#define MAX_N_COLS      (100)   
#define MAX_NODE_NUM    (MAX_N_ROWS * MAX_N_COLS)

static bool s_abIsUsed[MAX_NODE_NUM];
static SDWORD s_alMaxCost[MAX_NODE_NUM];
static SDWORD s_aalVals[MAX_N_ROWS][MAX_N_COLS];

static SDWORD getNodeIdx(SDWORD lRow, SDWORD lCol, SDWORD lNumCols)
{
    return lRow * lNumCols + lCol;
}

int main()
{
    SDWORD lInput_H = inputSDWORD();
    SDWORD lInput_W = inputSDWORD();
 
    SDWORD lInput_Sx = inputSDWORD();
    SDWORD lInput_Sy = inputSDWORD();
    SDWORD lInput_Gx = inputSDWORD();
    SDWORD lInput_Gy = inputSDWORD();
 
    SDWORD lNodeNum = lInput_H * lInput_W;
    vector<pair<SDWORD, SDWORD>> vplDir;
    vplDir.emplace_back(make_pair(1, 0));
    vplDir.emplace_back(make_pair(-1, 0));
    vplDir.emplace_back(make_pair(0, 1));
    vplDir.emplace_back(make_pair(0, -1));

    for (SDWORD lRow = 0; lRow < lInput_H; lRow++) {
        for (SDWORD lCol = 0; lCol < lInput_W; lCol++) {
            s_aalVals[lRow][lCol] = inputSDWORD();            
        }
    }
 
    static SDWORD aalCost[MAX_NODE_NUM][MAX_NODE_NUM];
    for (SDWORD lIdx0 = 0; lIdx0 < lInput_H; lIdx0++) {
        for (SDWORD lIdx1 = 0; lIdx1 < lInput_W; lIdx1++) {
            aalCost[lIdx0][lIdx1] = 0;
        }
    }
    for (SDWORD lRow = 0; lRow < lInput_H; lRow++) {
        for (SDWORD lCol = 0; lCol < lInput_W; lCol++) {
            for (auto diff:vplDir) {
                SDWORD lNextRow = lRow + diff.first;
                SDWORD lNextCol = lCol + diff.second;

                lNextRow = max(0, lNextRow);
                lNextRow = min(lInput_H - 1, lNextRow);

                lNextCol = max(0, lNextCol);
                lNextCol = min(lInput_W - 1, lNextCol);

                SDWORD lCurIdx = getNodeIdx(lRow, lCol, lInput_W);
                SDWORD lNextIdx = getNodeIdx(lNextRow, lNextCol, lInput_W);
                if (lCurIdx != lNextIdx) {
                    aalCost[lCurIdx][lNextIdx] = s_aalVals[lNextRow][lNextCol] + 
                                                    s_aalVals[lRow][lCol] * s_aalVals[lNextRow][lNextCol];
                }
            }
        }
    }

    /* Initialize */
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_abIsUsed); dwIdx++) {
        s_abIsUsed[dwIdx] = false;
    }
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_alMaxCost); dwIdx++) {
        s_alMaxCost[dwIdx] = 0;
    }

    /* 頂点0だけを登録 */
    SDWORD lStartIdx = getNodeIdx(lInput_Sx-1, lInput_Sy-1, lInput_W);
    s_alMaxCost[lStartIdx] = s_aalVals[lInput_Sx-1][lInput_Sy-1];

    SQWORD sqMaxCost = 0;
    while (1) {
        SDWORD lV = -1;

        /* search vertex with edge not belongs to X with maximum cost */
        for (SDWORD lVertexIdx = 0; lVertexIdx < lNodeNum; lVertexIdx++) {
            if (!s_abIsUsed[lVertexIdx]) {
                if (-1 == lV) {
                    lV = lVertexIdx;
                } else if (s_alMaxCost[lV] < s_alMaxCost[lVertexIdx]) {
                    lV = lVertexIdx;
                }
            }
        }

        if (-1 == lV) {
            break;
        }

        s_abIsUsed[lV] = true;
        sqMaxCost += (SQWORD)s_alMaxCost[lV];
        for (SDWORD lVertexIdx = 0; lVertexIdx < lNodeNum; lVertexIdx++) {
            s_alMaxCost[lVertexIdx] = max(s_alMaxCost[lVertexIdx], aalCost[lV][lVertexIdx]);
        }
    }

    printf("%lld\n", sqMaxCost);
}
