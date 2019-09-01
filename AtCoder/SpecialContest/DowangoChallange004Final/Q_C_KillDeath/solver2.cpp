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


/**
*   dp(i)[j]     i番目までの人(とその後ろの同一グループ)を見た時の、
*                合計がjの組み合わせの数
*                k は自分を含め、同じグループに属する人数
*
*   dp[i][j] = dp[i-1][j] + dp[i][j-k]
*
*/
#define MAX_PLAYER      (100)
#define MAX_KD_NUM      (1000)
#define MAX_KD_SUM      (1000)

#define ANS_MOD      (1000000007)
static SQWORD getPatternNum(
    vector<SQWORD> vsqKill,
    SQWORD sqSumDeath)
{
    static SQWORD aasqDpTbl[MAX_PLAYER+1][MAX_KD_NUM + 1];
    memset(aasqDpTbl, 0, sizeof(aasqDpTbl));

    /* initialize dp tbl */
    aasqDpTbl[0][0] = 1;

    /* exec dp */
    for (SDWORD lPlayerIdx = 1; lPlayerIdx <= vsqKill.size(); lPlayerIdx++) {
        SQWORD sqRestPlayer = lower_bound(vsqKill.begin(), vsqKill.end(), vsqKill[lPlayerIdx - 1] + 1)
                             - (vsqKill.begin() + lPlayerIdx) + 1;
//        printf("rest %lld\n", sqRestPlayer);

        for (SDWORD lSum = 0; lSum <= sqSumDeath; lSum++) {
            SQWORD sqDpMem1, sqDpMem2;
            if (1 <= lPlayerIdx) {
                sqDpMem1 =  aasqDpTbl[lPlayerIdx - 1][lSum];
            } else {
                sqDpMem1 = 0;
            }

            if (sqRestPlayer <= lSum) {
                sqDpMem2 = aasqDpTbl[lPlayerIdx][lSum - sqRestPlayer];
            } else {
                sqDpMem2 = 0;
            }

            aasqDpTbl[lPlayerIdx][lSum] = (sqDpMem1 + sqDpMem2) % ANS_MOD;
//            printf("%lld %lld %lld |", sqDpMem1, sqDpMem2, aasqDpTbl[lPlayerIdx][lSum]);
        }
//        printf("\n");
    }

//    printf("%d\n", aasqDpTbl[vsqKill.size()][sqSumDeath]);
    return aasqDpTbl[vsqKill.size()][sqSumDeath];
}


int main()
{
    SQWORD  sqInput_N = inputSQWORD();
    SQWORD  sqInput_M = inputSQWORD();

    vector<SQWORD> vsqKillA;
    vector<SQWORD> vsqKillB;

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        vsqKillA.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqInput_M; sqIdx++) {
        vsqKillB.emplace_back(inputSQWORD());
    }
    reverse(vsqKillA.begin(), vsqKillA.end());
    reverse(vsqKillB.begin(), vsqKillB.end());
    SQWORD sumDeathA = accumulate(vsqKillB.begin(), vsqKillB.end(), 0LL);
    SQWORD sumDeathB = accumulate(vsqKillA.begin(), vsqKillA.end(), 0LL);
    
    SQWORD sqPatternA = getPatternNum(vsqKillA, sumDeathA);
    SQWORD sqPatternB = getPatternNum(vsqKillB, sumDeathB);

    SQWORD sqAns = (sqPatternA * sqPatternB) % ANS_MOD;

    printf("%lld\n", sqAns);

    return 0;
}