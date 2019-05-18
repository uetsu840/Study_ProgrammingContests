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

static SQWORD getCombination(SQWORD n, SQWORD k)
{
    SQWORD sqRet = 1;
    for (SQWORD sqIdx = 0; sqIdx < k; sqIdx++) {
        sqRet *= (n-sqIdx);
    }
    for (SQWORD sqIdx = 0; sqIdx < k; sqIdx++) {
        sqRet /= (sqIdx + 1);
    }

    return sqRet;
}

#define SQWORD_INF_N    (-1000000000000000000LL)

int main()
{
    SDWORD lInput_N = inputSDWORD();
    vector<SQWORD> vsqInput_a(3 * lInput_N);

    for (SDWORD lIdx = 0; lIdx < 3 * lInput_N; lIdx++) {
        vsqInput_a[lIdx] = inputSDWORD();
    }

    vector<SQWORD> vSumL;
    vector<SQWORD> vSumR;

    /* 左半分 */
    priority_queue<SQWORD, vector<SQWORD>, greater<SQWORD>> pq;
    SQWORD sqSum = 0;
    for (SDWORD lIdx = 0; lIdx < lInput_N; lIdx++) {
        pq.push((SQWORD)vsqInput_a[lIdx]);
        sqSum += vsqInput_a[lIdx];
    }
    vSumL.emplace_back(sqSum);
    for (SDWORD lAddIdx = lInput_N; lAddIdx < lInput_N * 2; lAddIdx++) {
        SQWORD sqAddVal = (SQWORD)vsqInput_a[lAddIdx];
        pq.push(sqAddVal);
        SQWORD sqRemovedVal = pq.top();
        pq.pop();
        sqSum += (sqAddVal - sqRemovedVal);
        vSumL.emplace_back(sqSum);
    }

    /* 右半分 */
    priority_queue<SQWORD> pq_r;
    SQWORD sqSumR = 0;
    for (SDWORD lIdx = lInput_N * 2; lIdx < lInput_N * 3; lIdx++) {
        pq_r.push((SQWORD)vsqInput_a[lIdx]);
        sqSumR += vsqInput_a[lIdx];
    }
    vSumR.emplace_back(sqSumR);
    for (SDWORD lAddIdx = lInput_N * 2 - 1; lInput_N - 1 < lAddIdx; lAddIdx--) {
        SQWORD sqAddVal = (SQWORD)vsqInput_a[lAddIdx];
        pq_r.push(sqAddVal);
        SQWORD sqRemovedVal = pq_r.top();
        pq_r.pop();
        sqSumR += (sqAddVal - sqRemovedVal);
        vSumR.emplace_back(sqSumR);
//        printf("R: %lld %lld %lld\n", sqAddVal, sqRemovedVal, sqSumR);
    }
    reverse(vSumR.begin(), vSumR.end());

    SQWORD sqAns = SQWORD_INF_N;
    for (SDWORD lIdx = 0; lIdx < vSumL.size(); lIdx++) {
//        printf("%lld %lld\n", vSumL[lIdx], vSumR[lIdx]);
        sqAns = max(sqAns, vSumL[lIdx] - vSumR[lIdx]);
    }
    printf("%lld\n", sqAns);

    return 0; 
}