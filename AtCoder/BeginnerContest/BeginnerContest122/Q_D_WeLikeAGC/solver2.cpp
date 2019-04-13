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

static SQWORD calcZeroToNumXor(SQWORD sqNum)
{
    SQWORD sqRet = 0;
    for (DWORD dwBit = 1; dwBit < 64; dwBit++) {
        SQWORD sqMask = 0x1LL << dwBit;
        if (sqMask & sqNum) {
            if (0 == (sqNum & 0x1)) {
                sqRet |= sqMask;
            }
        }
    }

    /* lowest bit */
    if ((sqNum % 4 == 1) || (sqNum % 4 == 2)) {
        sqRet |= 1;
    }

    return sqRet;
}

static BYTE getCharFromKey(BYTE byKey)
{
    BYTE byChar = '0';
    switch (byKey) {
    case 1:
        byChar = 'A';
        break;
    case 2:
        byChar = 'G';
        break;
    case 3:
        byChar = 'C';
        break;
    case 4:
        byChar = 'T';
        break;
    default:
        break;
    }
    return byChar;
}

static BYTE getKeyFromChar(BYTE byChar)
{
    BYTE byKey = 0;
    switch (byChar) {
    case 'A':
        byKey = 1;
        break;
    case 'G':
        byKey = 2;
        break;
    case 'C':
        byKey = 3;
        break;
    case 'T':
        byKey = 4;
        break;
    default:
        break;
    }
    return byKey;
}

/**
 *  LAST3      [0x00, b[-3], b[-2], b[-1]]
 */
static bool isSequenceOK(DWORD dwLast3, BYTE byNext)
{
    BYTE byKey_3 = (dwLast3 & 0x00000F00) >> 8;
    BYTE byKey_2 = (dwLast3 & 0x000000F0) >> 4;
    BYTE byKey_1 = (dwLast3 & 0x0000000F);

    BYTE byC_3 = getCharFromKey(byKey_3);
    BYTE byC_2 = getCharFromKey(byKey_2);
    BYTE byC_1 = getCharFromKey(byKey_1);

    /* ?AGC */
    if (('A' == byC_2) && ('G' == byC_1) && ('C' == byNext)) {
        return false;
    }

    /* ?ACG */
    if (('A' == byC_2) && ('C' == byC_1) && ('G' == byNext)) {
        return false;
    }

    /* ?GAC */
    if (('G' == byC_2) && ('A' == byC_1) && ('C' == byNext)) {
        return false;
    }

    /* A?GC */
    if (('A' == byC_3) && ('G' == byC_1) && ('C' == byNext)) {
        return false;
    }

    /* AG?C */
    if (('A' == byC_3) && ('G' == byC_2) && ('C' == byNext)) {
        return false;
    }

    return true;
}

static SQWORD s_aasqDpTbl[102][0xFFF];

#define ANS_MOD (1000000007)

/**
*   @brief 次のカウントを得る。
*/ 
static DWORD getNextDp(DWORD dwNextLast3, SQWORD *psqPrevDpTbl, SQWORD *psqNextDpTbl)
{
    DWORD dwPrevLast3;
    BYTE byNext = getCharFromKey(dwNextLast3 & 0x0000000F);
    SQWORD sqNextDp = 0;
    for (DWORD dwKey = 0; dwKey <= 4; dwKey++) {
        dwPrevLast3 = (((dwNextLast3 & 0x00000FF0) >> 4)
                        | (dwKey << 8));
        if (isSequenceOK(dwPrevLast3, byNext)) {
            sqNextDp = (sqNextDp + *(psqPrevDpTbl + dwPrevLast3)) % ANS_MOD;
        }
    }

    *(psqNextDpTbl + dwNextLast3) = sqNextDp;
}


int main()
{
    SQWORD sqInput_N = inputSQWORD();

    /**
     *  dp[i][last3] 
     *      最後の3文字が last3 で、AGCが現れない文字列の数
     *  dp[0][*] = 0
     *  dp[1][*] = 4
     *  dp[2][*] = 16
     *  dp[3][*] = 61
     * 
     *  dp[j+1][*] = dp[j][]
     */

    memset(s_aasqDpTbl, 0, sizeof(s_aasqDpTbl));

    /* dp[1] を手動で初期化する */
    for (DWORD dwKey_1 = 1; dwKey_1 <= 4; dwKey_1++) {
        DWORD dwKey = dwKey_1;
        s_aasqDpTbl[1][dwKey] = 1;
    }

    for (DWORD dwIdx = 1; dwIdx <= sqInput_N; dwIdx++) {
        for (DWORD dwKey_3 = 0; dwKey_3 <= 4; dwKey_3++) {
            for (DWORD dwKey_2 = 0; dwKey_2 <= 4; dwKey_2++) {
                /* 追加する文字は空文字が存在しない */
                for (DWORD dwKey_1 = 1; dwKey_1 <= 4; dwKey_1++) {
                    DWORD dwNextKey = (dwKey_3 << 8) | (dwKey_2 << 4) | (dwKey_1);
                    getNextDp(dwNextKey, s_aasqDpTbl[dwIdx], s_aasqDpTbl[dwIdx+1]);
                }
            }
        }
    }

    SQWORD sqAns = 0;
    for (DWORD dwKey_3 = 1; dwKey_3 <= 4; dwKey_3++) {
        for (DWORD dwKey_2 = 1; dwKey_2 <= 4; dwKey_2++) {
            for (DWORD dwKey_1 = 1; dwKey_1 <= 4; dwKey_1++) {
                DWORD dwAllKey = (dwKey_3 << 8) | (dwKey_2 << 4) | (dwKey_1);
                sqAns = (sqAns + s_aasqDpTbl[sqInput_N][dwAllKey]) % ANS_MOD;
            }
        }
    }


    printf("%lld\n", sqAns);


    return 0;
}