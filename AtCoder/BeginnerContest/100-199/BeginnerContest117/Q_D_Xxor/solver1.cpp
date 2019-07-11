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

static bool greaterPair(pair<SQWORD, SQWORD> &p1, pair<SQWORD, SQWORD> &p2) 
{
    return (p1.first > p2.first);
}


#define BIT_NUM (48)    /* 10^12 */
#define MAX_N   (100000)

int main()
{
    SQWORD sqInput_N, sqInput_K;
    static SDWORD  alBitCnts[BIT_NUM];
    SQWORD  asqInput_A[MAX_N];
    SQWORD  sqInputSum = 0;

    sqInput_N = inputSQWORD();
    sqInput_K = inputSQWORD();

    /**
     * input A 
     *   - calc sum
     *   - count bit where bit == 1
     */
    for (DWORD dwIdx = 0; dwIdx < sqInput_N; dwIdx++) {
        SQWORD sqInput_A = inputSQWORD();
        asqInput_A[dwIdx] = sqInput_A;
        sqInputSum += sqInput_A;

        for (DWORD dwBit = 0; dwBit < BIT_NUM; dwBit++) {
            if (sqInput_A & 0x1) {
                alBitCnts[dwBit]++;
            }
            sqInput_A >>= 1;
        }
    }

    vector<SQWORD> vWeightCnt;
    for (DWORD dwIdx = 0; dwIdx < BIT_NUM; dwIdx++) {
        SDWORD lBitCnt = alBitCnts[dwIdx];
        SDWORD lXorDiffBitNum;
        if (2LL * (SQWORD)lBitCnt < sqInput_N) {
            lXorDiffBitNum = sqInput_N - 2LL * (SQWORD)lBitCnt;
        } else {
            lXorDiffBitNum = 0;
        }

        SQWORD sqWeight = (SQWORD)lXorDiffBitNum << dwIdx;
        vWeightCnt.emplace_back(sqWeight); 
    }

    SQWORD asqCumSum[BIT_NUM];
    SQWORD sqCumSum = 0;
    for (DWORD dwIdx = 0; dwIdx < BIT_NUM; dwIdx++) {
        asqCumSum[dwIdx] = sqCumSum;
        sqCumSum += vWeightCnt[dwIdx];
    }

    /* search MSB */
    DWORD dwMSB = 0;
    for (DWORD dwIdx = 0; dwIdx < BIT_NUM; dwIdx++) {
        if (sqInput_K & (0x1LL << dwIdx)) {
            dwMSB = dwIdx;
        }
    }

    SQWORD sqMaxAddVal = 0;
    for (SDWORD lBit = dwMSB; 0<=lBit; lBit--) {
        if (sqInput_K & (0x1LL << lBit)) {

            /* 自分より下位の0のビットの合計値を数える */
            SQWORD sqZeroBitSum = 0;
            for (SDWORD lCntZeroBit = lBit-1; 0<= lCntZeroBit; lCntZeroBit--) {
                if (!(sqInput_K & (0x1LL << lCntZeroBit))) {
                    sqZeroBitSum += vWeightCnt[lCntZeroBit];
                } 
            }
            if (vWeightCnt[lBit] <= sqZeroBitSum) {
                sqMaxAddVal += asqCumSum[lBit];
                break;
            } else {
                sqMaxAddVal += vWeightCnt[lBit];
            }
        }
    }
    SQWORD sqAns = sqInputSum + sqMaxAddVal;

    printf("%lld\n", sqAns);

    return 0;
}