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
#include <list>
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

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE ABS(DOUBLE a) {return a > 0 ? a : -a; }
static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline bool DoubleIsZero(const DOUBLE &a)
{
    return ABS(a) < DOUBLE_EPS;
}

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

static inline void inputStringSpSeparated(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c) || (' ' == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
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


static inline SQWORD inputSQWORD(void)
{
    SQWORD sqNumber = 0;
    SQWORD sqMultiplier = 1;
    bool bRead = false;
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
            bRead = true;
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
    bool bRead = false;
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
            bRead = true;
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
    bool bRead = false;
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
            bRead = true;
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
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/


/**
 *  BIT
 *      インデックスは 1 ～ N であることに注意する。
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


struct VAL_SEQ {
    SQWORD sqIdx;       /* 最初の順序 */
    SQWORD sqVal;       /* もとの値 */
    SQWORD sqUnique;    /* valの unique な値を昇順に並べたもの */

    VAL_SEQ(SQWORD i, SQWORD v) {
        sqIdx = i;
        sqVal = v;
        sqUnique = 0;
    };
};

bool seqLowerVal(const VAL_SEQ &a, const VAL_SEQ &b)
{
    return a.sqVal < b.sqVal;
}

bool seqLowerIdx(const VAL_SEQ &a, const VAL_SEQ &b)
{
    return a.sqIdx < b.sqIdx;
}

/**
 *      算術平均がK以上
 *      a-K の累積和が0以上
 *      累積和を計算するが、ひとつ足すたびにオフセットの側を動かす。     
 * 
 *      10      20      30      40      30      20      10      offset
 *      -16     -6      +4      +14     +4      -6      -16 
 * 
 *      -16                                                     0
 *      -16     0                                               +6
 *      -16     0       6                                       +2
 *      -16     0       6       2                               -12
 *      -16     0       6       2       -12                     -16
 */


int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_K = inputSQWORD();

    vector<SQWORD>    vecNums;
    vector<VAL_SEQ>   vecstSeq;

    SQWORD sqCurOfs = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqInput_a = inputSQWORD();
        SQWORD sqAOfs = sqInput_a - sqInput_K;

        if (0 == sqIdx) {
            sqCurOfs = 0;
        } else {
            sqCurOfs = sqCurOfs - (sqAOfs);
        }

        vecstSeq.emplace_back(sqIdx * 2    , sqAOfs + sqCurOfs);
        vecstSeq.emplace_back(sqIdx * 2 + 1, sqCurOfs);
    }

    sort(vecstSeq.begin(), vecstSeq.end(), seqLowerVal);

    SQWORD sqUnique = 1;
    for (auto it = vecstSeq.begin(); it != vecstSeq.end(); ++it) {
        if (it == vecstSeq.begin()) {
            it->sqUnique = sqUnique;
        } else {
            if (it->sqVal == (it-1)->sqVal) {
                it->sqUnique = (it-1)->sqUnique;
            } else {
                it->sqUnique = sqUnique;
            }
        }
        sqUnique++;
    }

    sort(vecstSeq.begin(), vecstSeq.end(), seqLowerIdx);

    SQWORD sqAns = 0;
    BinaryIndexedTree UniqueValBit(sqInput_N * 2);
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        UniqueValBit.Add(vecstSeq[sqIdx * 2].sqUnique, 1);
        SQWORD sqCount = UniqueValBit.Sum(vecstSeq[sqIdx * 2 + 1].sqUnique - 1);
        sqAns += (sqIdx + 1 - sqCount);
#if 0
        printf("uni[%lld:%lld][%lld %lld], %lld %lld\n", 
                    vecstSeq[sqIdx * 2].sqUnique,   vecstSeq[sqIdx * 2].sqVal,
                    vecstSeq[sqIdx * 2+1].sqUnique, vecstSeq[sqIdx * 2+1].sqVal,
                    sqIdx, sqCount);
#endif
    }

    printf("%lld\n", sqAns);

    return 0;
}