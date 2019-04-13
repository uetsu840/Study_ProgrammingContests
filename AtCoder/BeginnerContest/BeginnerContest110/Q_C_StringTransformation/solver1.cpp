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

static DWORD getIdx(char c)
{
    return (DWORD)(c - 'a');
}

static inline M_BOOL registTbl(char *pcTbl, char ca, char cb)
{
    DWORD dwIdx = getIdx(ca);
    if (0 == pcTbl[dwIdx]) {
        pcTbl[dwIdx] = cb;
    } else {
        if (cb != pcTbl[dwIdx]) {
            return M_FALSE;
        }
    }
    return M_TRUE;
}

#define MAX_STRING (200000 + 1)
#define MAX_ALPHABETS (26)
int main()
{
    char acStr1[MAX_STRING];
    char acStr2[MAX_STRING];
    char acTransFormationTable1[MAX_ALPHABETS];
    char acTransFormationTable2[MAX_ALPHABETS];
    M_BOOL bMatched = M_FALSE;
    char *pcCur1 = acStr1;
    char *pcCur2 = acStr2;

    inputString(acStr1);
    inputString(acStr2);

    memset(acTransFormationTable1, 0, sizeof(acTransFormationTable1));
    memset(acTransFormationTable2, 0, sizeof(acTransFormationTable2));

    for (;;) {
        char c1, c2;
        c1 = *pcCur1;
        c2 = *pcCur2;

        if ((*pcCur1 == '\0') || (*pcCur2 == '\0')) {
            if (*pcCur2 == *pcCur1) {
                bMatched = M_TRUE;                
            } else {
                bMatched = M_FALSE;
            }
            break;
        }

        if (!registTbl(acTransFormationTable1, c1, c2)) {
            break;
        };
        if (!registTbl(acTransFormationTable2, c2, c1)) {
            break;
        }
        pcCur1++;
        pcCur2++;
    }

    if (bMatched) {
        printf("Yes");
    } else {
        printf("No");
    }

    return 0;
}
