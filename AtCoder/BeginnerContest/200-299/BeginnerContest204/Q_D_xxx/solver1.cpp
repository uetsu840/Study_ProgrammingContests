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

#define MAX_N   800

struct PROBLEM_ONE {
    SQWORD sqN;
    SQWORD sqK;
    SQWORD aasqVal[MAX_N][MAX_N];
};


/**
 *  すべての K x K のマスの中間値が、x以上である。
 */
bool isMedialUpperThanN(SQWORD sqBorder, const PROBLEM_ONE &stProb)
{
    static bool aabIsUpper[MAX_N][MAX_N];
    static SQWORD aasqCumSum[MAX_N + 1][MAX_N + 1];

    for (SQWORD sqRow = 0; sqRow < stProb.sqN; sqRow++) {
        for (SQWORD sqCol = 0; sqCol < stProb.sqN; sqCol++) {
            if (sqBorder <= stProb.aasqVal[sqRow][sqCol]) {
                aabIsUpper[sqRow][sqCol] = true;
            } else {
                aabIsUpper[sqRow][sqCol] = false;
            }
        }
    }

    /* initialize */
    for (SQWORD sqRow = 0; sqRow <= stProb.sqN; sqRow++) {
        aasqCumSum[sqRow][0] = 0;
    }
    for (SQWORD sqCol = 0; sqCol <= stProb.sqN; sqCol++) {
        aasqCumSum[0][sqCol] = 0;
    }

    /* cum sum */
    for (SQWORD sqRow = 0; sqRow < stProb.sqN; sqRow++) {
        SQWORD sqRowSum = 0;
        for (SQWORD sqCol = 0; sqCol < stProb.sqN; sqCol++) {
            if (aabIsUpper[sqRow][sqCol]) {
                sqRowSum++;                
            }
            aasqCumSum[sqRow + 1][sqCol + 1] = aasqCumSum[sqRow][sqCol + 1] + sqRowSum; 
        }
    }

    SQWORD sqHalfCnt = (stProb.sqK * stProb.sqK) / 2 + 1;

    for (SQWORD sqRowUL = 0; sqRowUL <= stProb.sqN - stProb.sqK; sqRowUL++) {
        for (SQWORD sqColUL = 0; sqColUL <= stProb.sqN - stProb.sqK; sqColUL++) {
            SQWORD sqCellNum = aasqCumSum[sqRowUL + stProb.sqK][sqColUL + stProb.sqK]
                                - aasqCumSum[sqRowUL + stProb.sqK][sqColUL]
                                - aasqCumSum[sqRowUL][sqColUL + stProb.sqK]
                                + aasqCumSum[sqRowUL][sqColUL];
            if (sqCellNum < sqHalfCnt) {
                return false;
            }
        }
    }
    return true;
}


/*----------------------------------------------*/

static SQWORD binarySearch(
    bool (*pfJudge)(SQWORD, const PROBLEM_ONE&),
    SQWORD sqInitLower, 
    SQWORD sqInitUpper, 
    const PROBLEM_ONE &stParam)
{
    SQWORD sqOk = sqInitLower;
    SQWORD sqNg = sqInitUpper;

    while (1 < sqNg - sqOk) {
        SQWORD sqMid = (sqNg + sqOk) / 2LL;
        if (pfJudge(sqMid, stParam)) {
            sqOk = sqMid;
        } else {
            sqNg = sqMid;
        }
    }
    return sqOk;
}


int main(void)
{
    static PROBLEM_ONE stProb;

    stProb.sqN = inputSQWORD();
    stProb.sqK = inputSQWORD();
    for (SQWORD sqRow = 0; sqRow < stProb.sqN; sqRow++) {
        for (SQWORD sqCol = 0; sqCol < stProb.sqN; sqCol++) {
            stProb.aasqVal[sqRow][sqCol] = inputSQWORD();
        }
    }

    SQWORD sqAns = binarySearch(isMedialUpperThanN, 0, MAX_SDWORD, stProb);

    printf("%lld\n", sqAns);
    return 0;
}


