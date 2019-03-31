#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
using namespace std;

using QWORD  = uint64_t;
using SQWORD = int64_t;
using DWORD  = uint32_t;
using SDWORD = int32_t;
using WORD   = uint16_t;
using SWORD  = int16_t;
using BYTE   = uint8_t;
using SBYTE  = int8_t;
using DOUBLE = double;
using FLOAT  = float;

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


static inline SQWORD inputSQWORD(void)
{
    SQWORD sqNumber = 0;
    SQWORD sqMultiplier = 1;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                sqMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            sqNumber *= 10LL;
            sqNumber += (SQWORD)(c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return sqNumber * sqMultiplier;
            }
        }
    }
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

static inline DOUBLE inputFP(void)
{
    DOUBLE dInt = 0.0;
    DOUBLE dFrac = 0.0;
    DOUBLE dMultiplier = 1.0;
    DWORD dwFpCnt = 0;
    DOUBLE *pdCur = &dInt;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                dMultiplier = -1;
            }
        }
        if ('.' == c) {
            pdCur = &dFrac;
        } else if (('0' <= c) && (c <= '9')) {
            (*pdCur) *= 10;
            (*pdCur) += (DOUBLE)(c - '0');
            bRead = M_TRUE;
            if (pdCur == &dFrac) {
                dwFpCnt++;
            }
        } else {
            if (bRead) {
                return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0 , (DOUBLE)dwFpCnt)));
            }
        }
    }
}


static DWORD cntDiffChar(char *pA, char *pB, DWORD dwLen)
{
    DWORD dwDiffCnt = 0;
    for (DWORD dwIdx = 0; dwIdx < dwLen; dwIdx++) {
        if (*(pA + dwIdx) != *(pB + dwIdx)) {
            dwDiffCnt++;
        }
    }
    return dwDiffCnt;
}


#define NUM_ALPHABETS   (26)
static void cntChar(const char *pcStr, DWORD *pdwCnt)
{
    SDWORD lLen = strlen(pcStr);
    for (SDWORD lIdx = 0; lIdx < NUM_ALPHABETS; lIdx++) {
        *(pdwCnt + lIdx) = 0;
    }
    for (SDWORD lIdx = 0; lIdx < lLen; lIdx++) {
        (*(pdwCnt + (pcStr[lIdx]) - 'a'))++;
    }
}


/**
 *  元の文字列、変更後の文字列から指定した文字を外した時、
 *  上限以下で残りの文字列が構成可能かどうかを調べる。
 */
static bool testChar(
    SDWORD lChangeLimit,
    DWORD   *pdwCharCntOrg,
    DWORD   *pdwCharCntChange,
    DWORD   dwOrgCharIdx,
    DWORD   dwChgCharIdx)
{
    SDWORD alCharCntOrgTmp[NUM_ALPHABETS];
    SDWORD alCharCntRestTmp[NUM_ALPHABETS];
  
    if (0 == pdwCharCntOrg[dwOrgCharIdx]) {
        return false;
    }
    if (0 == pdwCharCntChange[dwChgCharIdx]) {
        return false;
    }

    memcpy(alCharCntOrgTmp, pdwCharCntOrg, sizeof(alCharCntOrgTmp));
    memcpy(alCharCntRestTmp, pdwCharCntChange, sizeof(alCharCntRestTmp));

    alCharCntOrgTmp[dwOrgCharIdx]--;
    alCharCntRestTmp[dwChgCharIdx]--;

    SDWORD lDiffCnt = 0;
    for (SDWORD lIdx = 0; lIdx < NUM_ALPHABETS; lIdx++) {
        lDiffCnt += abs(alCharCntOrgTmp[lIdx] - alCharCntRestTmp[lIdx]);
    }
    if ((lDiffCnt / 2) <= lChangeLimit) {
        return true;
    }
    return false;
}


#define    MAX_STRLEN   (100)
#define    BUFSIZE_STR  (MAX_STRLEN + 1)
int main()
{
    static char acInput[BUFSIZE_STR];
    static char acAns[BUFSIZE_STR];

    SDWORD lInput_N = inputSDWORD();
    SDWORD lInput_K = inputSDWORD();

    inputString(acInput);

    DWORD adwCharCntOrg[NUM_ALPHABETS];
    DWORD adwCharCntRest[NUM_ALPHABETS];
    cntChar(acInput, adwCharCntRest);
    cntChar(acInput, adwCharCntOrg);

    SDWORD  lChangeLimit = lInput_K;
    for (SDWORD lCharCur = 0; lCharCur < lInput_N; lCharCur++) {
        for (SDWORD lChgCharIdx = 0; lChgCharIdx < NUM_ALPHABETS; lChgCharIdx++) {
            SDWORD lOrgCharIdx = acInput[lCharCur] - 'a';
            SDWORD lNewChangeLimit;
            if (lOrgCharIdx != lChgCharIdx) {
                lNewChangeLimit = lChangeLimit - 1;
            } else {
                lNewChangeLimit = lChangeLimit;
            }
            if (testChar(lNewChangeLimit, 
                            adwCharCntOrg, adwCharCntRest, 
                            lOrgCharIdx,
                            lChgCharIdx)) {

                adwCharCntOrg[lOrgCharIdx]--;
                adwCharCntRest[lChgCharIdx]--;
                lChangeLimit = lNewChangeLimit;
                acAns[lCharCur] = (char)('a' + lChgCharIdx);
                break;
            }
        }
    }
    acAns[lInput_N] = '\0';

    printf("%s\n", acAns);
    return 0;
}