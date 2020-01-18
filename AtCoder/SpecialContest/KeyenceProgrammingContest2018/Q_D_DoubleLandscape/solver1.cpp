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

using MOD_LL = long long;

const MOD_LL MOD = 1000000007LL;
 
MOD_LL ADD_MOD(MOD_LL x, MOD_LL y) { return (x + y) % MOD; }
MOD_LL SUB_MOD(MOD_LL x, MOD_LL y) { return (x - y + MOD) % MOD; }
MOD_LL MUL_MOD(MOD_LL x, MOD_LL y) { return x * y % MOD; }
MOD_LL POW_MOD(MOD_LL x, MOD_LL e) {
  MOD_LL v = 1;
  for (; e; x = MUL_MOD(x, x), e >>= 1)
    if (e & 1) v = MUL_MOD(v, x);
  return v;
}
MOD_LL DIV_MOD(MOD_LL x, MOD_LL y) { /*assert(y%MOD!=0);*/
  return MUL_MOD(x, POW_MOD(y, MOD - 2));
}
MOD_LL COMB(MOD_LL n, MOD_LL k) { 
    MOD_LL v=1; 
    for(MOD_LL i=1; i<=k; i++) {
        v = DIV_MOD(MUL_MOD(v, n-i+1),i);
    }  
    return v; 
}


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


#define MAX_ARY_NUM (1000)
static DWORD s_adwMaxVal_Row_A[MAX_ARY_NUM];
static DWORD s_adwMaxVal_Col_B[MAX_ARY_NUM];

static bool checkDuplicate(const vector<DWORD> &vVals)
{
    DWORD dwPrevVal = 0;
    for (DWORD dwIdx = 0; dwIdx < vVals.size(); dwIdx++) {
        if (vVals[dwIdx] == dwPrevVal) {
            return false;
        }
        dwPrevVal = vVals[dwIdx];
    }
    return true;
}

static bool searchValue(vector<DWORD> &vals, DWORD dwValue)
{
    if (binary_search(vals.begin(), vals.end(), dwValue)) {
        return true;
    } 
    return false;
}

static DWORD getUpperBoundIndex(vector<DWORD> vVec, DWORD dwValue) 
{
    auto col = upper_bound(vVec.begin(), vVec.end(), dwValue);
    return (col - vVec.begin());
}

int main()
{
    DWORD dwInput_N, dwInput_M;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        s_adwMaxVal_Row_A[dwIdx] = inputSDWORD();
    }
    for (DWORD dwIdx = 0; dwIdx < dwInput_M; dwIdx++) {
        s_adwMaxVal_Col_B[dwIdx] = inputSDWORD();
    }

    vector<DWORD> vRowMaxVals(s_adwMaxVal_Row_A, s_adwMaxVal_Row_A + dwInput_N);
    vector<DWORD> vColMaxVals(s_adwMaxVal_Col_B, s_adwMaxVal_Col_B + dwInput_M);

    sort(vRowMaxVals.begin(), vRowMaxVals.end());
    sort(vColMaxVals.begin(), vColMaxVals.end());

    /* 重複した値がある場合は0 */
    if (!(checkDuplicate(vRowMaxVals)) || (!checkDuplicate(vColMaxVals))) {
        printf("0\n");
        return 0;
    }

    /* 上から順に値を決めてゆく */
    SQWORD sqAnswer = 1;
    for (DWORD dwValue = dwInput_M * dwInput_N; 0 < dwValue; dwValue--) {
        bool bIsExistRow = searchValue(vRowMaxVals, dwValue);
        bool bIsExistCol = searchValue(vColMaxVals, dwValue);

        DWORD dwCases;
        if (bIsExistRow && bIsExistCol) {
            dwCases = 1;
//            printf("0:%d\n", dwCases);
        } else if (bIsExistRow) {
            DWORD dwIdx = getUpperBoundIndex(vColMaxVals, dwValue);
            if (dwInput_M <= dwIdx) {
                printf("0\n");
                return 0;
            } 
            dwCases = dwInput_M - dwIdx;
//            printf("1:%d\n", dwCases);
        } else if (bIsExistCol) {
            DWORD dwIdx = getUpperBoundIndex(vRowMaxVals, dwValue);
            if (dwInput_N <= dwIdx) {
                printf("0\n");
                return 0;
            } 
            dwCases = dwInput_N - dwIdx;
//            printf("2:%d\n", dwCases);
        } else {
            DWORD dwColIdx = getUpperBoundIndex(vColMaxVals, dwValue);
            DWORD dwRowIdx = getUpperBoundIndex(vRowMaxVals, dwValue);
            DWORD dwCasesCand = (dwInput_M - dwColIdx) * (dwInput_N - dwRowIdx);
            dwCases = dwCasesCand - (dwInput_M * dwInput_N - dwValue);
//            printf("3:%d %d %d %d\n", dwRowIdx, dwColIdx, dwCasesCand, dwCases);
        }

        sqAnswer = MUL_MOD(sqAnswer, (SQWORD)dwCases);
    }
    printf("%d\n", sqAnswer);

    return 0;
}
