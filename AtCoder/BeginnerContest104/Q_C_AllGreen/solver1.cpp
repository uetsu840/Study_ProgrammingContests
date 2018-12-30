#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MAX_DWORD  (0xFFFFFFFF)
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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

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

static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber;
            }
        }
    }
}

/**
 *  DWORD 配列のprintf
 */
static void printDwordArray(DWORD *pdwData, DWORD dwNum)
{
    DWORD dwIdx;
    printf("[%d] ----[", dwNum);
    for (dwIdx = 0; dwIdx < dwNum; dwIdx++) {
        printf("%d ", *(pdwData + dwIdx));
    }
    printf("]\n");
}


#define MAX_QUESTION    (10)
#define MAX_PI          (100)
#define MAX_BONUS       (10000)
static DWORD s_adwNumQuestion[MAX_QUESTION];
static DWORD s_adwBonus[MAX_QUESTION];
static DWORD s_dwInput_D;
static DWORD s_dwInput_Goal;

#define MAX_SCORE       MAX_QUESTION * (MAX_QUESTION + 1) * MAX_PI / 2 + MAX_BONUS * MAX_PI
static DWORD s_adwDpTblA[MAX_SCORE + 1];
static DWORD s_adwDpTblB[MAX_SCORE + 1];


static inline DWORD getMinSolveCount(const DWORD *pdwDpPrev, DWORD dwScore, DWORD dwQuestionIdx)
{
    DWORD dwQuestionNum = s_adwNumQuestion[dwQuestionIdx];
    DWORD dwMinSolveCnt = MAX_DWORD;

    for (DWORD dwSlvCnt = 0; dwSlvCnt <= dwQuestionNum; dwSlvCnt++) {
        DWORD dwCurScore = dwSlvCnt * (dwQuestionIdx + 1);
        if (dwQuestionNum == dwSlvCnt) {
            dwCurScore += s_adwBonus[dwQuestionIdx];
        }
        if (dwCurScore <= dwScore) {
            DWORD dwSlvSum = *(pdwDpPrev + (dwScore - dwCurScore)) + dwSlvCnt;
            dwMinSolveCnt = MIN(dwMinSolveCnt, dwSlvSum);
        }
    }
    return dwMinSolveCnt;
}

static inline void updateDp(DWORD *pdwDpSrc, DWORD *pdwDpDst, DWORD dwQuestionIdx)
{
    for (DWORD dwScore = 0; dwScore < ArrayLength(s_adwDpTblA); dwScore++) {
        *(pdwDpDst + dwScore) = getMinSolveCount(pdwDpSrc, dwScore, dwQuestionIdx);
    }
}

int main()
{
    DWORD dwIdx;
    
    s_dwInput_D = inputSDWORD();
    s_dwInput_Goal = inputSDWORD() / 100;
//    printf("D = %d, G = %d\n", s_dwInput_D, s_dwInput_Goal);
    for (dwIdx = 0; dwIdx < s_dwInput_D; dwIdx++) {
        DWORD dwNumQuestion = inputSDWORD();
        DWORD dwBonus = inputSDWORD() / 100;
//        printf("num: %d, bonus: %d\n", dwNumQuestion, dwBonus);
        s_adwNumQuestion[dwIdx] = dwNumQuestion;
        s_adwBonus[dwIdx] = dwBonus;
    }

    /* Initialize */
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_adwDpTblA); dwIdx++) {
        s_adwDpTblA[dwIdx] = MAX_SDWORD;
        s_adwDpTblB[dwIdx] = MAX_SDWORD;
    }
    s_adwDpTblA[0] = 0;

    /* Dp */
    DWORD *pdwDpSrc, *pdwDpDst;
    for (DWORD dwQuestionIdx = 0; dwQuestionIdx < s_dwInput_D; dwQuestionIdx++) {
        if (dwQuestionIdx % 2 == 0) {
            pdwDpSrc = s_adwDpTblA;
            pdwDpDst = s_adwDpTblB;
        } else {
            pdwDpSrc = s_adwDpTblB;
            pdwDpDst = s_adwDpTblA;
        }
        updateDp(pdwDpSrc, pdwDpDst, dwQuestionIdx);
    }

    /* Get Answer */
    /**
     *  answer is minimum value of the right side of dp table
     */
    DWORD dwAnswer = MAX_SDWORD;
    for (dwIdx = s_dwInput_Goal; dwIdx < ArrayLength(s_adwDpTblA); dwIdx++) {
        dwAnswer = MIN(dwAnswer, *(pdwDpDst + dwIdx));       
    }
    printf("%d\n", dwAnswer);

    return 0;
}
