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

#define MAX_STUDENTS    (200000)

bool bDebug = false;

/**
 *  BIT
 *      引数のインデックスは 0 ～ N-1、
 *      内部のインデックスは 1 ～ N であることに注意する。
 * 
 */
struct BinaryIndexedTree {
    SDWORD lBitN;
    vector<SQWORD> vecsqBitN; 

    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        vecsqBitN.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < vecsqBitN.size(); dwIdx++) {
            vecsqBitN[dwIdx] = 0;
        }
    }

    SQWORD Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        SQWORD sqSum = 0;
        while (0 < lCur) {
            sqSum += vecsqBitN[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return sqSum;
    }

    void Add(SDWORD lIdx, SQWORD sqX)
    {
        while (lIdx <= lBitN) {
            vecsqBitN[lIdx] += sqX;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(SQWORD sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (sum < Sum(sqMid));
            } else {
                bJudge = (sum <= Sum(sqMid));
            }

            if (bJudge) {
                sqUb = sqMid;
            } else {
                sqLb = sqMid;
            }
        }
        return sqUb;
    }


    /**
    *  累積和が指定した数より大きくなるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumUpperBound(SQWORD sum) 
    {
        return binSearchExec(sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(SQWORD sum) 
    {
        return binSearchExec(sum, false);
    }

    SDWORD End() {
        return lBitN;
    }
};

/* score info */
struct ST_TIME_INFO {
    SQWORD sqTime;
    SQWORD sqIdx;
    SQWORD sqOrder;

    ST_TIME_INFO(SQWORD t, SQWORD i) {
        sqTime = t;
        sqIdx = i;
        sqOrder = 0;
    }
};

bool isScoreLess(const ST_TIME_INFO &a, const ST_TIME_INFO &b)
{
    return a.sqTime < b.sqTime;
}

bool isIdxLess(const ST_TIME_INFO &a, const ST_TIME_INFO &b)
{
    return a.sqIdx < b.sqIdx;
}

static void printTimes(const vector<ST_TIME_INFO> &vecTime)
{
   for (auto time: vecTime) {
        printf("[%lld %lld] ", time.sqTime, time.sqTime);
    }
    printf("\n");
}

int main()
{
    SQWORD sqInput_n = inputSQWORD();
    SQWORD sqInput_M = inputSQWORD();

    BinaryIndexedTree bitCount(sqInput_n);
    BinaryIndexedTree bitSum(sqInput_n);

    vector<ST_TIME_INFO> vecstTimes;
    for (SQWORD sqIdx = 1; sqIdx <= sqInput_n; sqIdx++) {
        SQWORD sqTime = inputSQWORD();
        vecstTimes.emplace_back(sqTime, sqIdx);
    }
    sort(vecstTimes.begin(), vecstTimes.end(), isScoreLess);

    for (SQWORD sqOrderIdx = 0; sqOrderIdx < sqInput_n; sqOrderIdx++) {
        vecstTimes[sqOrderIdx].sqOrder = sqOrderIdx + 1;   
    }
    sort(vecstTimes.begin(), vecstTimes.end(), isIdxLess);

    /* caution : bit has idex of 1..n (not 0..n-1) */
    SQWORD sqStudentCount = 0;

    for (auto time:vecstTimes) {
        SQWORD sqMaxTime = sqInput_M - time.sqTime;

        SQWORD sqBound = bitSum.findSumUpperBound(sqMaxTime);
        SQWORD sqCount = 0;
        if (sqBound != bitSum.End()) {
            sqCount = (sqStudentCount - bitCount.Sum(sqBound - 1));
        }
        printf("%lld\n", sqCount);

        bitSum.Add(time.sqOrder, time.sqTime);
        bitCount.Add(time.sqOrder, 1);

        sqStudentCount++;
    }

    return 0;
}