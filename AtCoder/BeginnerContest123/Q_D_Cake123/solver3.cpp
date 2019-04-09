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
#include <bits/stdc++.h>
#include <functional>
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

static SQWORD calcZeroToNumXor(SQWORD sqNum)
{
    SQWORD sqRet = 0;
    for (DWORD dwBit = 1; dwBit < 64; dwBit++) {
        SQWORD sqMask = 0x1LL << dwBit;
        if (sqMask & sqNum) {
            if (0 == (sqNum & 0x1)) {
                sqRet |= sqMask;
            }
        }
    }

    /* lowest bit */
    if ((sqNum % 4 == 1) || (sqNum % 4 == 2)) {
        sqRet |= 1;
    }

    return sqRet;
}

#define SQWORD_INF_N        (-10000000000)

typedef tuple<SDWORD, SDWORD, SDWORD> cake_indice;
typedef struct {
    SQWORD sqVal;
    SDWORD lIdxA;
    SDWORD lIdxB;
    SDWORD lIdxC;
} ST_CAKE_SELECT;

typedef  priority_queue<ST_CAKE_SELECT, vector<ST_CAKE_SELECT>, 
                 function<bool(ST_CAKE_SELECT, ST_CAKE_SELECT)>> pq_cake;
typedef set<cake_indice> set_cake;

typedef struct {
    vector<SQWORD> vsqInput_A;
    vector<SQWORD> vsqInput_B;
    vector<SQWORD> vsqInput_C;
} ST_INPUT;

static void addCakeSet(
    pq_cake &q,
    set_cake &setCakes,
    ST_INPUT &stInput,
    SDWORD lIdxA, SDWORD lIdxB, SDWORD lIdxC)
{
    SQWORD sqValue = stInput.vsqInput_A[lIdxA] + stInput.vsqInput_B[lIdxB] + stInput.vsqInput_C[lIdxC];
    q.push(ST_CAKE_SELECT{sqValue, lIdxA, lIdxB, lIdxC});
    setCakes.insert((cake_indice){lIdxA, lIdxB, lIdxC});   
}

int main()
{
    ST_INPUT stInput;

    SQWORD sqInput_X = inputSQWORD();
    SQWORD sqInput_Y = inputSQWORD();
    SQWORD sqInput_Z = inputSQWORD();
    SQWORD sqInput_K = inputSQWORD();

    for (SQWORD sqIdx = 0; sqIdx < sqInput_X; sqIdx++) {
        stInput.vsqInput_A.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqInput_Y; sqIdx++) {
        stInput.vsqInput_B.emplace_back(inputSQWORD());
    }
    for (SQWORD sqIdx = 0; sqIdx < sqInput_Z; sqIdx++) {
        stInput.vsqInput_C.emplace_back(inputSQWORD());
    }
    sort(stInput.vsqInput_A.begin(), stInput.vsqInput_A.end(), greater<SQWORD>());
    sort(stInput.vsqInput_B.begin(), stInput.vsqInput_B.end(), greater<SQWORD>());
    sort(stInput.vsqInput_C.begin(), stInput.vsqInput_C.end(), greater<SQWORD>());


    /* add index0(largest) */
    auto cake_is_high = [](ST_CAKE_SELECT l, ST_CAKE_SELECT r){ return l.sqVal < r.sqVal; };
	pq_cake q(cake_is_high);
    set<cake_indice> setCakes;

    addCakeSet(q, setCakes, stInput, 0, 0, 0);

    vector<SQWORD> vsqAns;

    for (SDWORD lIdx = 0; lIdx < sqInput_K; lIdx++) {
        ST_CAKE_SELECT sel = q.top();
        vsqAns.emplace_back(sel.sqVal);
        q.pop();

        SDWORD lIdxA = sel.lIdxA;
        SDWORD lIdxB = sel.lIdxB;
        SDWORD lIdxC = sel.lIdxC;

        if (lIdxA + 1 < sqInput_X) {
            auto next = (cake_indice){lIdxA+1, lIdxB, lIdxC};
            auto it = setCakes.find(next);
            if (it == setCakes.end()) {
                addCakeSet(q, setCakes, stInput, lIdxA + 1, lIdxB, lIdxC);
            }
        }
        if (lIdxB + 1 < sqInput_Y) {
            auto next = (cake_indice){lIdxA, lIdxB+1, lIdxC};
            auto it = setCakes.find(next);
            if (it == setCakes.end()) {
                addCakeSet(q, setCakes, stInput, lIdxA, lIdxB + 1, lIdxC);
            }
        }
        if (lIdxC + 1 < sqInput_Z) {
            auto next = (cake_indice){lIdxA, lIdxB, lIdxC+1};
            auto it = setCakes.find(next);
            if (it == setCakes.end()) {
                addCakeSet(q, setCakes, stInput, lIdxA, lIdxB, lIdxC + 1);
            }
        }
    }
    for (auto it: vsqAns) {
        printf("%lld\n", it);
    }

    return 0;
}