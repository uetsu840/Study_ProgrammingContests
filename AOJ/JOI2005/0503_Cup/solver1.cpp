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

#define     MAX_NUM_CUPS    (15)

typedef struct {
    BYTE    abyPos[MAX_NUM_CUPS];
    DWORD   dwDepth;
} CUP_STATE;

static bool s_abIsVisited[15000000];

static DWORD getIdx(CUP_STATE *pstState, DWORD dwCupNum)
{
    DWORD dwRetVal = 0;
    DWORD dwMultiplier = 1;
    for (DWORD dwIdx = 0; dwIdx < dwCupNum; dwIdx++) {
        dwRetVal += pstState->abyPos[dwIdx] * dwMultiplier;
        dwMultiplier *= 3;
    }
    return dwRetVal;
}

static CUP_STATE s_stGoalState1;
static CUP_STATE s_stGoalState2;

static bool detectGoal(CUP_STATE *pstState)
{
    if (0 == memcmp(&s_stGoalState1.abyPos, pstState->abyPos, sizeof(pstState->abyPos))) {
        return true;
    }
    if (0 == memcmp(&s_stGoalState2.abyPos, pstState->abyPos, sizeof(pstState->abyPos))) {
        return true;
    }
    return false;
}


/**
 *  次の状態遷移
 */
static void visitNext(
    queue<CUP_STATE> &queSearch, 
    const CUP_STATE *pstCurState,
    DWORD dwMoveCup,
    DWORD dwNextPos,
    DWORD dwCupNum)
{
    CUP_STATE stNextState = *pstCurState;
    stNextState.abyPos[dwMoveCup] = dwNextPos;          
    stNextState.dwDepth = pstCurState->dwDepth + 1;     
    SDWORD lNextIdx = getIdx(&stNextState, dwCupNum);
    if (!s_abIsVisited[lNextIdx]) {
        s_abIsVisited[lNextIdx] = true;
        queSearch.push(stNextState);                
    }
}


static SDWORD solveBfs(
    CUP_STATE *pstStartState,
    DWORD dwCupNum,
    SDWORD lIterMax)
{
    queue<CUP_STATE> queSearch;
    memset(s_abIsVisited, 0, sizeof(s_abIsVisited));

    s_abIsVisited[getIdx(pstStartState, dwCupNum)] = true;
    queSearch.push(*pstStartState);

    while(0 < queSearch.size()) {
        auto state = queSearch.front();
        queSearch.pop();

        if (lIterMax < state.dwDepth) {
            return -1;
        }
        if (detectGoal(&state)) {
            return state.dwDepth;
        }

        DWORD dwTop_A = 0, dwTop_B = 0, dwTop_C = 0;

        for (DWORD dwIdx = 0; dwIdx < dwCupNum; dwIdx++) {
            if (state.abyPos[dwIdx] == 0) {
                dwTop_A = max(dwTop_A, dwIdx+1);
            }
            if (state.abyPos[dwIdx] == 1) {
                dwTop_B = max(dwTop_B, dwIdx+1);
            }
            if (state.abyPos[dwIdx] == 2) {
                dwTop_C = max(dwTop_C, dwIdx+1);
            } 
        }

        if (dwTop_A < dwTop_B) {
            /* B->A */
            visitNext(queSearch, &state, dwTop_B-1, 0, dwCupNum);
        } else if (dwTop_B < dwTop_A) {
            /* A->B */
            visitNext(queSearch, &state, dwTop_A-1, 1, dwCupNum);
        }
        if (dwTop_B < dwTop_C) {
            /* C->B */
            visitNext(queSearch, &state, dwTop_C-1, 1, dwCupNum);
        } else if (dwTop_C < dwTop_B) {
            /* B->C */
            visitNext(queSearch, &state, dwTop_B-1, 2, dwCupNum);
        }
    }
    return -1;
}


int main()
{
    for (;;) {
        SQWORD sqInput_n = inputSDWORD();
        SQWORD sqInput_m = inputSQWORD();

        if ((0 == sqInput_n) && (0 == sqInput_m)) {
            break;
        }

        CUP_STATE stCupState;

        memset(&stCupState, 0, sizeof(stCupState));

        for (DWORD dwTrayIdx = 0; dwTrayIdx < 3; dwTrayIdx++) {
            SQWORD sqInput_nn = inputSQWORD();
            for (DWORD dwCupIdx = 0; dwCupIdx < sqInput_nn; dwCupIdx++) {
                SDWORD lInput_Cup = inputSQWORD();
                stCupState.abyPos[lInput_Cup - 1] = dwTrayIdx;
            }
            stCupState.dwDepth = 0;
        }
        for (DWORD dwCupIdx = 0; dwCupIdx < sqInput_n; dwCupIdx++) {
            s_stGoalState1.abyPos[dwCupIdx] = 0;
            s_stGoalState2.abyPos[dwCupIdx] = 2;
        }
        SDWORD lAns = solveBfs(&stCupState, sqInput_n, sqInput_m);

        printf("%d\n", lAns);
    }

    return 0;
}