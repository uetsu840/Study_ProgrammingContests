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
#include <bitset>
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

#define MAX_N   (2000)
char szInput[MAX_N + 1];
static bitset<MAX_N + 1> s_abitMatchTbl[MAX_N + 1];

int main(void)
{
    SDWORD lInput_n = inputSDWORD();
    for (SDWORD lIdxI = 2; lIdxI <= lInput_n; lIdxI++) {
        inputString(szInput);
        for (SDWORD lIdxJ = 1; lIdxJ < lIdxI; lIdxJ++) {
            if ('0' == szInput[lIdxJ - 1]) {
                s_abitMatchTbl[lIdxJ][lIdxI] = true;
            }
        }
    }

    static bitset<MAX_N + 1> s_absDpL[MAX_N+1];
    static bitset<MAX_N + 1> s_absDpR[MAX_N+1];

    for (SDWORD lPos = 1; lPos <= lInput_n; lPos++) {
        s_absDpL[lPos][lPos] = 1;
        s_absDpR[lPos][lPos] = 1;
    }
    for (SDWORD lUpdateWidth = 1; lUpdateWidth <= lInput_n; lUpdateWidth++) {
        for (SDWORD lUpdIdxL = 1; lUpdIdxL <= lInput_n - lUpdateWidth; lUpdIdxL++) {
            SDWORD lUpdIdxR = lUpdIdxL + lUpdateWidth;
            /* LR */
            bool bCondLR = false;
            bool bCondLL = false;
            bool bCondRR = false;
            for (SDWORD lUpdIdxM = lUpdIdxL; lUpdIdxM < lUpdIdxR; lUpdIdxM++) {
                bool bDpL  = s_absDpL[lUpdIdxL][lUpdIdxM];
                bool bDpML = s_absDpL[lUpdIdxM][lUpdIdxR];
                bool bDpR  = s_absDpR[lUpdIdxL][lUpdIdxM];
                bool bDpMR = s_absDpR[lUpdIdxM][lUpdIdxR];
                bool bDp1R = s_absDpR[lUpdIdxM+1][lUpdIdxR];
                if (bDpL && bDp1R) {
                    bCondLR = true;
                }
                if (lUpdIdxM != lUpdIdxL) {
                    if (bDpL && bDpML) {
                        bCondLL = true;
                    }
                    if (bDpR && bDpMR) {
                        bCondRR = true;
                    }
                }
            }
            if (bCondLR) {
                if (s_abitMatchTbl[lUpdIdxL][lUpdIdxR]) {
                    s_absDpL[lUpdIdxL].set(lUpdIdxR, true);
                } else {
                    s_absDpR[lUpdIdxL].set(lUpdIdxR, true);
                }
            }
            if (bCondLL) {
                s_absDpL[lUpdIdxL].set(lUpdIdxR, true);
            }
            if (bCondRR) {
                s_absDpR[lUpdIdxL].set(lUpdIdxR, true);
            }
        }
    }

    /* count winners */
    SDWORD lAns = 0;
    for (SDWORD lMid = 1; lMid <= lInput_n; lMid++) {
        if (s_absDpR[1][lMid] && s_absDpL[lMid][lInput_n]) {
            lAns++;
        }
    }

    printf("%d\n", lAns);

    return 0;
}