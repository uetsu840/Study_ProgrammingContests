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

#define MAX_N   (2000)
#define MAX_M   (2000)
#define ANS_MOD (1000000007LL)

static SQWORD addMod(SQWORD a, SQWORD b) 
{
    return (a+b) % ANS_MOD;
}

static SQWORD subMod(SQWORD a, SQWORD b)
{
    return (ANS_MOD + a-b) % ANS_MOD;
}

int main()
{
    /* 順方向にDPをしてM個の商品の選び方を求める */
    SQWORD sqInput_N = inputSQWORD();
    SQWORD sqInput_M = inputSQWORD();
    SQWORD sqInput_Q = inputSQWORD();

    vector<SQWORD> vsqItemCnts;
    for (SDWORD lIdx = 0; lIdx < sqInput_N; lIdx++) {
        vsqItemCnts.emplace_back(inputSQWORD());
    }

    /* dp(i)[j] i番目までの商品で合計j個を選ぶ選び方 */
    /**
     *  dp(i)[j] = dp(i)[j-1] + dp(i-1)[j] - dp(i-1)[j-a-1]
     */
    SQWORD asqDpTbl[MAX_M + 1];
    memset(asqDpTbl, 0 , sizeof(asqDpTbl));
    asqDpTbl[0] = 1;
    for (SDWORD lItemNo = 1; lItemNo <= sqInput_N; lItemNo++) {
        SQWORD asqNextDpTbl[MAX_M + 1];
        for (SDWORD lCountIdx = 0; lCountIdx <= sqInput_M; lCountIdx++) {
            SQWORD sqIdxj1 = lCountIdx - 1;
            SQWORD sqIdxja1 = lCountIdx - vsqItemCnts[lItemNo - 1] - 1;
            SQWORD sqNextDp_j1 = (0 <= sqIdxj1) ? asqNextDpTbl[sqIdxj1] : 0;
            SQWORD sqDp_j = asqDpTbl[lCountIdx];
            SQWORD sqDp_ja1 = (0 <= sqIdxja1) ? asqDpTbl[sqIdxja1] : 0;

            SQWORD sqNextDpVal = addMod(sqNextDp_j1, sqDp_j);
            sqNextDpVal = subMod(sqNextDpVal, sqDp_ja1);
            asqNextDpTbl[lCountIdx] = sqNextDpVal % ANS_MOD;
        }
        memcpy(asqDpTbl, asqNextDpTbl, sizeof(asqDpTbl));
    }

    vector<pair<SQWORD, SQWORD>> vecQuery; 
    unordered_set<SQWORD> setItems;

    for (SDWORD lQueryIdx = 0; lQueryIdx < sqInput_Q; lQueryIdx++) {
        SQWORD sqInput_k = inputSQWORD();
        SQWORD sqInput_a = inputSQWORD();

        vecQuery.emplace_back(make_pair(sqInput_k, sqInput_a));
        setItems.insert(sqInput_k);
    }

    static SQWORD aasqPrevDpTbl[MAX_N+1][MAX_M + 1];
    for (auto itemNo: setItems) {
        SQWORD sqItemIdx = itemNo - 1;
        SQWORD sqCount_item = vsqItemCnts[sqItemIdx];

        for (SDWORD lCountIdx = 0; lCountIdx <= sqInput_M; lCountIdx++) {
            SQWORD sqIdxj1 = lCountIdx - 1;
            SQWORD sqIdxja1 = lCountIdx - sqCount_item - 1;
            SQWORD sqDp_j1 = (0 <= sqIdxj1) ? asqDpTbl[sqIdxj1] : 0;
            SQWORD sqPrevDp_ja1 = (0 <= sqIdxja1) ? aasqPrevDpTbl[sqItemIdx][sqIdxja1] : 0;

            SQWORD sqPrevDpVal = subMod(asqDpTbl[lCountIdx], sqDp_j1);
            sqPrevDpVal = addMod(sqPrevDpVal, sqPrevDp_ja1);
            aasqPrevDpTbl[sqItemIdx][lCountIdx] = sqPrevDpVal % ANS_MOD;
        }
    }
    

    /* 戻すdp */
    /**
     *  dp(N-1)[j] = dp(N)[j] - dp(N)[j-1] + dp(N-1)[j-a-1]
     */
    for (auto query: vecQuery) {
        printf("%lld\n", aasqPrevDpTbl[query.first - 1][sqInput_M - query.second]);
    }


    return 0;
}