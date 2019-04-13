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

#define MAX_EDGES   (100000 + 1)
#define MAX_POINTS  (100000 + 1)
#define DIST_MIN   (0)
int main(void)
{
    DWORD dwInput_N, dwInput_M;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();


    static vector<DWORD> avEdges[MAX_POINTS];
    static vector<DWORD> avRevEdges[MAX_POINTS];
    DWORD dwEdgeNum = dwInput_N + dwInput_M - 1;

    for (DWORD dwIdx = 1; dwIdx <= dwEdgeNum; dwIdx++) {
        DWORD dwInput_A, dwInput_B;
        dwInput_A = inputSDWORD();
        dwInput_B = inputSDWORD();
    
        avEdges[dwInput_A].push_back(dwInput_B);
        avRevEdges[dwInput_B].push_back(dwInput_A);
    }

    /* 親を求める */
    DWORD dwCurPos = 1;
    while(1) {
        if (avRevEdges[dwCurPos].size() == 0) {
            break;
        }
        dwCurPos = avRevEdges[dwCurPos][0];
    }
    DWORD dwRoot = dwCurPos;

#if 0
    printf("root = %d\n", dwRoot);

    /* 重複する辺を探す */
    for (DWORD dwIdx = 1; dwIdx <= dwEdgeNum; dwIdx++) {
        printf("%d %d\n", dwIdx, avRevEdges[dwIdx].size());
        if (1 < avRevEdges[dwIdx].size()) {
            printf("dup pnt  %d <= %d\n", dwIdx, avRevEdges[dwIdx][0].first);
        }
    }
#endif
    DWORD   adwDist[MAX_POINTS];

    for (DWORD dwIdx = 0; dwIdx < ArrayLength(adwDist); dwIdx++) {
        adwDist[dwIdx] = DIST_MIN;
    }
    adwDist[dwRoot] = 1;

    for (DWORD dwLoop = 0; dwLoop < dwInput_N; dwLoop++) {
        for (DWORD dwPnt = 1; dwPnt <= dwInput_N; dwPnt++) {
            if (adwDist[dwPnt] != DIST_MIN) {
                for (DWORD dwEdgeIdx = 0; dwEdgeIdx < avEdges[dwPnt].size(); dwEdgeIdx++) {
                    DWORD dwNextPnt = avEdges[dwPnt][dwEdgeIdx];
                    DWORD dwNewDist = adwDist[dwPnt] + 1;
                    DWORD dwPrevDist = adwDist[dwNextPnt];
                    if (dwPrevDist < dwNewDist) {
                        adwDist[dwNextPnt] = dwNewDist;
                    }
                }
            }
        }
    }
    for (DWORD dwPnt = 1; dwPnt <= dwInput_N; dwPnt++) {
        DWORD dwEdgeNum = avRevEdges[dwPnt].size();

        if (0 == dwEdgeNum) {
            printf("0\n");
        } else {
            for (DWORD dwEdgeIdx = 0; dwEdgeIdx < dwEdgeNum; dwEdgeIdx++) {
                DWORD dwPrevPntDist = adwDist[avRevEdges[dwPnt][dwEdgeIdx]];
                if (dwPrevPntDist + 1 == adwDist[dwPnt]) {
                    printf("%d\n", avRevEdges[dwPnt][dwEdgeIdx]);
                    break;
                }
            }
        }
    }


    return 0;
}
