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
static SQWORD addModM(SQWORD x, SQWORD y, SQWORD m)
{ 
    return ((x % m) + (y % m)) % m;
}
 
static SQWORD subModM(SQWORD x, SQWORD y, SQWORD m)
{
    return ((x % m) - (y % m) + m) % m;
}

static SQWORD mulModM(SQWORD x, SQWORD y, SQWORD m) 
{
    return ((x % m) * (y % m)) % m;
}
 
static SQWORD powModM(SQWORD x, SQWORD e, SQWORD m) {
    SQWORD v = 1;
    for (; e; x = mulModM(x, x, m), e >>= 1) {
        if (e & 1) {
            v = mulModM(v, x, m);
        }
    }
    return v;
}

/**
 *  整数のべき乗演算
 */
static SQWORD powSQWORD(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = x*x, e >>= 1) {
        if (e & 1) {
            v = v*x;
        }
    }
    return v;
}

/*------------------------------------------------*/

/**
 *  f(l)
 *      10^(k-1)d + 10^(k-2)d + 10^(k-3)d + ..... + 1 を求める。
 */
static SQWORD calcTenPowSequenceMod(
    SQWORD sqDigits,    /* k */
    SQWORD sqBaseNum,   /* 10^d */
    SQWORD sqMod)       /* Mod */
{

    if (0 == sqDigits) {
        return 0;
    }

    if (0 == (sqDigits % 2)) {
        SQWORD sqL = sqDigits / 2;
        SQWORD sqModHalf = calcTenPowSequenceMod(sqL, sqBaseNum, sqMod);
        SQWORD sqRet = mulModM(sqModHalf
                                , powModM(sqBaseNum, sqL, sqMod)
                                , sqMod);
        sqRet = addModM(sqRet, sqModHalf, sqMod); 
        return sqRet;
    } else {
        SQWORD sqL = sqDigits - 1;
        SQWORD sqRet =  calcTenPowSequenceMod(sqL, sqBaseNum, sqMod);
        sqRet = mulModM(sqRet, sqBaseNum, sqMod);
        sqRet = addModM(sqRet, 1, sqMod);
        return sqRet;
    }
}

static SQWORD calcModA(
    SQWORD sqDigits,    /* k */
    SQWORD sqBaseNum,   /* 10^d */
    SQWORD sqInput_M,   /* M */
    SQWORD sqInput_A)   /* A */
{
//    printf("calcModA k[%lld] sqBaseNum[%lld], A[%lld]\n", sqDigits, sqBaseNum, sqInput_A);

    SQWORD sqRet = mulModM(sqInput_A, 
                            calcTenPowSequenceMod(sqDigits, sqBaseNum, sqInput_M), 
                            sqInput_M);
    return sqRet;
}    


/**
 *  g(l) = 0*10(k-1)d + B * 10^(k-2)d + 2B * 10^(k-3)d + ..... + (k-1)B * 10^0 を求める。
 * 
 *  g(l+1) = g(l) * sqBaseNum + Bl
 *  g(2l)  = g(l) * sqBaseNum^l + g(l) + Bl*f(l) 
 */
static SQWORD calcModB(
    SQWORD sqDigits,    /* k */
    SQWORD sqBaseNum,   /* 10^d */
    SQWORD sqMod,       /* m */
    SQWORD sqInput_B)
{
    if (0 == sqDigits) {
        return 0;
    }

    SQWORD sqRet;

    if (0 == (sqDigits % 2)) {
        SQWORD sqL = sqDigits / 2;
        SQWORD sqModHalf = calcModB(sqL, sqBaseNum, sqMod, sqInput_B);

        /* Upper */
        SQWORD sqUpper = mulModM(sqModHalf,
                                    powModM(sqBaseNum, sqL, sqMod),
                                    sqMod);
        sqUpper = addModM(sqUpper, sqModHalf, sqMod);

        /* Lower */
        SQWORD sqLower = calcTenPowSequenceMod(sqL, sqBaseNum, sqMod);
        sqLower = mulModM(sqLower,
                            mulModM(sqL, sqInput_B, sqMod),
                            sqMod);

        sqRet = addModM(sqUpper, sqLower, sqMod);
        return sqRet;
    } else {
        SQWORD sqL = sqDigits - 1;
        SQWORD sqRet = calcModB(sqL, sqBaseNum, sqMod, sqInput_B);
        sqRet = mulModM(sqRet, sqBaseNum, sqMod);
        sqRet = addModM(sqRet, mulModM(sqInput_B, sqL, sqMod), sqMod);

        return sqRet;
    }
}


/**
 * メイン
 * 
 *      何パターンか通らない。おそらくループのインデックスの扱いが違うと思う。
 *      (そこをYoutubeからコピーしてきた(solver4.cpp)ら、通った)
 */

int main(void)
{
    SQWORD sqInput_L = inputSQWORD();
    SQWORD sqInput_A = inputSQWORD();
    SQWORD sqInput_B = inputSQWORD();
    SQWORD sqInput_M = inputSQWORD();

    SQWORD sqSequenceMax = sqInput_A + sqInput_B * (sqInput_L - 1);

    SQWORD sqAns = 0;
    SQWORD sqDecOfsPowMod = 1;

    /* 後ろ側から順に計算してゆく */
    for (SQWORD sqDecPowIdx = 17; 0 <= sqDecPowIdx; sqDecPowIdx--) {
        SQWORD sqL = powSQWORD(10LL, sqDecPowIdx);
        SQWORD sqR = powSQWORD(10LL, sqDecPowIdx + 1LL) - 1LL;

        SQWORD sqIdxR, sqIdxL;
        if (sqL <= sqInput_A) {
            sqIdxL = 0;
        } else {
            sqIdxL = (sqL - sqInput_A + (sqInput_B - (SQWORD)1)) / sqInput_B;
        }
        if (sqSequenceMax <= sqR) {
            sqIdxR = sqInput_L - (SQWORD)1;
        } else {
            if (sqR < sqInput_A) {
                sqIdxR = -1;
            } else {
                sqIdxR = (sqR - sqInput_A) / sqInput_B;
            }
        }
        if (sqIdxL <= sqIdxR) {
            SQWORD sqTmp_A = sqInput_A + sqInput_B * sqIdxL;
            SQWORD sqK = sqIdxR - sqIdxL + 1;

//            printf(":::[%lld %lld] %lld %lld\n", sqIdxL, sqIdxR, sqK, sqDecPowIdx);
            SQWORD sqPartA = calcModA(sqK, powSQWORD(10, sqDecPowIdx + 1), sqInput_M, sqTmp_A);
            SQWORD sqPartB = calcModB(sqK, powSQWORD(10, sqDecPowIdx + 1), sqInput_M, sqInput_B);

            SQWORD sqSum = addModM(sqPartA, sqPartB, sqInput_M);
//            printf("A:%lld B:%lld M<%lld>\n", sqPartA, sqPartB, sqSum);            
            sqAns = addModM(sqAns, mulModM(sqSum, sqDecOfsPowMod, sqInput_M), sqInput_M);

            sqDecOfsPowMod = mulModM(sqDecOfsPowMod, 
                                    powModM(10, 
                                            mulModM((sqIdxR - sqIdxL + 1), (sqDecPowIdx + 1), sqInput_M),
                                            sqInput_M),
                                    sqInput_M);
//            printf("ofs %lld\n", sqDecOfsPowMod);
        }
    }
    printf("%lld\n", sqAns);

    return 0;
}