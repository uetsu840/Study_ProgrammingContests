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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/


int main(void)
{
    SDWORD lN = inputSDWORD();
    SDWORD lM = inputSDWORD();
    SDWORD lT = inputSDWORD();

    static SDWORD lInitCells[1002][1002];
    for (SDWORD lRow = 1; lRow <= lN; lRow++) {
        string strS;
        cin >> strS;
        for (SDWORD lCol = 1; lCol <= lM; lCol++) {
            char c = strS[lCol - 1];
            if ('0' == c) {
                lInitCells[lRow][lCol] = 1;
            } else {
                lInitCells[lRow][lCol] = 2;

            }
        }
    }

    const vector<pair<SDWORD, SDWORD>> vDir = {{-1, 0} ,{1, 0}, {0, -1}, {0, 1}};

    static SDWORD aalChgStartCnt[1002][1002];
    for (SDWORD lRow = 0; lRow <= lN + 1; lRow++) {
        for (SDWORD lCol = 0; lCol <= lM + 1; lCol++) {
            aalChgStartCnt[lRow][lCol] = -1;
        }
    }

    queue<pair<SDWORD, SDWORD>> queSearch;
    for (SDWORD lRow = 1; lRow <= lN; lRow++) {
        for (SDWORD lCol = 1; lCol <= lM; lCol++) {
            SDWORD lSelf = lInitCells[lRow][lCol];
            for (auto d: vDir) {
                if (lSelf == lInitCells[lRow + d.first][lCol + d.second]) {
                    aalChgStartCnt[lRow][lCol] = 0;
                    queSearch.push(make_pair(lRow, lCol));
                    break;
                }
            }
        }
    }

    /* bfs で変化開始カウントを求める */
    while(!queSearch.empty()) {
        auto pos = queSearch.front();
        queSearch.pop();

        SDWORD lRow = pos.first;
        SDWORD lCol = pos.second;

        for (auto d: vDir) {
            SDWORD lCur = aalChgStartCnt[lRow][lCol];
            if (aalChgStartCnt[lRow + d.first][lCol + d.second] < 0) {
                SDWORD lRowNext = lRow + d.first;
                SDWORD lColNext = lCol + d.second;
                if ((1 <= lRowNext)
                    && (lRowNext <= lN)
                    && (1 <= lColNext)
                    && (lColNext <= lM)){
                    aalChgStartCnt[lRowNext][lColNext] = lCur + 1;
                    queSearch.push(make_pair(lRowNext, lColNext));
                }
            }
        }
    }

#if 0
    for (SDWORD lRow = 0; lRow <= lN + 1; lRow++) {
        for (SDWORD lCol = 0; lCol <= lM + 1; lCol++) {
            printf("%d ", aalChgStartCnt[lRow][lCol]);
        }
        printf("\n");
    }
#endif

    /* クエリ */
    for (SDWORD lQueryIdx = 0; lQueryIdx < lT; lQueryIdx++) {
        SDWORD lRow = inputSDWORD();
        SDWORD lCol = inputSDWORD();
        SQWORD sqIter = inputSQWORD();

        SDWORD lAns;
        if (-1 == aalChgStartCnt[lRow][lCol]) {
            if (lInitCells[lRow][lCol] == 1) {
                lAns = 0;
            } else {
                lAns = 1;
            }
        } else {
            SDWORD lPhase = 0;
            if (aalChgStartCnt[lRow][lCol] < sqIter) {
                lPhase = (sqIter - (SQWORD)aalChgStartCnt[lRow][lCol]) % 2;
            }

            if (0 == lPhase) {
                if (lInitCells[lRow][lCol] == 1) {
                    lAns = 0;
                } else {
                    lAns = 1;
                }
            } else {
                if (lInitCells[lRow][lCol] == 1) {
                    lAns = 1;
                } else {
                    lAns = 0;
                }
            }
        }
        printf("%d\n", lAns);
    }


    return 0;
}