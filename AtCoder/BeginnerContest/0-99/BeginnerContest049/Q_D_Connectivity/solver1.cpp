#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using BYTE = unsigned char;
using SBYTE = char;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MAX_QWORD (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFF)
#define MAX_WORD  (0xFFFF)
#define MAX_BYTE  (0xFF)


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

static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber;
            }
        }
    }
}

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



#define MAX_CITY_NUM         (200000)
#define CITY_INFO_ARY_SIZE   (200000 + 1)

int main()
{
    DWORD dwInput_N;
    DWORD dwInput_K;
    DWORD dwInput_L;

    static DWORD adwRoad_Parents[CITY_INFO_ARY_SIZE];
    static DWORD adwRoad_Rank[CITY_INFO_ARY_SIZE];
    static DWORD adwRoad_Cnt[CITY_INFO_ARY_SIZE];
    static DWORD adwRail_Parents[CITY_INFO_ARY_SIZE];
    static DWORD adwRail_Rank[CITY_INFO_ARY_SIZE];
    static DWORD adwRail_Cnt[CITY_INFO_ARY_SIZE];
    static DWORD adwCon_Parents[CITY_INFO_ARY_SIZE];
    static DWORD adwCon_Rank[CITY_INFO_ARY_SIZE];
    static DWORD adwCon_Cnt[CITY_INFO_ARY_SIZE];

    ST_UNION_FIND stUfRoad;
    ST_UNION_FIND stUfRail;
    ST_UNION_FIND stUfCon;

    initUnionFind(&stUfRoad, adwRoad_Parents, adwRoad_Rank, adwRoad_Cnt, ArrayLength(adwRoad_Parents));
    initUnionFind(&stUfRail, adwRail_Parents, adwRail_Rank, adwRail_Cnt, ArrayLength(adwRail_Parents));
    initUnionFind(&stUfCon, adwCon_Parents, adwCon_Rank, adwCon_Cnt, ArrayLength(adwCon_Parents));

    dwInput_N = inputSDWORD();
    dwInput_K = inputSDWORD();
    dwInput_L = inputSDWORD();

    for (DWORD dwIdx = 0; dwIdx < dwInput_K; dwIdx++) {
        DWORD dwInput_p = inputSDWORD();
        DWORD dwInput_q = inputSDWORD(); 
        ufUnite(&stUfRoad, dwInput_p, dwInput_q);
    }
    for (DWORD dwIdx = 0; dwIdx < dwInput_L; dwIdx++) {
        DWORD dwInput_r = inputSDWORD();
        DWORD dwInput_s = inputSDWORD(); 
        ufUnite(&stUfRail, dwInput_r, dwInput_s);
    }
    for (DWORD dwIdxJ = 1; dwIdxJ < dwInput_N; dwIdxJ++) {
        for (DWORD dwIdxK = dwIdxJ+1; dwIdxK <= dwInput_N; dwIdxK++) {
            if (ufIsSame(&stUfRoad, dwIdxJ, dwIdxK) && ufIsSame(&stUfRail, dwIdxJ, dwIdxK)) {
                ufUnite(&stUfCon, dwIdxJ, dwIdxK);
            }
        }
    }

    for (DWORD dwIdxJ = 1; dwIdxJ <= dwInput_N; dwIdxJ++) {
        printf("%d", *(stUfCon.pdwCnt + ufGetRoot(&stUfCon, dwIdxJ)));
        if (dwIdxJ != dwInput_N) {
            printf(" ");
        }
    }
    printf("\n");
}
