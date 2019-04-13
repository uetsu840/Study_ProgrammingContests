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

#define INPUT_STR_BUF_SIZE      (102)
static char s_acString[INPUT_STR_BUF_SIZE];
static const char s_acRefString[INPUT_STR_BUF_SIZE] = "keyence";

int main()
{
    inputString(s_acString);

    DWORD dwOrgStrLen = strlen(s_acString);
    DWORD dwRefStrLen = strlen(s_acRefString);

    if (dwOrgStrLen < dwRefStrLen) {
        printf("NO\n");
        return 0;
    } 
    if (dwOrgStrLen == dwRefStrLen) {
        if (0 == strcmp(s_acString, s_acRefString)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
        return 0;
    }

    DWORD dwCutStrLen = dwOrgStrLen - dwRefStrLen;
    for (DWORD dwLen1 = 0; dwLen1 < dwOrgStrLen - dwCutStrLen; dwLen1++) {
        char acCuttedString[INPUT_STR_BUF_SIZE] = "";
        char *pcCurTop = s_acString;

        DWORD dwLen2 = strlen(s_acRefString) - dwLen1;
        strncat(acCuttedString, s_acString, dwLen1);
        strncat(acCuttedString, &s_acString[dwLen1 + dwCutStrLen], dwLen2);
        if (0 == strcmp(acCuttedString, s_acRefString)) {
            printf("YES\n");
            return 0;
        }
    }
    printf("NO\n");
    return 0;
}
