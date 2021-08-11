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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

static const DOUBLE myPI  = 3.14159265358979;

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

/*----------------------------------------------*/


/*----------------------------------------------*/

/*-----------------------------------------------------*/

/*----------------------------------------------*/

int main(void)
{
    SQWORD sqH = inputSQWORD();
    SQWORD sqW = inputSQWORD();

    vector<string> vstrMap;

    for (SQWORD sqRowIdx = 0; sqRowIdx < sqH; sqRowIdx++) {
        string strS;
        cin >> strS;
        vstrMap.emplace_back(strS);
    }

    SQWORD sqTurnCnt = (sqH + sqW - 2);

    vector<vector<SQWORD>> vvsqScore(sqH, vector<SQWORD>(sqW, 0));

    for (SQWORD sqTurnIdx = sqTurnCnt - 1; 0 <= sqTurnIdx; sqTurnIdx--) {
        bool bIsTurnTakahashi = (0 == sqTurnIdx % 2);
        for (SQWORD sqCurRow = 0; sqCurRow < sqH; sqCurRow++) {
            SQWORD sqCurCol = sqTurnIdx - sqCurRow;
            if (((0 <= sqCurCol) && (sqCurCol < sqW))
                && ((0 <= sqCurRow) && (sqCurRow < sqH))) {
                SQWORD sqScore = 0;
                if (bIsTurnTakahashi) {
                    SQWORD sqNextScore = MIN_SQWORD;
                    if (sqCurCol < sqW - 1) {
                        SQWORD sqScore = (vstrMap[sqCurRow][sqCurCol + 1] == '+') ? 1 : -1;
                        sqNextScore = MAX(sqNextScore, vvsqScore[sqCurRow][sqCurCol + 1] + sqScore);
                    }
                    if (sqCurRow < sqH - 1) {
                        SQWORD sqScore = (vstrMap[sqCurRow + 1][sqCurCol] == '+') ? 1 : -1;
                        sqNextScore = MAX(sqNextScore, vvsqScore[sqCurRow + 1][sqCurCol] + sqScore);
                    }
                    vvsqScore[sqCurRow][sqCurCol] = sqNextScore;
                } else {
                    SQWORD sqNextScore = MAX_SQWORD;
                    if (sqCurCol < sqW - 1) {
                        SQWORD sqScore = (vstrMap[sqCurRow][sqCurCol + 1] == '+') ? -1 : 1;
                        sqNextScore = MIN(sqNextScore, vvsqScore[sqCurRow][sqCurCol + 1] + sqScore);
                    }
                    if (sqCurRow < sqH - 1) {
                        SQWORD sqScore = (vstrMap[sqCurRow + 1][sqCurCol] == '+') ? -1 : 1;
                        sqNextScore = MIN(sqNextScore, vvsqScore[sqCurRow + 1][sqCurCol] + sqScore);
                    }
                    vvsqScore[sqCurRow][sqCurCol] = sqNextScore;
                }
            }
        }
    }

#if 0
    for (SQWORD sqRowIdx = 0; sqRowIdx < sqH; sqRowIdx++) {
        for (SQWORD sqColIdx = 0; sqColIdx < sqW; sqColIdx++) {
            printf("%3lld ", vvsqScore[sqRowIdx][sqColIdx]);
        }
        printf("\n");
    }
#endif
    if (0 < vvsqScore[0][0]) {
        printf("Takahashi\n");
    } else if (vvsqScore[0][0] < 0) {
        printf("Aoki\n");
    } else {
        printf("Draw\n");
    }

    return 0;
}

