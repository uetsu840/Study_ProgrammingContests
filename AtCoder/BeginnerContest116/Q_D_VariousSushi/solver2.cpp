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

/* 入力 */
typedef struct {
    SDWORD  lIdx;
    SDWORD  lType;
    SDWORD  lDel;
} SUSHI_ONE_ST;

#define MAX_SUSHI_NUM      (100000)

bool operator< (const SUSHI_ONE_ST &a, const SUSHI_ONE_ST &b)
{
    return a.lDel < b.lDel;
}

static vector<SUSHI_ONE_ST> s_vSushiSubstituteCandidate;
static vector<SUSHI_ONE_ST> s_avSushiNotUsed[MAX_SUSHI_NUM + 1];
static vector<SUSHI_ONE_ST> s_vSushiNotUsedChange;

int main()
{   
    SDWORD  lInput_N = inputSDWORD();
    SDWORD  lInput_K = inputSDWORD();

    vector<SUSHI_ONE_ST> vInput;
    for (SDWORD lIdx = 0; lIdx < lInput_N; lIdx++) {
        SUSHI_ONE_ST  stSushi;
        stSushi.lIdx  = lIdx; 
        stSushi.lType = inputSDWORD();
        stSushi.lDel  = inputSDWORD();

        vInput.emplace_back(stSushi); 
    }

    /* K個を貪欲に選択する */
    static bool s_abIsTypeSelected[MAX_SUSHI_NUM + 1];
    static SDWORD s_alCounts[MAX_SUSHI_NUM + 1];
    SQWORD  sqTotalPnt = 0;
    SDWORD  lKindNum = 0;
    sort(vInput.begin(), vInput.end());
    for (SDWORD lIdx = 0; lIdx < vInput.size(); lIdx++) {
        SDWORD lType = vInput[lIdx].lType;
        SDWORD lPos = s_alCounts[vInput[lIdx].lType];
        s_alCounts[vInput[lIdx].lType]++;
        if (lIdx < lInput_K) {
            sqTotalPnt += (SQWORD)vInput[lIdx].lDel;
            if (s_abIsTypeSelected[lType]) {
                s_vSushiSubstituteCandidate.emplace_back(vInput[lIdx]);
            } else {
                s_abIsTypeSelected[lType] = true;
                lKindNum++;
            }
        } else {
            s_avSushiNotUsed[lType].emplace_back(vInput[lIdx]);
        }
    }

    /* 交換候補もソートする */
    sort(s_vSushiSubstituteCandidate.begin(), s_vSushiSubstituteCandidate.end());

    /* 使われていない寿司の先頭だけを取り出す */
    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_vSushiSubstituteCandidate); lIdx++) {
        sort(s_avSushiNotUsed[lIdx].begin(),
                s_avSushiNotUsed[lIdx].end());
        s_vSushiNotUsedChange.emplace_back(s_avSushiNotUsed[lIdx][0]);
    }

    printf("first K : [%lld]\n", sqTotalPnt);
    
    return 0;
}