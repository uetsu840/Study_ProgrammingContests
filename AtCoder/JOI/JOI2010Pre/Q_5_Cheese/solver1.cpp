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

#define MAX_NUM_ROW     (1000)
#define MAX_NUM_COL     (1000)
#define DIST_INF        (1000000000)
#define SYM_WALL        ('X')
#define SYM_AVAILABLE   ('.')
static char s_aacMap[MAX_NUM_ROW+2][MAX_NUM_COL+2];

vector<pair<SDWORD, SDWORD>> searchDir = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

static SDWORD solveBfs(
    pair<SDWORD, SDWORD> pairStartPos,
    char symTarget,
    pair<SDWORD, SDWORD> &pairCheesePos)
{
    static SDWORD s_aalMinDist[MAX_NUM_ROW+2][MAX_NUM_COL+2];
    queue<pair<SDWORD, SDWORD>> queSearch;

    for (SDWORD lRow = 0; lRow < ArrayLength(s_aalMinDist); lRow++) {
        for (SDWORD lCol = 0; lCol < ArrayLength(s_aalMinDist[0]); lCol++) {
            s_aalMinDist[lRow][lCol] = DIST_INF;
        }
    }

    s_aalMinDist[pairStartPos.first][pairStartPos.second] = 0;
    queSearch.push(pairStartPos);

    while(0 < queSearch.size()) {
        auto pos = queSearch.front();
        queSearch.pop();

        for (auto it: searchDir) {
            SDWORD lNextRow = pos.first + it.first;
            SDWORD lNextCol = pos.second + it.second;
            SDWORD lCurDist = s_aalMinDist[pos.first][pos.second];

            char cNextSym = s_aacMap[lNextRow][lNextCol];
            if (symTarget == cNextSym) {
                pairCheesePos = make_pair(lNextRow, lNextCol);
                return lCurDist + 1;
            } else if ((cNextSym != SYM_WALL) 
                         && (DIST_INF == s_aalMinDist[lNextRow][lNextCol])) {
                s_aalMinDist[lNextRow][lNextCol] = lCurDist + 1;
                queSearch.push(make_pair(lNextRow, lNextCol));
            }
        }
    }
    return DIST_INF;
}

static bool searchChar(const char *pcMap, char c, DWORD *pdwPos)
{
    const char *pc = strchr(pcMap, c);
    if (NULL == pc) {
        return false;
    }

    *pdwPos = pc - pcMap;
    return true;
}

int main()
{
    char    acInput[MAX_NUM_COL + 1];
    DWORD   dwLandCnt = 0;

    SQWORD sqInput_Row = inputSQWORD();
    SQWORD sqInput_Col = inputSQWORD();
    SQWORD sqInput_Cheese = inputSQWORD();
    memset(s_aacMap, SYM_WALL, sizeof(s_aacMap));

    pair<SDWORD, SDWORD> pairStartPos;

    for (DWORD dwRowIdx = 1; dwRowIdx <= sqInput_Row; dwRowIdx++) {
        inputString(acInput);
        memcpy(&(s_aacMap[dwRowIdx][1]), acInput, sqInput_Col);

        /* search start pos */
        DWORD dwCharIdx;
        if (searchChar(acInput, 'S', &dwCharIdx)) {
            pairStartPos = make_pair(dwRowIdx, dwCharIdx + 1);
        }
    }

    SDWORD lTotalDist = 0;
    for (DWORD dwCheeseIdx = 0; dwCheeseIdx < sqInput_Cheese; dwCheeseIdx++) {
        pair<SDWORD, SDWORD> pairCheesePos;
        SDWORD lCurDist = solveBfs(pairStartPos, (char)('0' + dwCheeseIdx + 1), pairCheesePos);

        pairStartPos = pairCheesePos;
        lTotalDist += lCurDist;
    }

    printf("%d\n", lTotalDist);
    return 0;
}