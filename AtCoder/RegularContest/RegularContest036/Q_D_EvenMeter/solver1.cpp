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
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    vector<DWORD>   vdwFriend;
    DWORD   dwSize;
} ST_UNION_FIND;

static void initUnionFind(
    ST_UNION_FIND *pstUf, 
    DWORD dwSize)
{
    pstUf->dwSize = dwSize;
    pstUf->vdwPar.resize(dwSize);
    pstUf->vdwRank.resize(dwSize);
    pstUf->vdwCnt.resize(dwSize);
    pstUf->vdwFriend.resize(dwSize);
 
    for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
        pstUf->vdwPar[dwIdx]  = dwIdx;
        pstUf->vdwRank[dwIdx] = 0;
        pstUf->vdwCnt[dwIdx]  = 1;
        pstUf->vdwFriend[dwIdx] = 0;
    }
}


static inline DWORD ufGetParent(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    return pstUf->vdwPar[dwIdx];
}

static inline DWORD ufGetRank(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    return pstUf->vdwRank[dwIdx];
}

static inline void ufSetParent(ST_UNION_FIND *pstUf, DWORD dwIdx, DWORD dwParent)
{
    pstUf->vdwPar[dwIdx] = dwParent; 
    if (ufGetRank(pstUf, dwIdx) == ufGetRank(pstUf, dwParent)) {
        (pstUf->vdwRank[dwParent])++;
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

static inline DWORD ufGetFriend(const ST_UNION_FIND *pstUf, DWORD dwIdx)
{
    DWORD dwRoot = ufGetRoot(pstUf, dwIdx);
    return pstUf->vdwFriend[dwRoot];
}

static inline void ufSetFriend(ST_UNION_FIND *pstUf, DWORD dwIdx, DWORD dwFriend)
{
    DWORD dwRoot = ufGetRoot(pstUf, dwIdx);
    pstUf->vdwFriend[dwRoot] = dwFriend;
}

static inline void ufUnite(ST_UNION_FIND *pstUf, DWORD dwX, DWORD dwY)
{
    DWORD dwRootX = ufGetRoot(pstUf, dwX);
    DWORD dwRootY = ufGetRoot(pstUf, dwY);

    DWORD dwFriendX = ufGetFriend(pstUf, dwX);
    DWORD dwFriendY = ufGetFriend(pstUf, dwY);

    if (dwRootX == dwRootY) {
        return;
    }

    if (ufGetRank(pstUf, dwRootX) < ufGetRank(pstUf, dwRootY)) {
        ufSetParent(pstUf, dwRootX, dwRootY);
        (pstUf->vdwCnt[dwRootY]) += (pstUf->vdwCnt[dwRootX]);
    } else {
        ufSetParent(pstUf, dwRootY, dwRootX);
        (pstUf->vdwCnt[dwRootX]) += (pstUf->vdwCnt[dwRootY]);
    }

    if ((0 != dwFriendX) && (0 != dwFriendY)) {
        ufUnite(pstUf, dwFriendX, dwFriendY);
    } else if (0 != dwFriendX) {
        ufSetFriend(pstUf, dwRootX, dwFriendX);
    } else if (0 != dwFriendY) {
        ufSetFriend(pstUf, dwRootX, dwFriendY);
    }
}

static inline M_BOOL ufIsSame(const ST_UNION_FIND *pstUf, DWORD dwX, DWORD dwY)
{
    return (ufGetRoot(pstUf, dwX)  == ufGetRoot(pstUf, dwY));
}

/* ------------------------------------------------------------- */

#define N_MAX_NODE   (100001)
#define N_MAX_EDGE   (100001)

typedef struct {
    SDWORD  lFrom;
    SDWORD  lTo;
} ST_EDGE_ONE;

int main()
{
    SDWORD lInput_N = inputSDWORD();
    SDWORD lInput_Q = inputSDWORD();
    
    ST_UNION_FIND stUfIslands;

    initUnionFind(&stUfIslands, lInput_N + 1);

    for (SDWORD lIdx = 0; lIdx < lInput_Q; lIdx++) {
        SDWORD lInput_w = inputSDWORD();
        SDWORD lInput_x = inputSDWORD();
        SDWORD lInput_y = inputSDWORD();
        SDWORD lInput_z = inputSDWORD();

        if (1 == lInput_w) {
            /* add road */
//            printf("add road %d %d\n", lInput_x, lInput_y);
            if (0 == (lInput_z % 2)) {
                /* even */
//                printf("u1 %d %d\n", lInput_x, lInput_y);
                ufUnite(&stUfIslands, lInput_x, lInput_y);
            } else {
                /* odd */
                DWORD dwFriendX = ufGetFriend(&stUfIslands, lInput_x);
                DWORD dwFriendY = ufGetFriend(&stUfIslands, lInput_y);

                if (0 == dwFriendX) {
                    ufSetFriend(&stUfIslands, lInput_x, lInput_y);
                } else {
//                    printf("u2 %d %d\n", dwFriendX, lInput_y);
                    ufUnite(&stUfIslands, dwFriendX, lInput_y);
                }

                if (0 == dwFriendY) {
                    ufSetFriend(&stUfIslands, lInput_y, lInput_x);
                } else {
//                    printf("u3 %d %d\n", dwFriendY, lInput_x);
                    ufUnite(&stUfIslands, dwFriendY, lInput_x);
                }
            }

        } else if (2 == lInput_w) {
            /* query */
            if (ufIsSame(&stUfIslands, lInput_x, lInput_y)) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }
        } else {
            printf("Oops!\n");
        }
    }

    return 0;
}