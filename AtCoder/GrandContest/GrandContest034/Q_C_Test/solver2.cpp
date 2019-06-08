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

#define SQWORD_INF_N    (-10000000000000000)

typedef struct {
    SQWORD sqScoreA;
    SQWORD sqWeightL;
    SQWORD sqWeightU;
} TEST_ONE_SBJ;


typedef struct {
    SQWORD sqGain;
    SQWORD sqCumSumU;
    SQWORD sqCumSumL;
    TEST_ONE_SBJ stSbj;
} TEST_GAIN_ONE;

typedef struct {
    vector<TEST_GAIN_ONE> vecGain; 
    SQWORD sqScoreMax;
} JUDGE_PARAM_ST;


bool operator< (const TEST_GAIN_ONE a, const TEST_GAIN_ONE b) 
{
    if (a.sqGain == b.sqGain) {
        return a.stSbj.sqScoreA < b.stSbj.sqScoreA;
    }
    return a.sqGain > b.sqGain;
}

static SQWORD getPartialSumL(
    vector<TEST_GAIN_ONE> &vecGain,
    SQWORD sqOffset)
{
    SQWORD sqTtlSum = vecGain[vecGain.size() - 1].sqCumSumL;
    if (0 == sqOffset) {
        return sqTtlSum;
    } else {
        return sqTtlSum - vecGain[sqOffset - 1].sqCumSumL;
    }
}

static SQWORD getPartialSumU(
    vector<TEST_GAIN_ONE>  &vecGain,
    SQWORD sqEndIdx)
{
    if (0 == sqEndIdx) {
        return 0;
    } else {
        return vecGain[sqEndIdx - 1].sqCumSumU;
    }
}


/**
*   判定
*       true:   上側に超過
*       false:  下側に超過
*/
static bool isScoreGreater(
    SQWORD sqTryScore, 
    JUDGE_PARAM_ST &stParam)
{
    if (sqTryScore <= 0) {
        return true;
    }

    /**
     *  満点を取らない1課目を振りながら最大得点を出す。
     *  全課目が満点になってしまうパターンでは、
     *  n-1課目の満点と、「満点を取らない1課目」も満点となるように扱い例外処理を減らしている。
     */
    SQWORD sqInput_N = stParam.vecGain.size();
    SQWORD sqScoreRest = sqTryScore;
    SQWORD sqFullScoreNum = (sqTryScore - 1) / stParam.sqScoreMax;
    SQWORD sqRestScoreT   = sqTryScore  - (stParam.sqScoreMax * sqFullScoreNum);
    SQWORD sqTtlPnt = SQWORD_INF_N;

    if (sqInput_N <= sqFullScoreNum) {
        /* 2分探索の範囲から、この条件には入ってくることはない */
        return false;
    }

    for (SQWORD sqRestIdx = 0; sqRestIdx < sqInput_N; sqRestIdx++) {
        SQWORD sqRestWeightU = stParam.vecGain[sqRestIdx].stSbj.sqWeightU;
        SQWORD sqRestWeightL = stParam.vecGain[sqRestIdx].stSbj.sqWeightL;
        SQWORD sqRestScoreA  = stParam.vecGain[sqRestIdx].stSbj.sqScoreA;

        SQWORD sqWeight;
        if (sqRestScoreA < sqRestScoreT) {
            sqWeight = sqRestWeightU;
        } else {
            sqWeight = sqRestWeightL;
        }
        SQWORD sqRestPnt = (sqRestScoreT - sqRestScoreA) * sqWeight;

        SQWORD sqSumU = 0;
        SQWORD sqSumL = 0;
        if (sqRestIdx < sqFullScoreNum) {
            /* 満点を取らない1課目が満点の中にある */
            sqSumU = getPartialSumU(stParam.vecGain, sqFullScoreNum + 1)
                        - (stParam.sqScoreMax - sqRestScoreA) * sqRestWeightU;
            sqSumL = getPartialSumL(stParam.vecGain, sqFullScoreNum + 1);
        } else if (sqRestIdx == sqFullScoreNum) {
            /* 満点を取らない1課目がちょうど中間にある */
            sqSumU = getPartialSumU(stParam.vecGain, sqFullScoreNum);
            sqSumL = getPartialSumL(stParam.vecGain, sqFullScoreNum + 1);
        } else {
            /* 満点を取らない1課目が0点の中にある */
            sqSumU = getPartialSumU(stParam.vecGain, sqFullScoreNum);
            sqSumL = getPartialSumL(stParam.vecGain, sqFullScoreNum)
                        - (0LL - sqRestScoreA) * sqRestWeightL;
        }
        sqTtlPnt = max(sqTtlPnt, sqSumU + sqSumL + sqRestPnt);
    }

    return 0 <= sqTtlPnt;    
}

/**
*   2分探索
*/
static SQWORD binarySearch(
    bool (*judge)(SQWORD, JUDGE_PARAM_ST&), 
    SQWORD sqInitLb, 
    SQWORD sqInitUb,
    JUDGE_PARAM_ST &stParam)
{
    SQWORD sqLb = sqInitLb;
    SQWORD sqUb = sqInitUb;

    while (1LL < sqUb - sqLb) {
        SQWORD sqMid = (sqUb + sqLb) / 2LL;
        if (judge(sqMid, stParam)) {
            sqUb = sqMid;
        } else {
            sqLb = sqMid;
        }
    }
    return sqUb;
}


int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_X = inputSQWORD();
    vector<TEST_ONE_SBJ> vecTests;


    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqInput_b = inputSQWORD();
        SQWORD sqInput_l = inputSQWORD();
        SQWORD sqInput_u = inputSQWORD();

        vecTests.emplace_back(TEST_ONE_SBJ{sqInput_b, sqInput_l, sqInput_u});
    }

    JUDGE_PARAM_ST stParam;
    stParam.sqScoreMax = sqInput_X;

    /* 高橋君からみて満点を取るべき順にテストをソート */
    for (auto sbj: vecTests) {
        SQWORD sqGet = (stParam.sqScoreMax - sbj.sqScoreA) * sbj.sqWeightU;
        SQWORD sqLose = (0LL - sbj.sqScoreA) * sbj.sqWeightL;

        stParam.vecGain.emplace_back(TEST_GAIN_ONE{sqGet - sqLose, 0, 0, sbj});
    }
    sort(stParam.vecGain.begin(),stParam.vecGain.end());

    /* 重要度 × 点数の累積和を求めておく */
    SQWORD sqCumSumU = 0;
    SQWORD sqCumSumL = 0;
    for (auto it = stParam.vecGain.begin(); it != stParam.vecGain.end(); ++it) {
        auto &sbj = it->stSbj;
        SQWORD sqGet = (stParam.sqScoreMax - sbj.sqScoreA) * sbj.sqWeightU;
        SQWORD sqLose = (0LL - sbj.sqScoreA) * sbj.sqWeightL;

        sqCumSumU += sqGet;
        sqCumSumL += sqLose;
        it->sqCumSumU = sqCumSumU;
        it->sqCumSumL = sqCumSumL;
    }

    SQWORD sqAns = binarySearch(isScoreGreater, -1, sqInput_N * sqInput_X, stParam);
    printf("%lld\n", sqAns);

    return 0;
}