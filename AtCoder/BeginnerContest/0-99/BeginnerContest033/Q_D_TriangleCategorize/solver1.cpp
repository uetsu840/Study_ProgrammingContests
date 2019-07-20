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

/*----------------------------------------------*/
struct POSITION {
    SDWORD lX;
    SDWORD lY;
};

#define MAX_POINTS  (2000)

static inline SDWORD length2(const POSITION *pstPos1, const POSITION *pstPos2)
{
    SDWORD lDx = (pstPos1->lX - pstPos2->lX);
    SDWORD lDy = (pstPos1->lY - pstPos2->lY);

    return (lDx * lDx) + (lDy * lDy);
}

static SDWORD s_aalEdge[MAX_POINTS][MAX_POINTS];

int main(void)
{
    static POSITION s_astPosition[MAX_POINTS];

    SDWORD lInput_N = inputSDWORD();

    for (SDWORD lIdx = 0; lIdx < lInput_N; lIdx++) {
        SDWORD lInput_x = inputSDWORD();
        SDWORD lInput_y = inputSDWORD();

        s_astPosition[lIdx].lX = lInput_x;
        s_astPosition[lIdx].lY = lInput_y;
    }

    for (SDWORD lIdx0 = 0; lIdx0 < lInput_N - 1; lIdx0++) {
        for (SDWORD lIdx1 = lIdx0 + 1; lIdx1 < lInput_N; lIdx1++) {
            POSITION pos0 = s_astPosition[lIdx0];
            POSITION pos1 = s_astPosition[lIdx1];
            s_aalEdge[lIdx0][lIdx1] = length2(&pos0, &pos1);
        }
    }

    SDWORD lAns0 = 0;
    SDWORD lAns1 = 0;
    SDWORD lAns2 = 0;

    for (SDWORD lIdx0 = 0; lIdx0 < lInput_N - 2; lIdx0++) {
        for (SDWORD lIdx1 = lIdx0 + 1; lIdx1 < lInput_N - 1; lIdx1++) {
            for (SDWORD lIdx2 = lIdx1 + 1; lIdx2 < lInput_N; lIdx2++) {
                SDWORD lA = s_aalEdge[lIdx0][lIdx1];
                SDWORD lB = s_aalEdge[lIdx1][lIdx2];
                SDWORD lC = s_aalEdge[lIdx0][lIdx2];

                /* lA < lB < lC にする。 */
                if (lB < lA) {
                    swap(lA, lB);
                }
                if (lC < lA) {
                    swap(lA, lC);
                }
                if (lC < lB) {
                    swap(lC, lB);
                }

//                printf("-- %d %d %d\n", lA, lB, lC);

                if (lC < lA + lB) {
                    lAns0++;
                } else if (lC == lA + lB){
                    lAns1++;
                } else {
                    lAns2++;
                }
            }
        }
    }
    printf("%d %d %d\n", lAns0, lAns1, lAns2);


    return 0;
}