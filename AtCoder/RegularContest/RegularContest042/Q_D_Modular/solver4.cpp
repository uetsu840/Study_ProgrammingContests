#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <math.h>
#include <queue>
#include <algorithm>
#include <map>
#include <assert.h>
using namespace std;

#define unittest_name_helper(counter) unittest_ ## counter
#define unittest_name(counter) unittest_name_helper(counter)
#define unittest __attribute__((constructor)) void unittest_name(__COUNTER__) ()

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

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0xFFFFFFFFFFFFFFFF)

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

static DWORD powMod(DWORD x, DWORD k, DWORD m)
{
    DWORD dwRes = 1;
    while (k > 0) {
        if (k & 1) {
            dwRes = ((QWORD)dwRes * (QWORD)x) % (QWORD)m;
            --k;
        } else {
            x = ((QWORD)x * (QWORD)x) % (QWORD)m;
            k >>= 1;
        }   
    }
    return dwRes % m;
}

static DWORD modInv(DWORD g, DWORD p) {
    return powMod(g, p - 2, p);
}

static DWORD normalSearch(DWORD dwInput_X, DWORD dwInput_P, DWORD dwInput_A, DWORD dwInput_B)
{
    DWORD dwMin = powMod(dwInput_X, dwInput_A, dwInput_P);
    for (DWORD dwPow = dwInput_A+1; dwPow <= dwInput_B; dwPow++) {
        DWORD dwMod = powMod(dwInput_X, dwPow, dwInput_P);
        dwMin = MIN(dwMin, dwMod);
    }

    return dwMin;
}

/**
*   @brief  baby step giant step algorithm
*/
DWORD baby_step_giant_step(DWORD g, DWORD y, DWORD p) {
    DWORD sqrt_p = sqrt(p);
 
    unordered_map<DWORD, DWORD> baby;
    DWORD gb = 1;
    for (DWORD b = 0; b < sqrt_p + 3; b++) {
        baby[gb] = b;
        gb = (QWORD)gb * g % p;
    }
 
    DWORD g_sqrt_p_inv = modInv(powMod(g, sqrt_p, p), p);
    DWORD giant = y;
    for (DWORD a= 0; a < sqrt_p + 3; a++) {
        if (baby.count(giant)) {
            DWORD b = baby[giant];
            DWORD x = a * sqrt_p + b;
            return x % p;
        }
        giant = (QWORD)giant * g_sqrt_p_inv % p;
    }
    return -1;
}
 
unittest {
    assert (baby_step_giant_step( 3, powMod( 3,  0,  17),  17) ==  0);
    assert (baby_step_giant_step( 3, powMod( 3, 12,  17),  17) == 12);
    assert (baby_step_giant_step(12, powMod(12, 17, 101), 101) == 17);
}


static DWORD solveWithBabyGiant(DWORD dwInput_X, DWORD dwInput_P, DWORD dwInput_A, DWORD dwInput_B)
{
    for (DWORD dwRem = 1; dwRem <= MAX_DWORD; dwRem++) {
        DWORD dwCandidate = baby_step_giant_step(dwInput_X, dwRem, dwInput_P);
        if (MAX_DWORD != dwCandidate) {
            if ((dwInput_A <= dwCandidate) && (dwCandidate <= dwInput_B)) {
                return dwRem;
            }
        }
    }
}


int main()
{
    DWORD dwInput_X, dwInput_P, dwInput_A, dwInput_B;

    dwInput_X = inputSDWORD();
    dwInput_P = inputSDWORD();
    dwInput_A = inputSDWORD();
    dwInput_B = inputSDWORD();

    DWORD dwAns;

    if (dwInput_P <= dwInput_B - dwInput_A) {
        printf("1\n");
        return 0;
    }

    if (dwInput_B - dwInput_A <= 10000000) {
        dwAns = normalSearch(dwInput_X, dwInput_P, dwInput_A, dwInput_B); 
    } else {
        dwAns = solveWithBabyGiant(dwInput_X, dwInput_P, dwInput_A, dwInput_B);
    }
    printf("%d\n", dwAns);

    return 0;
}
