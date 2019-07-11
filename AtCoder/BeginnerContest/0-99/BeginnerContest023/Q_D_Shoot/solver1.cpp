#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <algorithm>    // std::binary_search, std::sort
#include <vector>       // std::vector
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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

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

#define MAX_BALOON_NUM (100000)
static DWORD s_adwInitHeight[MAX_BALOON_NUM];
static DWORD s_adwSpeed[MAX_BALOON_NUM];
static DWORD s_dwInput_N;

bool isShootable(SQWORD sqHeight) {
    QWORD qwHeight = (QWORD)sqHeight;
    DWORD adwLimitTime[MAX_BALOON_NUM];

    for (DWORD dwIdx = 0; dwIdx < s_dwInput_N; dwIdx++) {
        QWORD qwLimitTime = (qwHeight - (QWORD)(s_adwInitHeight[dwIdx])) / (QWORD)(s_adwSpeed[dwIdx]);
        if (qwHeight < (QWORD)s_adwInitHeight[dwIdx]) {
            return false;
        }
        if (qwLimitTime <= (QWORD)MAX_DWORD) {
            adwLimitTime[dwIdx] = (DWORD)qwLimitTime;
        } else { 
            adwLimitTime[dwIdx] = MAX_DWORD;
        }
    }
    vector<DWORD> v(adwLimitTime, adwLimitTime + s_dwInput_N);
    std::sort(v.begin(), v.end());

    for (DWORD dwIdx = 0; dwIdx < s_dwInput_N; dwIdx++) {
        if (v[dwIdx] < dwIdx) {
            return false;
        }
    }
    return true;
}


static SQWORD binarySearch(bool (*judge)(SQWORD))
{
    SQWORD sqUb = MAX_SQWORD / 2LL;
    SQWORD sqLb = 0;

    while (1LL < sqUb - sqLb) {
        SQWORD sqMid = (sqUb + sqLb) / 2LL;
        if (judge((QWORD)sqMid)) {
            sqUb = sqMid;
        } else {
            sqLb = sqMid;
        }
    }
    return sqUb;
}


int main()
{
    s_dwInput_N = inputSDWORD();

    for (DWORD dwIdx = 0; dwIdx < s_dwInput_N; dwIdx++) {
        s_adwInitHeight[dwIdx] = inputSDWORD();
        s_adwSpeed[dwIdx] = inputSDWORD();
    }
    printf("%lld\n", binarySearch(isShootable));

    return 0;
}
