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

QWORD mul_mod(QWORD a, QWORD b, QWORD div)
{
    QWORD ret;
    ret = ((QWORD)(a % div) * (QWORD)(b % div));
    ret %= div;
    return ret;
}

QWORD power_mod(QWORD x, QWORD y, QWORD div)
{
    if (y == 0) {
        return 1;
    } else if (y == 1) {
        return x % div;
    } else if ((y % 2) == 0) {
        QWORD tmp = power_mod(x, y/2, div);
        QWORD ret = (tmp * tmp) % (QWORD)div;
        return ret;
    } else {
        QWORD tmp = power_mod(x, y/2, div);
        QWORD ret = ((tmp * tmp) * x) % (QWORD)div;
        return ret;
    }
}

QWORD div_mod(QWORD a, QWORD b, QWORD div)
{
    return mul_mod(a, power_mod(b, div-2, div), div);
}

int main(void) 
{
    printf("%d\n", div_mod(26, 2, 7));
    printf("%d\n", div_mod(10, 2, DIVISOR));
    printf("%d\n", div_mod(45000000000, 5, DIVISOR));
    printf("%d\n", div_mod(3000000000, 2, DIVISOR));
    return 0;
}



