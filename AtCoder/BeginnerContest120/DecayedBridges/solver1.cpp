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

/* ------------------------------------------------------------- */

typedef struct {
    DWORD   *pdwPar;
    DWORD   *pdwRank;
    DWORD   *pdwCnt;
    DWORD   dwSize;
} ST_UNION_FIND;

static void initUnionFind(
    ST_UNION_FIND *pstUf, 
    DWORD *pdwPar, 
    DWORD *pdwRank,
    DWORD *pdwCnt,
    DWORD dwSize)
{
    pstUf->dwSize = dwSize;
    pstUf->pdwPar = pdwPar;
    pstUf->pdwRank = pdwRank;
    pstUf->pdwCnt = pdwCnt;

    for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
        *(pstUf->pdwPar + dwIdx)  = dwIdx;
        *(pstUf->pdwRank + dwIdx) = 0;
        *(pstUf->pdwCnt + dwIdx)  = 1;
    }
}

static inline DWORD ufGetParent(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    return *(pstUf->pdwPar + dwIdx);
}

static inline DWORD ufGetRank(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    return *(pstUf->pdwRank + dwIdx);
}

static inline void ufSetParent(const ST_UNION_FIND *pstUf, DWORD dwIdx, DWORD dwParent)
{
    *(pstUf->pdwPar + dwIdx) = dwParent; 
    if (ufGetRank(pstUf, dwIdx) == ufGetRank(pstUf, dwParent)) {
        (*(pstUf->pdwRank + dwParent))++;
    }
}

static inline DWORD ufGetRoot(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    if (ufGetParent(pstUf, dwIdx) == dwIdx) {
        return dwIdx;
    } else {
        DWORD dwParent = ufGetParent(pstUf, dwIdx);
        DWORD dwRoot = ufGetRoot(pstUf, dwParent);
        return dwRoot;
    }
}

static inline void ufUnite(ST_UNION_FIND *pstUf, DWORD dwX, DWORD dwY)
{
    DWORD dwRootX = ufGetRoot(pstUf, dwX);
    DWORD dwRootY = ufGetRoot(pstUf, dwY);

    if (dwRootX == dwRootY) {
        return;
    }

    if (ufGetRank(pstUf, dwRootX) < ufGetRank(pstUf, dwRootY)) {
        ufSetParent(pstUf, dwRootX, dwRootY);
        (*(pstUf->pdwCnt + dwRootY)) += (*(pstUf->pdwCnt + dwRootX));
    } else {
        ufSetParent(pstUf, dwRootY, dwRootX);
        (*(pstUf->pdwCnt + dwRootX)) += (*(pstUf->pdwCnt + dwRootY));
    }
}

static inline M_BOOL ufIsSame(const ST_UNION_FIND *pstUf, DWORD dwX, DWORD dwY)
{
    return (ufGetRoot(pstUf, dwX)  == ufGetRoot(pstUf, dwY));
}

/* ------------------------------------------------------------- */

#define N_MAX_ISLANDS   (100001)
#define N_MAX_BRIDGES   (100001)

typedef struct {
    SDWORD  lFrom;
    SDWORD  lTo;
} ST_BRIDGE_ONE;

int main()
{
    SDWORD lNumIslands = inputSDWORD();
    SDWORD lNumBridges = inputSDWORD();
    vector<ST_BRIDGE_ONE> vecBridges;

    for (DWORD dwIdx = 0; dwIdx < lNumBridges; dwIdx++) {
        ST_BRIDGE_ONE stBridge;

        stBridge.lFrom = inputSDWORD();
        stBridge.lTo   = inputSDWORD();
        vecBridges.emplace_back(stBridge);
    }
    reverse(vecBridges.begin(), vecBridges.end());
    
    static DWORD adwIslands_Parents[N_MAX_ISLANDS];
    static DWORD adwIslands_Rank[N_MAX_ISLANDS];
    static DWORD adwIslands_Cnt[N_MAX_ISLANDS];

    ST_UNION_FIND stUfIslands;

    initUnionFind(&stUfIslands, 
                    adwIslands_Parents,
                    adwIslands_Rank,
                    adwIslands_Cnt, ArrayLength(adwIslands_Parents));

    static SQWORD s_asqGroupCnt[N_MAX_BRIDGES];
    SQWORD sqGroupCnt = 0;   
    for (auto it = vecBridges.begin(); it != vecBridges.end(); ++it) {
        SDWORD lFrom = it->lFrom;
        SDWORD lTo = it->lTo;

        if (ufIsSame(&stUfIslands, lFrom, lTo)) {
            ;
        } else {
            SDWORD lRoot1 = ufGetRoot(&stUfIslands, lFrom);
            SDWORD lRoot2 = ufGetRoot(&stUfIslands, lTo);
            SQWORD sqCnt1 = adwIslands_Cnt[lRoot1];
            SQWORD sqCnt2 = adwIslands_Cnt[lRoot2];

            SQWORD sqCntDiff = (sqCnt1 + sqCnt2) * (sqCnt1 + sqCnt2 - 1LL) / 2LL
                                - (sqCnt1 * (sqCnt1 - 1LL)) / 2LL
                                - (sqCnt2 * (sqCnt2 - 1LL)) / 2LL;
        
            ufUnite(&stUfIslands, lRoot1, lRoot2);
            sqGroupCnt += sqCntDiff;
        }
        DWORD dwIdx = it - vecBridges.begin() + 1;
        s_asqGroupCnt[dwIdx] = sqGroupCnt;
    }

    SQWORD sqBaseNum = (SQWORD)lNumIslands * (SQWORD)(lNumIslands - 1LL) / 2LL;
    for (DWORD dwIdx = 0; dwIdx < lNumBridges; dwIdx++) {
        printf("%lld\n", sqBaseNum - s_asqGroupCnt[lNumBridges - dwIdx - 1]);
    }

    return 0;
}