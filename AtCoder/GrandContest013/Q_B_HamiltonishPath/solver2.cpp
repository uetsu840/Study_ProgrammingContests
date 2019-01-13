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

#define MAX_POINTS  (200000)
static bool s_abIsIncluded[MAX_POINTS + 1];

static bool addEdgeOne(
    vector<pair<DWORD, DWORD>> &avEdgeOne, 
    vector<DWORD> &vAns,
    DWORD *pdwEnd,
    bool *pbAdded,
    bool bIsTail)
{
    for (vector<pair<DWORD, DWORD>>::iterator it = avEdgeOne.begin(); 
            it != avEdgeOne.end(); 
            ++it) {
        if (!s_abIsIncluded[it->second]) {
            if (bIsTail) {
                vAns.push_back(it->second);
            } else {
                vAns.insert(vAns.begin(), it->second);
            }
            *pdwEnd = it->second;
            s_abIsIncluded[it->second] = true;
            *pbAdded = true;
            break;
        }
    }
}

int main()
{
    DWORD dwInput_N;
    DWORD dwInput_M;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    static vector<pair<DWORD, DWORD>> avEdges[MAX_POINTS + 1];
    static vector<DWORD> vAnswer;

    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_abIsIncluded); dwIdx++) {
        s_abIsIncluded[dwIdx] = false;
    }

    bool bIsFirst = true;
    DWORD dwEndA, dwEndB;

    /* input */
    for (DWORD dwIdx = 0; dwIdx < dwInput_M; dwIdx++) {
        DWORD dwInput_A = inputSDWORD();
        DWORD dwInput_B = inputSDWORD();

        pair<DWORD, DWORD> EdgeA(dwInput_A, dwInput_B);
        pair<DWORD, DWORD> EdgeB(dwInput_B, dwInput_A);

        if (bIsFirst) {
            vAnswer.push_back(EdgeA.first);
            vAnswer.push_back(EdgeA.second);
            dwEndA = dwInput_A;
            dwEndB = dwInput_B;
            s_abIsIncluded[dwEndA] = true;
            s_abIsIncluded[dwEndB] = true;
            bIsFirst = false;
        }

        avEdges[dwInput_A].push_back(EdgeA);
        avEdges[dwInput_B].push_back(EdgeB);
    }

    /* search hamiltonish path */
    while(1) {
        bool bAdded = false;
        addEdgeOne(avEdges[dwEndA], vAnswer, &dwEndA, &bAdded, false);
        addEdgeOne(avEdges[dwEndB], vAnswer, &dwEndB, &bAdded, true);

        if (!bAdded) {
            break;
        }
    }
    
    printf("%d\n", vAnswer.size());
    for (vector<DWORD>::iterator it = vAnswer.begin(); it != vAnswer.end(); ++it) {
        printf("%d", *it);
        if (it != vAnswer.end() - 1) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
