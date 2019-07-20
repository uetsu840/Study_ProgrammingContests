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

#define DPTBL_SIZE  (100000)

/*----------------------------------------------*/
int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_K = inputSQWORD();

    static SQWORD s_asqDpTbl0[DPTBL_SIZE];  /* [1, sqrt(N)] */
    static SQWORD s_asqDpTbl1[DPTBL_SIZE];  /* (sqrt(N), N] */

    /* initialize */
    for (SQWORD sqIdx = 1; sqIdx < ArrayLength(s_asqDpTbl0); sqIdx++) {
        s_asqDpTbl0[sqIdx] = 1;
    }
    for (SQWORD sqIdx = 1; sqIdx < ArrayLength(s_asqDpTbl1); sqIdx++) {
        s_asqDpTbl1[sqIdx] = 1;
    }

    SQWORD sqBorder = sqrt(sqInput_N);

    /* update */
    for (SQWORD sqIdx = 1; sqIdx < sqInput_K; sqIdx++) {
        static SQWORD s_asqDpTbl1Prev[DPTBL_SIZE];  /* (sqrt(N), N] */
        SQWORD sqDpVal;
        memcpy(s_asqDpTbl1Prev, s_asqDpTbl1, sizeof(s_asqDpTbl1Prev));
        for (SQWORD sqDp1Idx = 1; sqDp1Idx <= sqInput_N / sqDp1Idx; sqDp1Idx++) {
            s_asqDpTbl1[sqDp1Idx] = addMod(s_asqDpTbl1[sqDp1Idx - 1], s_asqDpTbl0[sqDp1Idx]);
            if (sqDp1Idx < sqInput_N / sqDp1Idx) {
                 sqDpVal = s_asqDpTbl1[sqDp1Idx];
            }
//            printf(":%lld ", s_asqDpTbl1[sqDp1Idx]);
        }
        s_asqDpTbl0[sqBorder + 1] = sqDpVal;
        for (SQWORD sqDp0Idx = sqrt(sqInput_N); 1 <= sqDp0Idx; sqDp0Idx--) {
            SQWORD sqBoundUpper = sqInput_N / sqDp0Idx;
            SQWORD sqBoundLower = sqInput_N / (sqDp0Idx + 1);
            SQWORD sqSeqNum = sqBoundUpper - sqBoundLower;
            s_asqDpTbl0[sqDp0Idx] = addMod(s_asqDpTbl0[sqDp0Idx + 1], mulMod(s_asqDpTbl1Prev[sqDp0Idx], sqSeqNum));
//            printf("_%lld-%lld ", sqDp0Idx, s_asqDpTbl0[sqDp0Idx]);
        }
//        printf("\n");
    }

    /* sum up */
    SQWORD sqAns = 0;
    for (SQWORD sqDp1Idx = 1; sqDp1Idx < sqInput_N / sqDp1Idx; sqDp1Idx++) {
        SQWORD sqBoundUpper = sqInput_N / sqDp1Idx;
        SQWORD sqBoundLower = sqInput_N / (sqDp1Idx + 1);
        SQWORD sqSeqNum = sqBoundUpper - sqBoundLower;
        sqAns = addMod(sqAns, mulMod(s_asqDpTbl1[sqDp1Idx], sqSeqNum));
    }
    for (SQWORD sqDp0Idx = sqBorder; 1 <= sqDp0Idx; sqDp0Idx--) {
        sqAns = addMod(sqAns, s_asqDpTbl0[sqDp0Idx]);
    }
    printf("%lld\n", sqAns);

    return 0;
}