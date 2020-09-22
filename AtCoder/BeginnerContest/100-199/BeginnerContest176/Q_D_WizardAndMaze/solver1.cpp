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

#define MAX_H   (1000)
#define MAX_W   (1000)
#define DIST_INF    (100100100100)

int main(void)
{
    SDWORD lH = inputSQWORD();
    SDWORD lW = inputSQWORD();
    SDWORD lCh = inputSQWORD();
    SDWORD lCw = inputSQWORD();
    SDWORD lDh = inputSQWORD();
    SDWORD lDw = inputSQWORD();

    static SQWORD s_aasqDist[MAX_H + 2][MAX_W + 2];
    static SDWORD s_aalMap[MAX_H + 2][MAX_W + 2];

    for (SDWORD lIdxH = 0; lIdxH < ArrayLength(s_aasqDist); lIdxH++) {
        for (SDWORD lIdxW = 0; lIdxW < ArrayLength(s_aasqDist[0]); lIdxW++) {
            s_aasqDist[lIdxH][lIdxW] = DIST_INF;
            s_aalMap[lIdxH][lIdxW] = 1;
        }
    }
    for (SDWORD lRow = 0; lRow < lH; lRow++) {
        string strRow;
        cin >> strRow;
        for (SDWORD lCol = 0; lCol < lW; lCol++) {
            if ('.' == strRow[lCol]) {
                s_aalMap[lRow + 1][lCol + 1] = 0;
            }
        }
    }


    queue<pair<SDWORD, SDWORD>> quePos;

    vector<pair<SDWORD, SDWORD>> vpOfs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    s_aasqDist[lCh][lCw] = 0;
    quePos.push(make_pair(lCh, lCw));
    while(1) {
        vector<pair<SDWORD, SDWORD>> vWarpFrom;
        if (quePos.empty()) {
            break;
        }
        while (!quePos.empty()) {
            auto pos = quePos.front();
            quePos.pop();
            vWarpFrom.emplace_back(pos);
            SDWORD lCurH = pos.first;
            SDWORD lCurW = pos.second;
            SQWORD sqCurDist = s_aasqDist[lCurH][lCurW];
//            printf("------: %lld %lld - %lld\n", lCurH, lCurW, sqCurDist);
            for (auto ofs: vpOfs) {
                SDWORD lNextH = lCurH + ofs.first;
                SDWORD lNextW = lCurW + ofs.second;
                if (s_aalMap[lNextH][lNextW] == 0) {
                    /* not using warp */
                    if (DIST_INF == s_aasqDist[lNextH][lNextW]) {
                        s_aasqDist[lNextH][lNextW] = sqCurDist;
//                        printf("r: %lld %lld - %lld\n", lNextH, lNextW, sqCurDist);
                        quePos.push(make_pair(lNextH, lNextW));
                    }
                }
            }
        }
        for (auto pos: vWarpFrom) {
            SDWORD lCurH = pos.first;
            SDWORD lCurW = pos.second;
            SQWORD sqCurDist = s_aasqDist[lCurH][lCurW];
            for (SDWORD lWarpH = max(0, lCurH - 2); lWarpH <= min(lH + 1, lCurH + 2); lWarpH++) {
                for (SDWORD lWarpW = max(0, lCurW - 2); lWarpW <= min(lW + 1, lCurW + 2); lWarpW++) {
                    if (s_aalMap[lWarpH][lWarpW] == 0) {
                        if (s_aasqDist[lWarpH][lWarpW] == DIST_INF) {
//                            printf("warp: %lld %lld\n", lWarpH, lWarpW);
                            s_aasqDist[lWarpH][lWarpW] = sqCurDist + 1;
                            quePos.push(make_pair(lWarpH, lWarpW));
                        }
                    }
                }
            }
        }
    }


    SQWORD sqAns = s_aasqDist[lDh][lDw];
    if (DIST_INF == sqAns) {
        printf("-1\n");
    } else {
        printf("%lld\n", sqAns);    
    }

    return 0;
}

