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

#define DIV_MOD (100000)

struct SHUFFLE {
    SQWORD sqX;
    SQWORD sqY;

    SHUFFLE (SQWORD x, SQWORD y): sqX(x), sqY(y) {};
};

struct SEGMENT {
    SQWORD sqStartIdx;
    SQWORD sqLen;

    SEGMENT(SQWORD start, SQWORD len) : sqStartIdx(start), sqLen(len) {};
};


static void stackOne(
    SQWORD sqDivL,
    SQWORD sqDivH,
    const vector<SEGMENT> &vecSegmentCur,
    vector<SEGMENT> &vecSegPart)
{
    SQWORD sqPosCur = 0;
    for (auto seg: vecSegmentCur) {
        if (sqDivH <= sqPosCur) {
            ;
        } else if (sqPosCur + seg.sqLen <= sqDivL) {
            ;
        } else {
            SQWORD sqCurSegL = max(sqPosCur, sqDivL);
            SQWORD sqCurSegH = min(sqPosCur + seg.sqLen, sqDivH);

            vecSegPart.emplace_back(seg.sqStartIdx + (sqCurSegL - sqPosCur), sqCurSegH - sqCurSegL);
        }
        sqPosCur += seg.sqLen;
    }
}

/*
*   1回シャッフルする
*/
static void execShffleOne(
    vector<SEGMENT> vecSegmentCur,
    vector<SEGMENT> &vecSegmentNext,
    SQWORD sqInput_n,
    SHUFFLE &shuffle)
{
    SQWORD sqPosCur = 0;
    SQWORD sqIterIdx = 0;
    vector<SEGMENT> vecPart0, vecPart1, vecPart2;

    stackOne(0,           shuffle.sqX, vecSegmentCur, vecPart0);
    stackOne(shuffle.sqX, shuffle.sqY, vecSegmentCur, vecPart1);
    stackOne(shuffle.sqY, sqInput_n,   vecSegmentCur, vecPart2);

    vecSegmentNext.clear();
    copy(vecPart2.begin(), vecPart2.end(), back_inserter(vecSegmentNext));
    copy(vecPart1.begin(), vecPart1.end(), back_inserter(vecSegmentNext));
    copy(vecPart0.begin(), vecPart0.end(), back_inserter(vecSegmentNext));
}


int main()
{
    SQWORD sqInput_n = inputSQWORD();
    SQWORD sqInput_m = inputSQWORD();

    SQWORD sqInput_p = inputSQWORD();
    SQWORD sqInput_q = inputSQWORD();
    SQWORD sqInput_r = inputSQWORD();

    vector<SHUFFLE> vecShffle;
    vector<SQWORD> vecsqDiv;

    for (SQWORD sqIdx = 0; sqIdx < sqInput_m; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vecShffle.emplace_back(sqX, sqY);
    }

    vector<SEGMENT> vecSegment;
    vecSegment.emplace_back(0, sqInput_n);

    for (auto shffle: vecShffle) {
        execShffleOne(vecSegment, vecSegment, sqInput_n, shffle);

    }
#if 0
    for (auto seg: vecSegment) {
        printf("[%lld %lld]", seg.sqStartIdx, seg.sqLen);
    }
    printf("\n");
#endif

    SQWORD sqAns = 0;
    SQWORD sqCardIdxSum = 0;
    for (auto seg: vecSegment) {
        SQWORD sqCardIdxL = min(sqInput_q,     max(sqInput_p - 1, sqCardIdxSum));
        SQWORD sqCardIdxH = max(sqInput_p - 1, min(sqInput_q,     sqCardIdxSum + seg.sqLen));

        /* 開始から何枚目までがR以下か */
        SQWORD sqNumEndIndex   = sqCardIdxSum + max((SQWORD)0, (sqInput_r - seg.sqStartIdx));
        sqNumEndIndex = min(sqNumEndIndex, sqCardIdxH);

        SQWORD sqCardNum = max((SQWORD)0, sqNumEndIndex - sqCardIdxL);

//        printf("[%lld %lld] %lld %lld\n", sqCardIdxL, sqCardIdxH, sqNumEndIndex, sqCardNum);
        sqAns += sqCardNum;

        sqCardIdxSum += seg.sqLen;
    }
    printf("%lld\n", sqAns);



    return 0;
}