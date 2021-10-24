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

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

static const DOUBLE myPI  = 3.14159265358979;

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

/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
#define MAX_LENGTH  (8)
static SQWORD s_aasqCombination[MAX_LENGTH + 1][MAX_LENGTH + 1];

/**
 *  nCm を計算する
 */
static void prepareComb()
{
    s_aasqCombination[0][0] = 1;
    for (SQWORD sqM = 1; sqM <= MAX_LENGTH; sqM++) {
        SQWORD sqComb = sqM;
        for (SQWORD sqN = 0; sqN <= sqM; sqN++) {
            if (0 == sqN) {
                s_aasqCombination[sqM][sqN] = 1;            
            } else {
                s_aasqCombination[sqM][sqN] = sqComb;
                sqComb *= (sqM - sqN);
                sqComb /= (sqN + 1);
            }
        }
    }
}


#define NUM_ALPHABETS   (26)
#define MAX_N           (8)

int main()
{
    string strS;

    cin >> strS;
    static SQWORD s_asqCnts[NUM_ALPHABETS];
    SQWORD sqK = inputSQWORD();

    prepareComb();

    for (auto c: strS) {
        SQWORD sqIdx = c - 'a';
        s_asqCnts[sqIdx]++;
    }

    static SQWORD s_asqCharToIdx[NUM_ALPHABETS];
    static char s_asqIdxToChar[MAX_N];
    vector<SQWORD> vsqCnt(MAX_N);

    SQWORD sqCurIdx = 0;
    for (SQWORD sqCharIdx = 0; sqCharIdx < ArrayLength(s_asqCnts); sqCharIdx++) {
        if (0 < s_asqCnts[sqCharIdx]) {
            vsqCnt[sqCurIdx] = s_asqCnts[sqCharIdx];
            s_asqCharToIdx[sqCharIdx] = sqCurIdx;
            s_asqIdxToChar[sqCurIdx] = sqCharIdx;
            sqCurIdx++;
        }
    }
    SQWORD sqCharTypes = sqCurIdx;

    SQWORD sqStrLen = strS.size();
    SQWORD sqPatternBase = 0;

    string strAns;

    for (SQWORD sqPosIdx = 0; sqPosIdx < strS.size(); sqPosIdx++) {
//        printf("pos: %lld pat: %lld\n", sqPosIdx, sqPatternBase);
        for (SQWORD sqCharSel = 0; sqCharSel < sqCharTypes; sqCharSel++) {
            if (0 < vsqCnt[sqCharSel]) {
                SQWORD sqRestCnt = sqStrLen - sqPosIdx - 1;
                SQWORD sqPatternNum = 1;
                for (SQWORD sqRestCharIdx = 0; sqRestCharIdx < vsqCnt.size(); sqRestCharIdx++) {
                    SQWORD sqRestCharCnt = vsqCnt[sqRestCharIdx];
                    if (sqCharSel == sqRestCharIdx) {
                        sqRestCharCnt--;
                    }
                    if ((0 < sqRestCharCnt) && (0 < sqRestCnt)) {
                        sqPatternNum *= s_aasqCombination[sqRestCnt][sqRestCharCnt];
                        sqRestCnt -= sqRestCharCnt;
                    }
                }
                
//                printf("   char %lld, K: %lld  base:%lld num:%lld\n", sqCharSel, sqK, sqPatternBase, sqPatternNum);
                if (sqK <= sqPatternBase + sqPatternNum) {
                    strAns += s_asqIdxToChar[sqCharSel] + 'a';
                    vsqCnt[sqCharSel]--;
                    break;
                }
                sqPatternBase += sqPatternNum;
            }
        }
    }
    printf("%s\n", strAns.c_str());
}

