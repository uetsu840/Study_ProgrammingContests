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

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}

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

#define DIFF_INF    (100100100100100100)

static SQWORD getMinDiff(vector<SQWORD> &vsqA, vector<SQWORD> &vsqB)
{
    SQWORD sqSizeA = vsqA.size();
    SQWORD sqSizeB = vsqB.size();

    if ((0 == sqSizeA) || (0 == sqSizeB)) {
        return DIFF_INF;
    }
//    printf("size %lld %lld\n", sqSizeA, sqSizeB);

    SQWORD sqIdxA = 0;
    SQWORD sqIdxB = 0;
    SQWORD sqMinDiff = MAX_SQWORD;
    for (SQWORD sqIdxB = 0; sqIdxB < sqSizeB; sqIdxB++) {
        if (sqIdxA + 1 < sqSizeA) {
            sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB]), sqMinDiff);
            if (0 < sqIdxB) {
                sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB - 1]), sqMinDiff);
            }
            while ((vsqA[sqIdxA] < vsqB[sqIdxB]) && (sqIdxA + 1 < sqSizeA)) {
                sqIdxA++;
//                printf("----   %lld %lld\n", sqIdxA, sqIdxB);
                sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB]), sqMinDiff);
                if (0 < sqIdxB) {
                    sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB - 1]), sqMinDiff);
                }
            }
        } else {
//            printf("->--   %lld %lld\n", sqIdxA, sqIdxB);
            sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB]), sqMinDiff);
            if (0 < sqIdxB) {
                sqMinDiff = min(ABS(vsqA[sqIdxA] - vsqB[sqIdxB - 1]), sqMinDiff);
            }
        }
    }

    return sqMinDiff;
}

enum CELL_COLOR {
    COLOR_R,
    COLOR_B,
    COLOR_NONE
};

#define ANS_MOD (998244353)

int main()
{
    SQWORD sqH = inputSQWORD();
    SQWORD sqW = inputSQWORD();

    vector<vector<CELL_COLOR>> vvCellColor(sqH, vector<CELL_COLOR>(sqW, COLOR_NONE));

    for (SQWORD sqRow = 0; sqRow < sqH; sqRow++) {
        string strRow;
        cin >> strRow;
        for (SQWORD sqCol = 0; sqCol < sqW; sqCol++) {
            CELL_COLOR eColor;
            switch(strRow[sqCol]) {
            case 'R':
                eColor = COLOR_R;
                break;
            case 'B':
                eColor = COLOR_B;
                break;
            case '.':
                eColor = COLOR_NONE;
                break;
            }
            vvCellColor[sqRow][sqCol] = eColor;
        }
    }

    SQWORD sqAns = 1;
    for (SQWORD sqSrchIdx = 0; sqSrchIdx < sqH + sqW - 1; sqSrchIdx++) {
        SQWORD sqCountR = 0;
        SQWORD sqCountB = 0;
        SQWORD sqCountNone = 0;
        for (SQWORD sqRow = 0; sqRow <= sqSrchIdx; sqRow++) {
            SQWORD sqCol = sqSrchIdx - sqRow;
            if ((0 <= sqRow) && (sqRow < sqH) && (0 <= sqCol) && (sqCol < sqW)) {
//                printf("%lld %lld %lld\n", sqRow, sqCol, vvCellColor[sqRow][sqCol]);
                if (COLOR_R == vvCellColor[sqRow][sqCol]) {
                    sqCountR++;
                }
                if (COLOR_B == vvCellColor[sqRow][sqCol]) {
                    sqCountB++;
                }
                if (COLOR_NONE == vvCellColor[sqRow][sqCol]) {
                    sqCountNone++;
                }
            }
        }
//        printf("%lld %lld\n", sqCountR, sqCountB);
        if ((0 < sqCountR) && (0 < sqCountB)) {
            sqAns *= 0;
        } else {
            if ((0 == sqCountB + sqCountR) && (0 < sqCountNone)) {
                sqAns *= 2;
            }
        }
        sqAns %= ANS_MOD;
    }
    printf("%lld\n", sqAns);

    return 0;
}
