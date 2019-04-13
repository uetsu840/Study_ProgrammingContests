

#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
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

#define MAX_FACTORY (60)
static DWORD s_adwFactoryCnt[MAX_FACTORY];
static DWORD s_adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

M_BOOL calc_PrimeFactorization(DWORD dwNum)
{
    DWORD dwCurNum = dwNum;
    DWORD dwCeiling = (DWORD)(sqrt(dwNum) + 1);

    DWORD dwFactoryIdx = 0;
    for (DWORD dwDiv = 2; dwDiv <= dwCeiling; dwDiv++) {
        if (0 == dwCurNum % dwDiv) {
            DWORD dwFactoryCnt = 0;
            while(1) {
                dwCurNum /= dwDiv;
                dwFactoryCnt++;
                if (0 != dwCurNum % dwDiv) {
                    break;
                }   
            }
            s_adwFactoryCnt[s_dwFactoryNum] = dwFactoryCnt;
//            printf(" --> [%d] %d %d\n", s_dwFactoryNum, dwDiv, dwFactoryCnt);
            s_dwFactoryNum++;
        }

        if (1 == dwCurNum) {
            break;
        }
    }

    return M_TRUE;
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

DWORD add_mod(DWORD a, DWORD b)
{
    return (a + b) % DIVISOR;
}

DWORD mul_mod(DWORD a, DWORD b)
{
    return ((a % DIVISOR) * (b % DIVISOR)) % DIVISOR;
}

DWORD power_mod(DWORD x, DWORD y)
{
    if (y == 0) {
        return 1;
    } else if (y == 1) {
        return x % DIVISOR;
    } else if ((y % 2) == 0) {
        DWORD tmp = power_mod(x, y/2);
        return (tmp * tmp) % DIVISOR;
    } else {
        DWORD tmp = power_mod(x, y/2);
        return ((tmp * tmp) * x) % DIVISOR;
    }
}

DWORD div_mod(DWORD a, DWORD b)
{
    return mul_mod(a, power_mod(b, DIVISOR-2));
}

/**
 *  DWORD 配列のprintf
 */
static void printDwordArray(DWORD *pdwData, DWORD dwNum)
{
    DWORD dwIdx;
    printf("[%d] ----[", dwNum);
    for (dwIdx = 0; dwIdx < dwNum; dwIdx++) {
        printf("%d ", *(pdwData + dwIdx));
    }
    printf("]\n");
}

/**
 *  2^28 > 
 * 2^30 = 1073741824
 * 3^20 = 3486784401
 * 5^14 = 6103515625
 * 7^11 = 1977326743
 * DPテーブルのサイズは  (30+1) * (20+1) * (14+1) * (11+1) あればよい。 
 */
#define DPTBL_SIZE   (1000000)

static DWORD s_dwMaxDepth;
static DWORD s_adwDpTbl[DPTBL_SIZE];
static DWORD s_adwDpTblNext[DPTBL_SIZE];

static DWORD getDpTblIdx(DWORD *pdwIndices)
{
    DWORD dwRet = 0;
    DWORD dwIdx = 0;
    for (dwIdx = 0; dwIdx < s_dwMaxDepth; dwIdx++) {
        dwRet += pdwIndices[dwIdx] * s_adwFactoryCumMul[dwIdx];
    }
    return dwRet;
}

#define MAX_INDEX_NUM   (10)

static DWORD getSubIndiceDpTbl(DWORD *pdwIndices, DWORD *pdwUpper)
{
    DWORD adwSubIdx[MAX_INDEX_NUM];

    /* インデックスを反転する */
    for (DWORD dwIdx = 0; dwIdx <= s_dwMaxDepth; dwIdx++) {
        adwSubIdx[dwIdx] = *(pdwUpper + dwIdx) - *(pdwIndices + dwIdx);
    }
    return s_adwDpTbl[getDpTblIdx(adwSubIdx)];
}



static DWORD getDpTblEntry(DWORD *pdwIndice, DWORD *pdwUpper, DWORD dwCurDepth)
{
    DWORD dwDp = 0;
    DWORD dwIdx;
    DWORD dwCurUpper;
    
    dwCurUpper = *(pdwUpper + dwCurDepth);
    
    if (dwCurDepth == s_dwMaxDepth) {
        DWORD dwDpSub = getSubIndiceDpTbl(pdwIndice, pdwUpper);
        return dwDpSub;
    }

    for (dwIdx = 0; dwIdx <= dwCurUpper; dwIdx++) {
        *(pdwIndice + dwCurDepth) = dwIdx;
        DWORD dwDpSub = getDpTblEntry(pdwIndice, pdwUpper, dwCurDepth + 1);
        dwDp = add_mod(dwDp, dwDpSub); 
    }

    return dwDp;
}

/**
* DPテーブルの更新
*/
static void getNextDp(DWORD *pdwIndice, DWORD dwCurDepth)
{
    DWORD dwCurUpper;
    DWORD adwWorkIndice[MAX_INDEX_NUM];
    dwCurUpper = s_adwFactoryCnt[dwCurDepth];
    DWORD dwDp;
//    printf("getNextDp  depth[%d] upper[%d]\n", dwCurDepth, dwCurUpper);
    
    if (dwCurDepth == s_dwMaxDepth) {
        memset(adwWorkIndice, 0, sizeof(adwWorkIndice));
//        printDwordArray(pdwIndice, s_dwFactoryNum);
        DWORD dp = getDpTblEntry(adwWorkIndice, pdwIndice, 0);
//        printf("dp = %d\n", dp);
        s_adwDpTblNext[getDpTblIdx(pdwIndice)] = dp;
        return;
    }

    for (DWORD dwIdx = 0; dwIdx <= dwCurUpper; dwIdx++) {
        *(pdwIndice + dwCurDepth) = dwIdx;
        getNextDp(pdwIndice, dwCurDepth + 1);
    }
}


/**
 * 【参考】
 * 2*3*5*7*11*13*17*19*23*29 = 6469693230
 * なので約数の種類はたかだか10個。
 * 2^28 で 10^9 を超えるので、1個の約数のループはたかだか28回。
 */
int main()
{
    DWORD dwInput_N, dwInput_M;
    DWORD adwCurIndice[MAX_INDEX_NUM];
 
    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    static DWORD s_adwDpTable[1024 * 1024 * 32];

    calc_PrimeFactorization(dwInput_M);

    /* calc cummul */
    DWORD dwCumMul = 1;
    for (DWORD dwIdx = 0; dwIdx < s_dwFactoryNum; dwIdx++) {
        DWORD dwCur = s_dwFactoryNum - dwIdx - 1;
        s_adwFactoryCumMul[dwCur] = dwCumMul;
        dwCumMul *= (s_adwFactoryCnt[dwCur] + 1);
    }

    s_dwMaxDepth = s_dwFactoryNum;
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_adwDpTbl); dwIdx++) {
        s_adwDpTbl[dwIdx] = 1;
    }
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(adwCurIndice); dwIdx++) {
        adwCurIndice[dwIdx] = 0;
    }

//    printDwordArray(s_adwFactoryCnt, s_dwFactoryNum);
//    printDwordArray(s_adwFactoryCumMul, s_dwFactoryNum);
    DWORD dwDpTblSize = sizeof(DWORD) * (getDpTblIdx(s_adwFactoryCnt) + 1);
    for (DWORD dwCurN = 0; dwCurN < dwInput_N-1; dwCurN++) {
        getNextDp(adwCurIndice, 0);
        memcpy(s_adwDpTbl, s_adwDpTblNext, dwDpTblSize);
//        printf("---- step[%d]\n", dwCurN);
    }
    printf("%d", s_adwDpTbl[getDpTblIdx(s_adwFactoryCnt)]);

    return 0;
}
