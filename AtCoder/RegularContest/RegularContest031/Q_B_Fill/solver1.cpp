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

#define NUM_ROW     (10)
#define NUM_COL     (10)
static char s_aacMap[NUM_ROW+2][NUM_COL+2];

struct SearchQueEntry {
    SDWORD lRow;
    SDWORD lCol;

public:
    SearchQueEntry(SDWORD r, SDWORD c)
    {
        lRow = r;
        lCol = c;
    }
};

vector<pair<SDWORD, SDWORD>> searchDir = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

static bool bfs(SDWORD lStartRow, SDWORD lStartCol, SDWORD lLandCnt)
{
    queue<SearchQueEntry> pqSearch;
    bool aabIsVisited[NUM_ROW+2][NUM_COL+2];

    memset(aabIsVisited, 0, sizeof(aabIsVisited));

    aabIsVisited[lStartRow][lStartCol] = true;
    pqSearch.push(SearchQueEntry(lStartRow, lStartCol));

    SDWORD lVisitedCnt = 0;
    while(!pqSearch.empty()) {
        SearchQueEntry pos = pqSearch.front();
        pqSearch.pop();

        for (auto it: searchDir) {
            SDWORD lNextRow = pos.lRow + it.first;
            SDWORD lNextCol = pos.lCol + it.second;
            if (!aabIsVisited[lNextRow][lNextCol]
                && (s_aacMap[lNextRow][lNextCol] == 'o')) {
                    aabIsVisited[lNextRow][lNextCol] = true;
                    lVisitedCnt++;
                    pqSearch.push(SearchQueEntry(lNextRow, lNextCol));
            }
        }
    }
    if (lVisitedCnt == lLandCnt) {
        return true;
    }
    return false;
}

static DWORD countLand(const char *pcMap)
{
    DWORD dwRet = 0;
    for (DWORD dwIdx = 0; dwIdx < NUM_COL; dwIdx++) {
        if (*(pcMap + dwIdx) == 'o') {
            dwRet++;
        }
    }
    return dwRet;
}


int main()
{
    char    acInput[NUM_COL + 1];
    DWORD   dwLandCnt = 0;

    memset(s_aacMap, 'x', sizeof(s_aacMap));
    for (DWORD dwIdx = 1; dwIdx <= NUM_ROW; dwIdx++) {
        inputString(acInput);
        memcpy(&(s_aacMap[dwIdx][1]), acInput, NUM_COL);
        dwLandCnt+= countLand(acInput);
    }

    bool bSarchFromLand = false;
    for (SDWORD lStartRow = 1; lStartRow <= NUM_ROW; lStartRow++) {
        for (SDWORD lStartCol = 1; lStartCol <= NUM_COL; lStartCol++) {
            if ('x' == s_aacMap[lStartRow][lStartCol]) {
                if (bfs(lStartRow, lStartCol, dwLandCnt)) {
                    printf("YES\n");
                    return 0;
                }
            } else {
                if (!bSarchFromLand) {
                    if (bfs(lStartRow, lStartCol, dwLandCnt - 1)) {
                        printf("YES\n");
                        return 0;
                    }
                    bSarchFromLand = true;
                }
            }
        }
    }
    
    printf("NO\n");
    return 0;
}