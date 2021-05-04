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

struct ST_ITEM {
    SQWORD sqW;
    SQWORD sqV;

    ST_ITEM(SQWORD w, SQWORD v): sqW(w), sqV(v) {};
};

bool operator<(const ST_ITEM &a, const ST_ITEM &b) {
    return a.sqV < b.sqV;
}

void solveOneQuery(vector<ST_ITEM> &items, vector<SQWORD> &boxes, SQWORD sqL, SQWORD sqR)
{
//    printf("====\n");
    vector<bool> vbUsed(boxes.size(), false);
    SQWORD sqAns = 0;
    /* 価値が高い順見てゆく */
    for (auto item: items) {
        SQWORD sqMaxBoxIdx = -1;
        SQWORD sqMinBoxSize = MAX_SQWORD;
        for (SQWORD sqBoxIdx = 0; sqBoxIdx < boxes.size(); sqBoxIdx++) {
            if ((sqBoxIdx < sqL) || (sqR < sqBoxIdx)) {
//                printf("idx[%d] min[%d] cursize[%d]\n", sqBoxIdx, sqMinBoxSize, boxes[sqBoxIdx]);
                if ((item.sqW <= boxes[sqBoxIdx]) && (boxes[sqBoxIdx] < sqMinBoxSize) && !vbUsed[sqBoxIdx]) {
                    sqMinBoxSize = boxes[sqBoxIdx];
                    sqMaxBoxIdx = sqBoxIdx;                               
                }
            }
        }
        if (-1 != sqMaxBoxIdx) {
            vbUsed[sqMaxBoxIdx] = true;
            sqAns += item.sqV;
        }
    }

    printf("%lld\n", sqAns);

//    printf("====\n");
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    vector<ST_ITEM> vstItems;
    vector<SQWORD> vsqBoxes;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqW = inputSQWORD();
        SQWORD sqV = inputSQWORD();

        vstItems.emplace_back(sqW, sqV);
    }
    sort(vstItems.begin(), vstItems.end());
    reverse(vstItems.begin(), vstItems.end());

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        vsqBoxes.emplace_back(sqX);
    }

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqL = inputSQWORD();
        SQWORD sqR = inputSQWORD();

        /* 0-indexed */
        sqL--;
        sqR--;

        solveOneQuery(vstItems, vsqBoxes, sqL, sqR);
    }
    return 0;
}

