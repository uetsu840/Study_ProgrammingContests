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


struct MOBILE {
    SDWORD  lLenR;
    SDWORD  lLenB;
    SDWORD  lPtrR;
    SDWORD  lPtrB;
};

static SQWORD getMinWeight(const vector<MOBILE> vM, SDWORD lNodeNo)
{
    const MOBILE *pM = &(vM[lNodeNo]);
    SDWORD lMinWeightR, lMinWeightB;
    if (0 == pM->lPtrR) {
        lMinWeightR = 1;
    } else {
        lMinWeightR = getMinWeight(vM, pM->lPtrR);
    }

    if (0 == pM->lPtrB) {
        lMinWeightB = 1;
    } else {
        lMinWeightB = getMinWeight(vM, pM->lPtrB);
    }

    SDWORD lGcd = __gcd(pM->lLenB, pM->lLenR);
    SDWORD lRatioR = pM->lLenR / lGcd;
    SDWORD lRatioB = pM->lLenB / lGcd;

    SDWORD lGcdW = __gcd((lRatioR * lMinWeightR), (lRatioB * lMinWeightB));
    SDWORD lWMulR = (lRatioB * lMinWeightB) / lGcdW;
    SDWORD lWMulB = (lRatioR * lMinWeightR) / lGcdW;
    
    return lMinWeightR * lWMulR + lMinWeightB * lWMulB;
}


int main()
{
    SQWORD sqInput_N = inputSQWORD();
    vector<MOBILE> vMobile(sqInput_N + 1);
    vector<SDWORD> vlRoot(sqInput_N + 1, 0);

    for (SDWORD lIdx = 1; lIdx <= sqInput_N; lIdx++) {
        SDWORD lInput_p = inputSDWORD();
        SDWORD lInput_q = inputSDWORD();
        SDWORD lInput_r = inputSDWORD();
        SDWORD lInput_b = inputSDWORD();

        vMobile[lIdx] = MOBILE{lInput_p, lInput_q, lInput_r, lInput_b};

        vlRoot[lInput_r] = lIdx;
        vlRoot[lInput_b] = lIdx;
    }

    /* search root */
    SDWORD lNode = 1;
    while(1) {
        if (vlRoot[lNode] == 0) {
            break;
        }
        lNode = vlRoot[lNode];
    }
    SDWORD lRoot = lNode;
    
    /* search mobile weight */
    SQWORD sqAns = getMinWeight(vMobile, lRoot);

    printf("%lld\n", sqAns);

    return 0;
}