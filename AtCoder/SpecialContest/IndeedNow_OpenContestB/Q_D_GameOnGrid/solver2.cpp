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

#define MAX_N_ROWS      (100)
#define MAX_N_COLS      (100)   

static bool s_aabIsUsed[MAX_N_ROWS][MAX_N_COLS];
static SDWORD s_aalMaxCost[MAX_N_ROWS][MAX_N_COLS];
static SDWORD s_aalPoint[MAX_N_ROWS][MAX_N_COLS];

int main()
{
    SDWORD lInput_H = inputSDWORD();
    SDWORD lInput_W = inputSDWORD();
 
    SDWORD lInput_Sx = inputSDWORD();
    SDWORD lInput_Sy = inputSDWORD();
    SDWORD lInput_Gx = inputSDWORD();
    SDWORD lInput_Gy = inputSDWORD();
 
    vector<pair<SDWORD, SDWORD>> vplDir;
    vplDir.emplace_back(make_pair(1, 0));
    vplDir.emplace_back(make_pair(-1, 0));
    vplDir.emplace_back(make_pair(0, 1));
    vplDir.emplace_back(make_pair(0, -1));

    for (SDWORD lRow = 0; lRow < lInput_H; lRow++) {
        for (SDWORD lCol = 0; lCol < lInput_W; lCol++) {
            s_aalPoint[lRow][lCol] = inputSDWORD();            
        }
    }
 
    /* Initialize */
    for (SDWORD lRow = 0; lRow < lInput_H; lRow++) {
        for (SDWORD lCol = 0; lCol < lInput_W; lCol++) {
            s_aabIsUsed[lRow][lCol] = false;
            s_aalMaxCost[lRow][lCol] = 0;
        }
    }

    /* 頂点0だけを登録 */
    s_aalMaxCost[lInput_Sy-1][lInput_Sx-1] = s_aalPoint[lInput_Sy-1][lInput_Sx-1];

    SQWORD sqTotalCost = 0;
    while (1) {
        SDWORD lAddRow = -1;
        SDWORD lAddCol = -1;

        /* search vertex with edge not belongs to X with maximum cost */
        for (SDWORD lRow = 0; lRow < lInput_H; lRow++) {
            for (SDWORD lCol = 0; lCol < lInput_W; lCol++) {
                if (!s_aabIsUsed[lRow][lCol]) {
                    if (-1 == lAddRow) {
                        lAddRow = lRow;
                        lAddCol = lCol;
                    } else if (s_aalMaxCost[lAddRow][lAddCol] < s_aalMaxCost[lRow][lCol]) {
                        lAddRow = lRow;
                        lAddCol = lCol;
                    }
                }
            }
        }

        if (-1 == lAddRow) {
            break;
        }

        s_aabIsUsed[lAddRow][lAddCol] = true;
        sqTotalCost += (SQWORD)(s_aalMaxCost[lAddRow][lAddCol]);
//        printf("%d\n", s_aalMaxCost[lAddRow][lAddCol]);
        for (SDWORD lRow = max(0, lAddRow - 1); lRow <= min(lInput_H - 1, lAddRow + 1); lRow++) {
            for (SDWORD lCol = max(0, lAddCol - 1); lCol <= min(lInput_W - 1, lAddCol + 1); lCol++) {
                for (auto dir : vplDir) {
                    if ((dir.first == (lRow - lAddRow)) && (dir.second == (lCol - lAddCol))) {
                       s_aalMaxCost[lRow][lCol] = max(s_aalMaxCost[lRow][lCol],
                                                    s_aalPoint[lRow][lCol] + 
                                                    s_aalPoint[lRow][lCol] * s_aalPoint[lAddRow][lAddCol]);                        
                    }
                }
            }
        }
    }

    printf("%lld\n", sqTotalCost);
}
