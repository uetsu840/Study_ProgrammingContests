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
using LDOUBLE = long double;
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

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) {return 0 < a ? a: -a;}

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

/*----------------------------------------------*/

/*----------------------------------------------*/

struct NUMBER_ENTRY_ST {
    SQWORD sqA;
    SQWORD sqB;

    NUMBER_ENTRY_ST(SQWORD a, SQWORD b): sqA(a), sqB(b) {};
};


bool greaterA(const NUMBER_ENTRY_ST &a, const NUMBER_ENTRY_ST &b)
{
    return a.sqA < b.sqA;
} 

bool greaterB(const NUMBER_ENTRY_ST &a, const NUMBER_ENTRY_ST &b)
{
    return a.sqB < b.sqB;
} 


#define MAX_B   (5000)

#define ANS_MOD (998244353)

int main(void)
{
    SQWORD sqN = inputSQWORD();

    vector<SQWORD> vsqA;
    vector<SQWORD> vsqB;
    SQWORD sqMaxB = 0;
    SQWORD sqMaxA = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA  =inputSQWORD();
        vsqA.emplace_back(sqA);
        sqMaxA = MAX(sqMaxA, sqA);
    }
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqB = inputSQWORD();
        sqMaxB = MAX(sqMaxB, sqB);
        vsqB.emplace_back(sqB);
    }
    vector<NUMBER_ENTRY_ST> vstNumbers;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        vstNumbers.emplace_back(vsqA[sqIdx], vsqB[sqIdx]);
    }

    sort(vstNumbers.begin(), vstNumbers.end(), greaterA);
    vector<SQWORD> vsqDp(sqMaxA + 1, 0);

    /**
     *  dp[i][j]
     *      i番目までを考慮したとき、合計がjであるような組み合わせの数
     *  dp[-1][0] = 1 (初期値)
     *  dp[i + 1][j + a[k]] += dp[i][j]      if j + a[k] <= b[k]
     */
    SQWORD sqAns = 0;
    vsqDp[0] = 1;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqCurA = vstNumbers[sqIdx].sqA;
        SQWORD sqCurB = vstNumbers[sqIdx].sqB;
        for (SQWORD sqSum = sqMaxA; 0 <= sqSum; sqSum--) {
            if (sqSum + sqCurB <= sqMaxA) {
                vsqDp[sqSum + sqCurB] += vsqDp[sqSum];
                vsqDp[sqSum + sqCurB] %= ANS_MOD;
                if (sqSum + sqCurB <= sqCurA) {
                    sqAns += vsqDp[sqSum];
                    sqAns %= ANS_MOD;
                }
            }
        }
    }
    printf("%lld\n", sqAns);

    return 0;
}
