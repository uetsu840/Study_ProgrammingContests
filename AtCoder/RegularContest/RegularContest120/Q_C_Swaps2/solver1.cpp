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

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}

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
 *  BIT
 *      インデックスは 1 ～ N であることに注意する。
 * 
 */
template<typename T>
class BinaryIndexedTree {
    SDWORD lBitN;
    vector<T> _vecBit; 

public:
    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        _vecBit.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < _vecBit.size(); dwIdx++) {
            _vecBit[dwIdx] = 0;
        }
    }

    T Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        T _sum = 0;
        while (0 < lCur) {
            _sum += _vecBit[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return _sum;
    }

    void Add(SDWORD lIdx, T _x)
    {
        while (lIdx <= lBitN) {
            _vecBit[lIdx] += _x;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(T _sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (_sum < Sum(sqMid));
            } else {
                bJudge = (_sum <= Sum(sqMid));
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
    SDWORD findSumUpperBound(T _sum) 
    {
        return binSearchExec(_sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(T _sum) 
    {
        return binSearchExec(_sum, false);
    }

    SDWORD End() {
        return lBitN;
    }

    void DebugPrint(SDWORD lMax) {
        for (SDWORD lIdx = 0; lIdx <= lMax; lIdx++) {
            printf("%lld ", Sum(lIdx));
        }
        printf("\n");
    }
};


int main()
{
    SQWORD sqN = inputSQWORD();

    vector<SQWORD> vsqA;
    vector<SQWORD> vsqB;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vsqA.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vsqB.emplace_back(inputSQWORD());
    }
    map<SQWORD, vector<SQWORD>> mapPosA, mapPosB;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        mapPosA[vsqA[sqIdx] + sqIdx].emplace_back(sqIdx);
    }
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        mapPosB[vsqB[sqIdx] + sqIdx].emplace_back(sqIdx);
    }

    vector<SQWORD> vsqDstA(sqN);
    for (auto mA: mapPosA) {
        vector<SQWORD> &vPosA = mA.second;

        if (mapPosB[mA.first].size() != vPosA.size()) {
            printf("-1\n");
            return 0;
        }
        for (SQWORD sqIdx = 0; sqIdx < mA.second.size(); sqIdx++) {
            vsqDstA[mA.second[sqIdx]] = mapPosB[mA.first][sqIdx];
        }
    }

    BinaryIndexedTree<SQWORD> bit(sqN);
    /* 転倒数を求める */
    SQWORD sqCorssNum = 0;
    for (DWORD dwJ = 0; dwJ < sqN; dwJ++) {
        sqCorssNum += dwJ - bit.Sum(vsqDstA[dwJ] + 1);
        bit.Add(vsqDstA[dwJ] + 1, 1);
    }
    printf("%lld\n", sqCorssNum);

#if 0
    for (auto p: vsqDstA) {
        printf("%lld ", p);
    }
    printf("\n");
#endif


    return 0;
}
