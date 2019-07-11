#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void printMatrix(DWORD **p, DWORD dwSize)
{
    printf("---\n");
    for (DWORD i = 0; i < dwSize; i++) {
        for(DWORD j = 0; j < dwSize; j++) {
            printf("%5d", p[i*dwSize + j]);
        }
        cout << endl;
    }
    printf("---\n");
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

#define MAX_STR_LEN (100)

int main()
{
    DWORD dwInput_N, dwInput_M, dwInput_Q;
 
    static char str[MAX_STR_LEN + 1];

    /* string input */
    fgets(str, ArrayLength(str), stdin);

    dwInput_N = inputSDWORD();
    DWORD dwIdx = 0;
    while(1) {
        if (dwInput_N - 1 <= dwIdx) {
            break;
        }
        if (str[dwIdx] != '1') {
            break;
        }
        dwIdx++;
    }
    printf("%c\n", str[dwIdx]);

    return 0;
}
