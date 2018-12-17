/**
 * 解説見た。
 */
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

#define MAX_FACTORY (100)
static DWORD s_adwFactoryCnt[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

M_BOOL calc_PrimeFactorization(DWORD dwNum)
{
    DWORD dwCurNum = dwNum;
    DWORD dwCeiling = (DWORD)(sqrt(dwNum) + 1);

    for (DWORD dwDiv = 2; dwDiv <= dwCeiling; dwDiv++) {
        if (0 == (dwCurNum % dwDiv)) {
            DWORD dwFactoryCnt = 0;
            while(1) {
                dwCurNum /= dwDiv;
                dwFactoryCnt++;
                if (0 != dwCurNum % dwDiv) {
                    break;
                }   
            }
            s_adwFactoryCnt[s_dwFactoryNum] = dwFactoryCnt;
            s_dwFactoryNum++;
        }
    }
    if (1 != dwCurNum) {
        s_adwFactoryCnt[s_dwFactoryNum] = 1;
        s_dwFactoryNum++;
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
    return 0;
}

DWORD mul_mod(DWORD a, DWORD b)
{
    QWORD qwTmp = (QWORD)(a % DIVISOR) * (QWORD)(b % DIVISOR);
    return (DWORD)(qwTmp % (QWORD)DIVISOR);
}

DWORD power_mod(DWORD x, DWORD y)
{
    if (y == 0) {
        return 1;
    } else if (y == 1) {
        return x % DIVISOR;
    } else if ((y % 2) == 0) {
        DWORD tmp = power_mod(x, y/2);
        QWORD qwTmp_2 = (QWORD)tmp * (QWORD)tmp;
        DWORD dwRet = qwTmp_2 % (QWORD)DIVISOR;
        return dwRet;
    } else {
        DWORD tmp = power_mod(x, y/2);
        QWORD qwTmp_2 = (QWORD)tmp * (QWORD)tmp;
        DWORD dwRet = ((qwTmp_2 % (QWORD)DIVISOR) * (QWORD)x) % DIVISOR;
        return dwRet;
    }
}

DWORD div_mod(DWORD a, DWORD b)
{
    return mul_mod(a, power_mod(b, DIVISOR-2));
}


/**
 *  Combinationの計算
 */
static DWORD calcCombination(DWORD dwM, DWORD dwN)
{
    DWORD dwIdx;
    DWORD dwTmp;
    DWORD dwNum = 1;
    DWORD dwDen = 1;
    for (dwIdx = 1; dwIdx <= dwN; dwIdx++) {
        dwDen = mul_mod(dwDen, dwIdx);
    }
    for (dwIdx = 0; dwIdx < dwN; dwIdx++) {
        dwNum = mul_mod(dwNum, (dwM - dwIdx));
    }
    DWORD ret = div_mod(dwNum, dwDen);
    return ret;
}

int main()
{
    DWORD dwInput_N, dwInput_M;
 
    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    memset(s_adwFactoryCnt, 0, sizeof(s_adwFactoryCnt));

    calc_PrimeFactorization(dwInput_M);

    DWORD dwAns = 1;
    for (DWORD dwIdx = 0; dwIdx < s_dwFactoryNum; dwIdx++) {
        DWORD dwFactory = s_adwFactoryCnt[dwIdx];
        DWORD dwComb = calcCombination(dwFactory + dwInput_N - 1, dwFactory);
        dwAns = mul_mod(dwAns, dwComb);
    }
    printf("%d", dwAns);

    return 0;
}
