#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <algorithm>    // std::binary_search, std::sort
#include <vector>       // std::vector
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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

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

#define N_MAX_ROWS  (100000)
#define N_MAX_COLS  (100000)
#define N_MAX_CANDIES   (100000)

int main()
{
    SQWORD sqR = inputSQWORD();
    SQWORD sqC = inputSQWORD();
    SQWORD sqK = inputSQWORD();
    SQWORD sqN = inputSQWORD();

    vector<SQWORD> vsqCntEachRow(sqR, 0);
    vector<SQWORD> vsqCntEachCol(sqC, 0);
    vector<vector<SQWORD>> vvsqCandyColOnRow(sqR);

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sq_r = inputSQWORD();
        SQWORD sq_c = inputSQWORD();

        /* 0-indexed */
        sq_r--;
        sq_c--;

//        printf("%lld %lld\n", sq_r, sq_c);

        vsqCntEachRow[sq_r]++;
        vsqCntEachCol[sq_c]++;
        vvsqCandyColOnRow[sq_r].emplace_back(sq_c);
    }

    vector<SQWORD> vsqCandyNumsRow(sqN + 1, 0);
    vector<SQWORD> vsqCandyNumsCol(sqN + 1, 0);
    for (auto r: vsqCntEachRow) {
        vsqCandyNumsRow[r]++;
    }
    for (auto c: vsqCntEachCol) {
        vsqCandyNumsCol[c]++;
    }

    SQWORD sqAns = 0;
    for (SQWORD sqRowIdx = 0; sqRowIdx < sqR; sqRowIdx++) {
        SQWORD sqBaseCnt = vsqCntEachRow[sqRowIdx];


        for (auto candy_col: vvsqCandyColOnRow[sqRowIdx]) {
            vsqCandyNumsCol[vsqCntEachCol[candy_col]]--;
            vsqCandyNumsCol[vsqCntEachCol[candy_col]-1]++;
        }

        if (0 <= sqK - sqBaseCnt) {
            sqAns += vsqCandyNumsCol[sqK - sqBaseCnt];
        }
        for (auto candy_col: vvsqCandyColOnRow[sqRowIdx]) {
            vsqCandyNumsCol[vsqCntEachCol[candy_col]]++;
            vsqCandyNumsCol[vsqCntEachCol[candy_col]-1]--;
        }
    }

    printf("%lld\n", sqAns);
    return 0;
}