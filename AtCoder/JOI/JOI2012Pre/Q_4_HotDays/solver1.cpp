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

#define MAX_TTL_W   (10000)
#define MAX_N       (50)
#define MAX_K       (MAX_N)
#define MAX_B       (MAX_W * MAX_N)

struct CLOTHES_ONE {
    SDWORD lLower;
    SDWORD lUpper;
    SDWORD lPnt;
};

#define MAX_NUM_CLOTHES     (200)

int main()
{
    SDWORD lInput_D = inputSDWORD();
    SDWORD lInput_N = inputSDWORD();

    vector<SDWORD> vHighestTemp(lInput_D, 0);
    vector<CLOTHES_ONE> vClothes(lInput_N);

    for (SDWORD lDayIdx = 0; lDayIdx < lInput_D; lDayIdx++) {
        SDWORD lTemp = inputSDWORD();
        vHighestTemp[lDayIdx] = lTemp;
    }

    for (SDWORD lClothesIdx = 0; lClothesIdx < lInput_N; lClothesIdx++) {
        CLOTHES_ONE stClothes;
        stClothes.lLower = inputSDWORD();
        stClothes.lUpper = inputSDWORD();
        stClothes.lPnt = inputSDWORD();
        vClothes[lClothesIdx] = stClothes;
    }

    /* dp (i) [j] (i日目に服jを着用したときの、派手さの最大値。 */
    static SQWORD asqDpTbl[MAX_NUM_CLOTHES + 1];
    static SQWORD asqNextDpTbl[MAX_NUM_CLOTHES + 1];

    #define     VAL_UNWEARABLE  (10000000000)
    /* Initialize DP Tbl */
    for (SDWORD lClothesIdx = 0; lClothesIdx < lInput_N; lClothesIdx++) {
        if ((vClothes[lClothesIdx].lLower <= vHighestTemp[0]) && (vHighestTemp[0] <= vClothes[lClothesIdx].lUpper)) {
            asqDpTbl[lClothesIdx] = 0;
        } else {
            asqDpTbl[lClothesIdx] = VAL_UNWEARABLE;
        }
    }

    /* Exec DP */
    for (SDWORD lDayIdx = 1; lDayIdx < lInput_D; lDayIdx++) {
        SDWORD lTmpToday = vHighestTemp[lDayIdx];
        for (SDWORD lClothesIdx = 0; lClothesIdx < lInput_N; lClothesIdx++) {
            if ((vClothes[lClothesIdx].lLower <= lTmpToday) && (lTmpToday <= vClothes[lClothesIdx].lUpper)) {
                SQWORD lMaxValue = 0;
                for (SDWORD lClothesPrevIdx = 0; lClothesPrevIdx < lInput_N; lClothesPrevIdx++) {
                    if (VAL_UNWEARABLE != asqDpTbl[lClothesPrevIdx]) {
                        lMaxValue = max(lMaxValue, 
                                        asqDpTbl[lClothesPrevIdx]
                                             + abs(vClothes[lClothesPrevIdx].lPnt - vClothes[lClothesIdx].lPnt));
                    } 
                }
                asqNextDpTbl[lClothesIdx] = lMaxValue;
            } else {
                asqNextDpTbl[lClothesIdx] = VAL_UNWEARABLE;
            }
        }
        memcpy(asqDpTbl, asqNextDpTbl, sizeof(asqDpTbl));
    }

    SQWORD sqAns = 0;
    for (SDWORD lClothesIdx = 0; lClothesIdx < lInput_D; lClothesIdx++) {
        if (VAL_UNWEARABLE != asqDpTbl[lClothesIdx]) {
            sqAns = max(sqAns, asqDpTbl[lClothesIdx]);
        }
    }
    printf("%lld\n", sqAns);

    return 0;
}