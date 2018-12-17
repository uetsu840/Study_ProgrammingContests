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

static void printMatrix(DWORD *p, DWORD dwRowNum, DWORD dwColNum)
{
    printf("---\n");
    for (DWORD i = 0; i < dwRowNum; i++) {
        for(DWORD j = 0; j < dwColNum; j++) {
            printf("%5d", p[i*dwColNum + j]);
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

int main()
{
    DWORD dwInput_H, dwInput_W;
 
    dwInput_H = inputSDWORD();
    dwInput_W = inputSDWORD();

    /* input coins */
    DWORD aadwCoinCount[dwInput_H][dwInput_W];
    for (DWORD dwRow = 0; dwRow < dwInput_H; dwRow++) {
        for (DWORD dwCol = 0; dwCol < dwInput_W; dwCol++) {
            aadwCoinCount[dwRow][dwCol] = inputSDWORD();
        }
    }

    DWORD aadwProcedure[dwInput_H * dwInput_W][4];
    DWORD dwProcCnt = 0;
    for (DWORD dwRow = 0; dwRow < dwInput_H; dwRow++) {
        for (DWORD dwCol = 0; dwCol < dwInput_W-1; dwCol++) {
            if (0 != (aadwCoinCount[dwRow][dwCol] % 2)) {
                aadwProcedure[dwProcCnt][0] = dwRow;
                aadwProcedure[dwProcCnt][1] = dwCol;
                aadwProcedure[dwProcCnt][2] = dwRow;
                aadwProcedure[dwProcCnt][3] = dwCol+1;
                dwProcCnt++;
                aadwCoinCount[dwRow][dwCol]--;
                aadwCoinCount[dwRow][dwCol+1]++;
            }
        }
        if (dwRow != dwInput_H - 1) {
            if (0 != (aadwCoinCount[dwRow][dwInput_W-1] % 2)) {
                aadwProcedure[dwProcCnt][0] = dwRow;
                aadwProcedure[dwProcCnt][1] = dwInput_W-1;
                aadwProcedure[dwProcCnt][2] = dwRow+1;
                aadwProcedure[dwProcCnt][3] = dwInput_W-1;
                dwProcCnt++;
                aadwCoinCount[dwRow][dwInput_W-1]--;
                aadwCoinCount[dwRow+1][dwInput_W-1]++;
            }
        }
    }
    printf("%d\n", dwProcCnt);
    for (DWORD dwIdx = 0; dwIdx < dwProcCnt; dwIdx++) {
        printf("%d %d %d %d\n",
            aadwProcedure[dwIdx][0]+1, 
            aadwProcedure[dwIdx][1]+1, 
            aadwProcedure[dwIdx][2]+1, 
            aadwProcedure[dwIdx][3]+1);
    }

//    printMatrix((DWORD*)aadwCoinCount, dwInput_H, dwInput_W);

    return 0;
}
