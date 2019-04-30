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

#define SQWORD_INF_N        (-10000000000)

static SQWORD getNextScore(vector<SQWORD> &sqTips, vector<SQWORD>::iterator it)
{
    if (sqTips.end() <= it + 1) {
        return 0;
    }
    
    return *(it + 1);
}

#define MAX_TIPS_A  (1000)
#define MAX_TIPS_B  (1000)

int main()
{
    SQWORD  sqInput_A = inputSQWORD();
    SQWORD  sqInput_B = inputSQWORD();

    vector<SQWORD> vsqTipsA;
    vector<SQWORD> vsqTipsB;

    for (SDWORD lIdx = 0; lIdx < sqInput_A; lIdx++) {
        vsqTipsA.emplace_back(inputSQWORD());
    }
    for (SDWORD lIdx = 0; lIdx < sqInput_B; lIdx++) {
        vsqTipsB.emplace_back(inputSQWORD());
    }
    vsqTipsA.emplace_back(0);
    vsqTipsB.emplace_back(0);
    reverse(vsqTipsA.begin(), vsqTipsA.end());
    reverse(vsqTipsB.begin(), vsqTipsB.end());

    vector<vector<SQWORD>> vvsqDpTbl(MAX_TIPS_A + 1, vector<SQWORD>(MAX_TIPS_B+1, 0));

    SQWORD sqMyTurn;
    if ((sqInput_B + sqInput_A) % 2 == 0) {
        sqMyTurn = 0;
    } else {
        sqMyTurn = 1;
    }

    /* dp[i][j] 山A,山Bがそれぞれi枚、j枚残っているときにそこから自分が取れる最大の点数 */
    for (SQWORD sqIdxA = 0; sqIdxA <= sqInput_A; sqIdxA++) {
        for (SQWORD sqIdxB = 0; sqIdxB <= sqInput_B; sqIdxB++) {
            if ((0 == sqIdxA) && (0 == sqIdxB)) {
                vvsqDpTbl[sqIdxA][sqIdxB] = 0;
            } else {
                SQWORD sqScoreA, sqScoreB;
                if ((sqIdxA + sqIdxB) % 2 == sqMyTurn) {
                    sqScoreA = vsqTipsA[sqIdxA];
                    sqScoreB = vsqTipsB[sqIdxB];
                    if (0 == sqIdxA) {
                        vvsqDpTbl[sqIdxA][sqIdxB] = vvsqDpTbl[sqIdxA][sqIdxB-1] + sqScoreB; 
                    } else if (0 == sqIdxB) {
                        vvsqDpTbl[sqIdxA][sqIdxB] = vvsqDpTbl[sqIdxA-1][sqIdxB] + sqScoreA; 
                    } else {
                        vvsqDpTbl[sqIdxA][sqIdxB] = max(vvsqDpTbl[sqIdxA-1][sqIdxB] + sqScoreA, 
                                                        vvsqDpTbl[sqIdxA][sqIdxB-1] + sqScoreB);
                    }
                } else {
                    if (0 == sqIdxA) {
                        vvsqDpTbl[sqIdxA][sqIdxB] = vvsqDpTbl[sqIdxA][sqIdxB-1]; 
                    } else if (0 == sqIdxB) {
                        vvsqDpTbl[sqIdxA][sqIdxB] = vvsqDpTbl[sqIdxA-1][sqIdxB]; 
                    } else {
                        vvsqDpTbl[sqIdxA][sqIdxB] = min(vvsqDpTbl[sqIdxA-1][sqIdxB], 
                                                        vvsqDpTbl[sqIdxA][sqIdxB-1]);
                    }
                }
            }

//            printf("%lld ", vvsqDpTbl[sqIdxA][sqIdxB]);
        }
//        printf("\n");
    }

    SQWORD sqMaxScore = vvsqDpTbl[sqInput_A][sqInput_B];

    printf("%lld\n", sqMaxScore);


//    printf("%d\n", (SDWORD)sqAns);

    return 0;
}