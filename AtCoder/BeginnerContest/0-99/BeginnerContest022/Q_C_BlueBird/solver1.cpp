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

#define MAX_QWORD (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFF)
#define MAX_WORD  (0xFFFF)
#define MAX_BYTE  (0xFF)


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


#define DIST_INF    (100000000)
#define MAX_POINTS  (300)
static DWORD s_aadwDist[MAX_POINTS + 1][MAX_POINTS + 1];
static DWORD s_aadwDistFromStartPnt[MAX_POINTS + 1];

int main()
{
    DWORD dwInput_N;
    DWORD dwInput_M;
    DWORD dwRoomCnt = 0;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    for (DWORD dwIdx1 = 0; dwIdx1 <= MAX_POINTS; dwIdx1++) {
        for (DWORD dwIdx2 = 0; dwIdx2 <= MAX_POINTS; dwIdx2++) {
            if (dwIdx1 == dwIdx2) {
                s_aadwDist[dwIdx1][dwIdx2] = 0;
            } else {
                s_aadwDist[dwIdx1][dwIdx2] = DIST_INF;
            }
        }
    }
    for (DWORD dwIdx1 = 0; dwIdx1 < ArrayLength(s_aadwDistFromStartPnt); dwIdx1++) {
        s_aadwDistFromStartPnt[dwIdx1] = DIST_INF;
    }

    for (DWORD dwIdx = 0; dwIdx < dwInput_M; dwIdx++) {
        DWORD dwInput_U, dwInput_V, dwInput_L;
        dwInput_U = inputSDWORD();
        dwInput_V = inputSDWORD();
        dwInput_L = inputSDWORD();

        if (1 != dwInput_U) {
            s_aadwDist[dwInput_U][dwInput_V] = dwInput_L;
            s_aadwDist[dwInput_V][dwInput_U] = dwInput_L;
        } else {
            s_aadwDistFromStartPnt[dwInput_V] = dwInput_L;
        }
    }

    for (int i = 1; i <= dwInput_N; i++) {     // 経由する頂点
        for (int j = 1; j <= dwInput_N; j++) {    // 開始頂点
            for (int k = 1; k <= dwInput_N; k++) {  // 終端
                s_aadwDist[j][k] = MIN(s_aadwDist[j][k], s_aadwDist[j][i] + s_aadwDist[i][k]);
            }
        }
    }

    DWORD dwAnswer = DIST_INF;
    for (DWORD dwPnt1 = 2; dwPnt1 < dwInput_N; dwPnt1++) {
        for (DWORD dwPnt2 = dwPnt1+1; dwPnt2 <= dwInput_N; dwPnt2++) {
            dwAnswer = MIN(dwAnswer, s_aadwDistFromStartPnt[dwPnt1]
                                        + s_aadwDistFromStartPnt[dwPnt2]
                                        + s_aadwDist[dwPnt1][dwPnt2]);
        }
    }

    if (DIST_INF <= dwAnswer) {
        printf("-1\n");
    } else {
        printf("%d\n", dwAnswer);
    } 
    return 0;
}
