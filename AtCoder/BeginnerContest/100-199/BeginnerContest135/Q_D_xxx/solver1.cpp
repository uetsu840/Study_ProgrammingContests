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
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

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
#define MAX_STRING  (100000)

static bool matchDigit3(
    string ref, 
    string b)
{
    for (SDWORD lIdx = 0; lIdx < 3; lIdx++) {
        if (ref[lIdx] != '?') {
            if (ref[lIdx] != b[lIdx]) {
                return false;
            }
        }
    }
    return true;
}


static void countMods(
    string substr, 
    vector<SDWORD> &veclMods)
{
    for (SDWORD lIdx = 0; lIdx < 13; lIdx++) {
        veclMods[lIdx] = 0;
    }
    reverse(substr.begin(), substr.end());

    for (SDWORD lNum = 0; lNum < 1000; lNum ++) {
        char acRefStr[4];
        sprintf(acRefStr, "%03d", lNum);
        if (matchDigit3(substr, acRefStr)) {
            veclMods[lNum % 13] ++;
        }
    }
}


int main(void)
{
    string str;

    cin >> str;
    SQWORD sqNumDigit = str.size();
    reverse(str.begin(), str.end());
    str += string("00000");

    SQWORD sqDigitCur = 0;
    static SDWORD s_alDpTbl[13];
    static SDWORD s_alDpTblNext[13];
    for (SDWORD lDpIdx = 0;; lDpIdx++) {
        char acSepDigit[4];

        string string3 = str.substr(sqDigitCur, 3);
        vector<SDWORD> veclMods(13, 0);
        countMods(string3, veclMods);

        /* update dp */
        if (0 == lDpIdx) {
            for (SDWORD lIdx = 0; lIdx < 13; lIdx++) {
                s_alDpTbl[lIdx] = veclMods[lIdx];
            }
        } else {
            memset(s_alDpTblNext, 0, sizeof(s_alDpTblNext));
            if (0 == lDpIdx % 2) {
                for (SDWORD lTblIdx = 0; lTblIdx < 13; lTblIdx++) {
                    SQWORD sqNextVal = 0;
                    for (SDWORD lNumIdx = 0; lNumIdx < 13; lNumIdx++) {
                        sqNextVal = addMod(sqNextVal, 
                                            mulMod(veclMods[lNumIdx], s_alDpTbl[(13 + lNumIdx - lTblIdx) % 13]));
                    }
                    s_alDpTblNext[lTblIdx] = sqNextVal;
                }
            } else {
                for (SDWORD lTblIdx = 0; lTblIdx < 13; lTblIdx++) {
                    SQWORD sqNextVal = 0;
                    for (SDWORD lNumIdx = 0; lNumIdx < 13; lNumIdx++) {
                        sqNextVal = addMod(sqNextVal,
                                            mulMod(veclMods[lNumIdx], s_alDpTbl[(lNumIdx + lTblIdx) % 13]));
                    }
                    s_alDpTblNext[lTblIdx] = sqNextVal;
                }
            }
            memcpy(s_alDpTbl, s_alDpTblNext, sizeof(s_alDpTbl));
        }

        sqDigitCur += 3;
        if (sqNumDigit < sqDigitCur) {
            break;
        }
    }

    printf("%d\n", s_alDpTbl[5]);

 
    return 0;
}