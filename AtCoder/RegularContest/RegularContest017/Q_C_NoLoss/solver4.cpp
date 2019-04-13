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

#define MAX_OBJECT_NUM    (32)
static DWORD s_adwSize[MAX_OBJECT_NUM];
static DWORD s_adwRevCumSum[MAX_OBJECT_NUM];
static SQWORD s_aqwSumsU[65536];
static SQWORD s_aqwSumsD[65536];



DWORD lookup(vector<QWORD>& v, const SQWORD& x)
{
    pair<vector<QWORD>::iterator, vector<QWORD>::iterator> range;
    range = equal_range(v.begin(), v.end(), x);

    DWORD dwIdx1 = range.first - v.begin();
    DWORD dwIdx2 = range.second - v.begin();

//    printf("%d %d\n", dwIdx1, dwIdx2);
	return dwIdx2 - dwIdx1;
}


DWORD solve(DWORD dwInput_X, DWORD dwInput_N) {
    DWORD dwSum = 0;
    DWORD dwHalf_N1 = dwInput_N / 2;
    DWORD dwHalf_N2 = dwInput_N - dwHalf_N1;

    for (DWORD dwIdx = 0; dwIdx < (1 << dwHalf_N1); dwIdx++) {
        QWORD qwSum = 0;
        DWORD dwPos = 0;
        for (DWORD dwBit = dwIdx; dwBit != 0; dwBit >>= 1, dwPos++) {
            if (dwBit & 1) {
                qwSum += (QWORD)s_adwSize[dwPos];
            }
        }
        s_aqwSumsU[dwIdx] = qwSum;
    }

    for (DWORD dwIdx = 0; dwIdx < (1 << dwHalf_N2); dwIdx++) {
        SQWORD qwSum = 0;
        DWORD dwPos = 0;
        for (DWORD dwBit = dwIdx; dwBit != 0; dwBit >>= 1, dwPos++) {
            if (dwBit & 1) {
                qwSum += (SQWORD)s_adwSize[dwPos + dwHalf_N1];
            }
        }
        s_aqwSumsD[dwIdx] = qwSum;
    }


    static  vector<QWORD> v(s_aqwSumsU, s_aqwSumsU + (1 << dwHalf_N1));
    sort(v.begin(), v.end());
    for (DWORD dwIdx = 0; dwIdx < (1 << dwHalf_N2); dwIdx++) {
        if (s_aqwSumsD[dwIdx] <= (QWORD) dwInput_X) {
            dwSum += lookup(v, (QWORD)dwInput_X - s_aqwSumsD[dwIdx]); 
        }
    }
    return dwSum;
}


int main()
{
    DWORD dwInput_N;
    DWORD dwInput_X;
    DWORD dwAnswer;

    dwInput_N = inputSDWORD();
    dwInput_X = inputSDWORD();


    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        s_adwSize[dwIdx] = inputSDWORD();
     }

    printf("%d\n", solve(dwInput_X, dwInput_N));
    return 0;
}
