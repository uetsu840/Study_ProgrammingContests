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

typedef struct {
    SQWORD sqScoreA;
    SQWORD sqWeightL;
    SQWORD sqWeithtU;
} TEST_ONE_SBJ;


typedef struct {
    SQWORD sqGain;
    TEST_ONE_SBJ stSbj;
} TEST_GAIN_ONE;

bool operator< (const TEST_GAIN_ONE a, const TEST_GAIN_ONE b) 
{
    if (a.sqGain == b.sqGain) {
        return a.stSbj.sqScoreA < b.stSbj.sqScoreA;
    }
    return a.sqGain > b.sqGain;
}

/**
*   判定
*       true:   上側に超過
*       false:  下側に超過
*/
static bool isScoreGreater(
    SQWORD sqTryScore, 
    vector<TEST_GAIN_ONE> &vecGain, 
    SQWORD sqScoreMax)
{
//    printf("trying...[%lld] %d\n", sqTryScore, vecGain.size());

    SQWORD sqScoreRest = sqTryScore;
    SQWORD sqDiff = 0;
    for (auto sbj_g : vecGain) {
//        printf("score rest...[%lld] [%lld:%lld:%lld]\n", sqScoreRest, sbj_g.stSbj.sqScoreA, sbj_g.stSbj.sqWeightL, sbj_g.stSbj.sqWeithtU);
        if (sqScoreRest <= sqScoreMax) {
            /* partial score */
            SQWORD sqScore = sqScoreRest;
            SQWORD sqWeight;
            if (sbj_g.stSbj.sqScoreA < sqScore) {
                sqWeight = sbj_g.stSbj.sqWeithtU;
            } else {
                sqWeight = sbj_g.stSbj.sqWeightL;
            }
            sqDiff += (sqScoreRest - sbj_g.stSbj.sqScoreA) * sqWeight;
            sqScoreRest -= sqScore;
        } else {
            /* full score */
            sqScoreRest -= sqScoreMax;
            sqDiff += (sqScoreMax - sbj_g.stSbj.sqScoreA) * sbj_g.stSbj.sqWeithtU;
        }
    }
//    printf("diff[%lld]\n", sqDiff);

    return 0 <= sqDiff;    
}

/**
*   2分探索
*/
static SQWORD binarySearch(
    bool (*judge)(SQWORD, vector<TEST_GAIN_ONE>&, SQWORD), 
    SQWORD sqInitLb, 
    SQWORD sqInitUb, 
    vector<TEST_GAIN_ONE> &vecTest, 
    SQWORD sqScoreMax)
{
    SQWORD sqLb = sqInitLb;
    SQWORD sqUb = sqInitUb;

    while (1LL < sqUb - sqLb) {
        SQWORD sqMid = (sqUb + sqLb) / 2LL;
        if (judge(sqMid, vecTest, sqScoreMax)) {
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

    SQWORD sqScoreMax = sqInput_X;
    vector<TEST_GAIN_ONE> vecGain;

    for (auto sbj: vecTests) {
        SQWORD sqGet = (sqScoreMax - sbj.sqScoreA) * sbj.sqWeithtU;
        SQWORD sqLose = (0LL - sbj.sqScoreA) * sbj.sqWeightL;
//        printf("push %lld %lld\n", sqGet, sqLose);

        vecGain.emplace_back(TEST_GAIN_ONE{sqGet - sqLose, sbj});
    }

    sort(vecGain.begin(),vecGain.end());
    SQWORD sqAns = binarySearch(isScoreGreater, 0, sqInput_N * sqInput_X, vecGain, sqInput_X);
    printf("%lld\n", sqAns);

    return 0;
}