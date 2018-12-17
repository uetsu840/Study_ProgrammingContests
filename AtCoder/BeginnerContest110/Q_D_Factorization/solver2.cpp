/**
 *  solver2.cpp
 *      2018/12/17
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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

M_BOOL calc_PrimeFactorization(DWORD dwNum)
{
    DWORD dwCurNum = dwNum;
    DWORD dwCeiling = (DWORD)(sqrt(dwNum) + 1);

    DWORD dwFactoryIdx = 0;
    DWORD dwCumMul = 1;
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
            adwFactoryNum[s_dwFactoryNum] = dwFactoryCnt;
            adwFactoryCumMul[s_dwFactoryNum] = dwCumMul;
            dwCumMul *= (dwFactoryCnt + 1);
            s_dwFactoryNum++;
            printf(" --> %d %d\n", dwDiv, dwFactoryCnt);
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


DWORD calcBlockCombination(DWORD dwNumber, DWORD dwBlock)
{
    printf("------ %d %d\n", dwNumber, dwBlock);
    DWORD dwAnswer = 1;
    DWORD dwTmp = dwNumber + (dwBlock - 1);
    for (DWORD dwIdx = 0; dwIdx < dwNumber; dwIdx++) {
        dwAnswer = mul_mod(dwAnswer, dwTmp - dwIdx);    
        printf("------------%d\n", dwAnswer);
    }

    for (DWORD dwIdx = 1; dwIdx <= dwNumber; dwIdx++) {
        dwAnswer = div_mod(dwAnswer, dwIdx);
        printf("/-----------%d\n", dwAnswer);
    }

    return dwAnswer;
}

int main()
{
    DWORD dwInput_N, dwInput_M;
 
    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    static DWORD s_adwDpTable[1024 * 1024 * 32];

    calc_PrimeFactorization(dwInput_M);

    for (DWORD dwIdx = 0; dwIdx < s_dwFactoryNum; dwIdx++) {
        printf("== prime factory %d ^ %d\n", adwFactoryNum[dwIdx], adwFactoryCumMul[dwIdx]);
    }

    DWORD dwAnswer = 1;
    for (DWORD dwFactoryIdx = 0; dwFactoryIdx < s_dwFactoryNum; dwFactoryIdx++) {
        dwAnswer = mul_mod(dwAnswer, calcBlockCombination(adwFactoryNum[dwFactoryIdx], dwInput_N));
        printf(">> %d\n", dwAnswer);
    } 
    printf("%d\n", dwAnswer);

    printf("%d\n", div_mod(3000000000, 2));
    return 0;
}
