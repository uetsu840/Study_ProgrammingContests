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


/*-----------------------------------------------------*/

/*----------------------------------------------*/

static bool isVisited(SQWORD sqBitmap, SQWORD sqNode)
{
    return (sqBitmap & (0x1 << sqNode));
}

static bool isRed(SQWORD sqRedBitmap, SQWORD sqNode) {
    return (sqRedBitmap & (0x1 << sqNode));
}

#define COLOR_NONE  (255)
#define COLOR_BLUE  (0)
#define COLOR_GREEN (1)

static bool paintNode(
    SQWORD sqN, 
    vector<BYTE> &vbyColors, 
    const vector<SQWORD> &vEdge, 
    SQWORD sqCurNode) 
{
    SQWORD sqEdge = vEdge[sqCurNode];
    for (SQWORD sqNextNode = 0; sqNextNode < sqN; sqNextNode++) {
        if (sqEdge & (0x1 << sqNextNode)) {
            if (vbyColors[sqNextNode] == COLOR_NONE) {
                if (COLOR_BLUE == vbyColors[sqCurNode]) {
                    vbyColors[sqNextNode] = COLOR_GREEN;
                } else {
                    vbyColors[sqNextNode] = COLOR_BLUE;
                }
                if (!paintNode(sqN, vbyColors, vEdge, sqNextNode)) {
                    return false;
                }
            } else {
                if (vbyColors[sqNextNode] == vbyColors[sqCurNode]) {
                    return false;
                }
            }
        }
    }
    return true;
}

static SQWORD serach2Colors(SQWORD sqN, const vector<SQWORD> &vEdge, SQWORD sqRedBitMap)
{
    vector<SQWORD> vMaskedEdge(sqN, 0);
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vMaskedEdge[sqIdx] = vEdge[sqIdx] & (~sqRedBitMap);
    }

    SQWORD sqAns = 1;
    vector<BYTE> vbyColors(sqN, COLOR_NONE);
    for (SQWORD sqStartNode = 0; sqStartNode < sqN; sqStartNode++) {
        if (!isRed(sqRedBitMap, sqStartNode) && (COLOR_NONE == vbyColors[sqStartNode])) {
            vbyColors[sqStartNode] = COLOR_GREEN;
            if (paintNode(sqN, vbyColors, vMaskedEdge, sqStartNode)) {
                sqAns *= (SQWORD)2;                
            } else {
                return 0;
            }
        }
    }
    return sqAns;
}



int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    vector<SQWORD> vEdge(sqN, 0);

    for (SQWORD sqEdgeIdx = 0; sqEdgeIdx < sqM; sqEdgeIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();

        sqA--;  /* 0-indexed */
        sqB--;  /* 0-indexed */

        vEdge[sqA] |= (1 << sqB);
        vEdge[sqB] |= (1 << sqA);
    }

    SQWORD sqBitMapMax = (1 << sqN) - 1;
    SQWORD sqAns = 0;
    for (SQWORD sqRedBitMap = 0; sqRedBitMap <= sqBitMapMax; sqRedBitMap++) {
        bool bRedValid = true;
        for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
            SQWORD sqCurEdge = vEdge[sqIdx] & sqRedBitMap;
            if ((sqRedBitMap & (0x1 << sqIdx)) && (0 < sqCurEdge)) {
                bRedValid = false;
            }
        }
        if (bRedValid) {
            sqAns += serach2Colors(sqN, vEdge, sqRedBitMap);
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}

