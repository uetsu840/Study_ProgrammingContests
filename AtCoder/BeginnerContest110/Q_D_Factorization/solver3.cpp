

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
    DWORD ret;
    ret = a+b;

    if (DIVISOR <= ret) {
        return ret - DIVISOR;
    } else {
        return ret;
    }
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
#define MAX_INDEX_NUM   (10)

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

static DWORD calcCumSum(DWORD *pdwIndice, const DWORD *pdwUpper, DWORD dwCurDepth)
{
    DWORD dwCurUpper;
    dwCurUpper = *(pdwUpper + dwCurDepth);
    
    if (dwCurDepth == s_dwMaxDepth) {
        DWORD dwDpTblIdx = getDpTblIdx(pdwIndice);
        return s_adwDpTbl[dwDpTblIdx]; 
    }

    DWORD dwSum = 0;
    for (DWORD dwCurIdx = 0; dwCurIdx <= dwCurUpper; dwCurIdx++) {
        *(pdwIndice + dwCurDepth) = dwCurIdx;
        DWORD dwSumSub = calcCumSum(pdwIndice, pdwUpper, dwCurDepth + 1);
        dwSum = add_mod(dwSum, dwSumSub);
    }
    return dwSum;
}


/**
* DPテーブルの更新
*/
static void updateDp(DWORD *pdwIndice, DWORD dwCurDepth)
{
    DWORD dwCurUpper;
    dwCurUpper = s_adwFactoryCnt[dwCurDepth];
    
    if (dwCurDepth == s_dwMaxDepth) {
        DWORD adwIndiceWork[MAX_INDEX_NUM];
        for (DWORD dwIdx = 0; dwIdx < ArrayLength(adwIndiceWork); dwIdx++) {
            adwIndiceWork[dwIdx] = 0;
        }
        DWORD dwCumSum = calcCumSum(adwIndiceWork, pdwIndice, 0);
        DWORD dwDpTblIdx = getDpTblIdx(pdwIndice);
        s_adwDpTblNext[dwDpTblIdx] = dwCumSum;
        return;
    }

    for (DWORD dwCurIdx = 0; dwCurIdx <= dwCurUpper; dwCurIdx++) {
        *(pdwIndice + dwCurDepth) = dwCurIdx;
        updateDp(pdwIndice, dwCurDepth + 1);
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

//    printDwordArray(s_adwFactoryCnt, s_dwFactoryNum);
//    printDwordArray(s_adwFactoryCumMul, s_dwFactoryNum);
    DWORD dwDpTblSize = sizeof(DWORD) * (getDpTblIdx(s_adwFactoryCnt) + 1);
    for (DWORD dwCurN = 2; dwCurN <= dwInput_N; dwCurN++) {
        for (DWORD dwIdx = 0; dwIdx < ArrayLength(adwCurIndice); dwIdx++) {
            adwCurIndice[dwIdx] = 0;
        }
        updateDp(adwCurIndice, 0);
        memcpy(s_adwDpTbl, s_adwDpTblNext, dwDpTblSize);
    }
    printf("%d\n", s_adwDpTbl[getDpTblIdx(s_adwFactoryCnt)]);

    return 0;
}
