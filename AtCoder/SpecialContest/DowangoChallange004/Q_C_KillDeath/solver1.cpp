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
*   dp(i)[j][k]     i番目までの人を見た時の、
*                   合計がjで、かつ右端がkになるような組み合わせの数
*
*   (a[i] = a[i-1]) のとき
*   dp(i+1)[j][k] =   dp(i)[j-k][0]
*                   + dp(i)[j-k][1]
*                           :
*                   + dp(i)[j-k][k-1]
*                   + dp(i)[j-k][k]
*                 = dp(i+1)[j-1][k-1]
*                   + dp(i)[j-k][k]
*
*   (a[i] ≠ a[i-1]) のとき
*   dp(i+1)[j][k] =   dp(i)[j-k][0]
*                   + dp(i)[j-k][1]
*                       :
*                   + dp(i)[j-k][n]
*                 = dp(i+1)[j-1][k-1] 
*/
#define MAX_KD_NUM   (1000)
#define MAX_KD_SUM   (1000)

#define ANS_MOD      (1000000007)
static SQWORD getPatternNum(
    vector<SQWORD> vsqKill,
    SQWORD sqSumDeath)
{
    static SQWORD aasqDpTbl[MAX_KD_SUM+1][MAX_KD_NUM + 1];
    SQWORD sqMemCount = vsqKill.size();

//    printf("entry : %lld\n", sqSumDeath);

    /* Initialize Dp Tbl */
    memset(aasqDpTbl, 0, sizeof(aasqDpTbl));
    for (SQWORD sqFirstDeathCnt = 0; sqFirstDeathCnt <= sqSumDeath; sqFirstDeathCnt++) {
        aasqDpTbl[sqFirstDeathCnt][sqFirstDeathCnt] = 1;
    }

    /* exec dp */
    for (SQWORD sqMemIdx = 1; sqMemIdx < sqMemCount; sqMemIdx++) {
//        printf("------------\n");
        static SQWORD aasqNextDpTbl[MAX_KD_SUM+1][MAX_KD_NUM + 1];
        if (vsqKill[sqMemIdx] == vsqKill[sqMemIdx - 1]) {
            /* a[i] = a[i-1] */
            for (SQWORD sqSum = 0; sqSum <= sqSumDeath; sqSum++) {
                for (SQWORD sqLatest = 0; sqLatest <= sqSumDeath; sqLatest++) {
                    SQWORD sqNextDpTblPrev;
                    SQWORD sqDpTblAdd;
                    if ((1 <= sqSum) && (1 <= sqLatest)) {
                        sqNextDpTblPrev = aasqNextDpTbl[sqSum - 1][sqLatest - 1];
                    } else {
                        sqNextDpTblPrev = 0;
                    }
                    if (sqLatest <= sqSum) {
                        sqDpTblAdd = aasqDpTbl[sqSum - sqLatest][sqLatest];
                    } else {
                        sqDpTblAdd = 0;                       
                    }

                    aasqNextDpTbl[sqSum][sqLatest] = (sqNextDpTblPrev + sqDpTblAdd) % ANS_MOD;
//                    printf("eq: %lld %lld: %lld\n", sqSum, sqLatest, aasqNextDpTbl[sqSum][sqLatest]);
                }
            }
        } else {
            /* a[i] != a[i-1] */
            for (SQWORD sqSum = 0; sqSum <= sqSumDeath; sqSum++) {
                for (SQWORD sqLatest = 0; sqLatest <= sqSumDeath; sqLatest++) {
                    SQWORD sqNextDpVal;
                    if ((1 <= sqSum) && (1 <= sqLatest)) {
                        sqNextDpVal = aasqNextDpTbl[sqSum - 1][sqLatest - 1];
                    } else {
                        sqNextDpVal = 0;
                        if (sqLatest <= sqSum) {
                            for (SQWORD sqScore = 0; sqScore <= sqSumDeath; sqScore++) {
                                sqNextDpVal = (aasqDpTbl[sqSum - sqLatest][sqScore] + sqNextDpVal) % ANS_MOD;
                            }
                        }
                    }
//                    printf("ne: %lld %lld: %lld\n", sqSum, sqLatest, aasqNextDpTbl[sqSum][sqLatest]);
                    aasqNextDpTbl[sqSum][sqLatest] = sqNextDpVal;
                }
            }
        }
        memcpy(aasqDpTbl, aasqNextDpTbl, sizeof(aasqDpTbl));
    }

    SQWORD sqPattern = 0;
    for (SQWORD sqLatest = 0; sqLatest <= sqSumDeath; sqLatest++) {
        sqPattern = (sqPattern + aasqDpTbl[sqSumDeath][sqLatest]) % ANS_MOD;
    }
//    printf("--> pat: %lld\n", sqPattern);
    return sqPattern;
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
    SQWORD sumDeathA = accumulate(vsqKillB.begin(), vsqKillB.end(), 0LL);
    SQWORD sumDeathB = accumulate(vsqKillA.begin(), vsqKillA.end(), 0LL);
    
    SQWORD sqPatternA = getPatternNum(vsqKillA, sumDeathA);
    SQWORD sqPatternB = getPatternNum(vsqKillB, sumDeathB);

    SQWORD sqAns = (sqPatternA * sqPatternB) % ANS_MOD;

    printf("%lld\n", sqAns);

    return 0;
}