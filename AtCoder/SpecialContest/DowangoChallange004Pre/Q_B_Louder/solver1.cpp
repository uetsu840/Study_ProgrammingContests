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


bool bDebug = false;

/**
 *  BIT
 *      最大値の計算と、値の代入。(加算ではない)
 * 
 *      引数のインデックスは 0 ～ N-1、
 *      内部のインデックスは 1 ～ N であることに注意する。
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

    SQWORD Max(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        SQWORD sqMax = 0;
        while (0 < lCur) {
            sqMax = max(sqMax, vecsqBitN[lCur]);
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return sqMax;
    }

    void Update(SDWORD lIdx, SQWORD sqX)
    {
        while (lIdx <= lBitN) {
            vecsqBitN[lIdx] = max(vecsqBitN[lIdx], sqX);
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD End() {
        return lBitN;
    }
};

#define MAX_VAL_X   (200000)

/* 空間圧縮用 */
struct ST_INPUT_V {
    SQWORD sqIdx;
    SQWORD sqV;

    ST_INPUT_V (SDWORD idx, SQWORD v) : sqIdx(idx), sqV(v) {};
};

bool cmpVal(const ST_INPUT_V &a, const ST_INPUT_V &b) {
    if (a.sqV == b.sqV) {
        return (a.sqIdx > b.sqIdx);
    }
    return a.sqV < b.sqV;
}

bool cmpIdx(const ST_INPUT_V &a, const ST_INPUT_V &b) {
    return a.sqIdx < b.sqIdx;
}

int main()
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    BinaryIndexedTree bit(sqN);

    vector<ST_INPUT_V> vecV;

    for (SDWORD lIdx = 1; lIdx <= sqN; lIdx++) {
        SQWORD sqV = inputSQWORD();
        vecV.emplace_back(lIdx, sqV);
    }

    /* 空間圧縮。bit 用に1はじまりにする */
    sort (vecV.begin(), vecV.end(), cmpVal);
    SQWORD sqCmp = 1; 
    for (auto it = vecV.begin(); it!= vecV.end(); ++it) {
        it->sqV = sqCmp;
        sqCmp++;
    }

    for (SDWORD lIdx = 0; lIdx <= sqK; lIdx++) {
        for (auto v: vecV) {
            SQWORD sqVal = bit.Max(v.sqIdx - 1);
            bit.Update(v.sqIdx, sqVal + 1);            
        }
#if 0
        for (SDWORD lIdx = 1; lIdx <= sqN; lIdx++) {  
            printf("%lld ", bit.Max(lIdx));
        }
        printf("\n");
#endif
    }

    SQWORD sqAns = 0;
    for (SDWORD lIdx = 1; lIdx <= sqN; lIdx++) {  
        sqAns = max(sqAns, bit.Max(lIdx));
    }
    printf("%lld\n", sqAns); 

    return 0;
}