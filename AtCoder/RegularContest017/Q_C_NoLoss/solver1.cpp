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

#define MAX_OBJECT_NUM    (10000000)
static SDWORD s_adwSize[MAX_OBJECT_NUM];

DWORD getAnswer(DWORD dwIdx, DWORD dwRest, DWORD dwObjNum) {
//    printf("%d %d %d\n", dwIdx, dwRest, dwObjNum);
    if (dwIdx == dwObjNum - 1) {
        if ((dwRest == s_adwSize[dwIdx] ) || (0 == dwRest)) {
//            printf(">\n");
            return 1;
        } else {
//            printf("x\n");
            return 0;
        }
    } else {
        if (s_adwSize[dwIdx] <= dwRest) {
            DWORD dwAnswer;
//            printf("=====\n");
            dwAnswer = getAnswer(dwIdx + 1, dwRest - s_adwSize[dwIdx], dwObjNum)
                        + getAnswer(dwIdx+1, dwRest, dwObjNum);
            return dwAnswer;
        } else {
//            printf("-----\n");
            return getAnswer(dwIdx+1, dwRest, dwObjNum);
        }
    }
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
  //      printf("-- %d\n", s_adwSize[dwIdx]);
    }

    printf("%d\n", getAnswer(0, dwInput_X, dwInput_N));
    return 0;
}
