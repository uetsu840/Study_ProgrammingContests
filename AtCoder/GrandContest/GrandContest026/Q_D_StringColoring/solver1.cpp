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


void separateString(
    const string &str, 
    SQWORD sqLen, 
    SQWORD sqBitMap,
    string &strR,
    string &strB)
{
    SQWORD sqMask = 1;
    strR.clear();
    strB.clear();
    SQWORD sqPosR = 0;
    SQWORD sqPosB = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqLen; sqIdx++, sqMask<<= 1) {
        if (sqBitMap & sqMask) {
            strR += str[sqIdx];
        } else {
            strB += str[sqIdx];
        }
    }
    cout << "[  separate: " << strR << "-" << strB << "]" << '\n';
}

#define DPTBL_SIZE  (18)

static SQWORD searchString(
    const string &strSecondHalf, 
    const string &strR,
    const string &strB)
{
    /*
    * dp(j)[k] 元の文字列をj文字目まで見て、比較文字列をk文字目まで見たときに一致している組み合わせの数
    * 
    *   dp(j)[k] = sum(i=0 to j-1){dp[i][k-1]} if P[j] = P[k] 
    *               else 0
    */
    SQWORD sqOrgStrLen = strSecondHalf.length();
    SQWORD sqStrRLen = strR.length();
    SQWORD sqStrBLen = strB.length();

    static SQWORD s_asqDpTbl[DPTBL_SIZE];
    static SQWORD s_asqDpTblPrev[DPTBL_SIZE];
    for (SQWORD sqIdx = 0; sqIdx < sqOrgStrLen; sqIdx++) {
        s_asqDpTbl[sqIdx] = 0;
        s_asqDpTblPrev[sqIdx] = 1;
    }

    cout << endl;
    cout << strSecondHalf << "\n";
    cout << strR << "\n";

    for (SQWORD sqStrRIdx = 0; sqStrRIdx < sqStrRLen; sqStrRIdx++) {
        for (SQWORD sqRefStrIdx = 0; sqRefStrIdx < sqOrgStrLen; sqRefStrIdx++) {
            if (sqRefStrIdx < sqStrRIdx) {
                /* 0 */
                s_asqDpTbl[sqRefStrIdx]     = 0;
            } else {
                SQWORD sqStrBCur = sqRefStrIdx - sqStrRIdx;
                if ((strR[sqStrRIdx] == strSecondHalf[sqRefStrIdx]) && (sqStrBCur < sqStrBLen)) {
                    SQWORD sqSum = 0;
                    for (SQWORD sqSumIdx = 0;
                         sqSumIdx < sqRefStrIdx; 
                         sqSumIdx++) {
                    printf("------------------ %lld %lld %lld\n", sqStrBCur, sqStrRIdx, sqSumIdx);
                        SQWORD sqCmpLen = sqRefStrIdx - sqSumIdx - 1;
                        if ((0 <= sqStrBCur - sqCmpLen) && (0 <= sqRefStrIdx - sqCmpLen)) {
                            string strBSubstr = strB.substr(sqStrBCur - sqCmpLen, 
                                                            sqCmpLen);
                    printf("-----------------| %lld\n", sqRefStrIdx - sqCmpLen);
                            string strOrgSubstr = strSecondHalf.substr(sqRefStrIdx - sqCmpLen,
                                                                        sqCmpLen);
                    cout << strBSubstr << "\n";
                    cout << strOrgSubstr << "\n";
                            if (strBSubstr == strOrgSubstr) {
                                sqSum += s_asqDpTblPrev[sqSumIdx];
                            }
                        }
                    }
                    s_asqDpTbl[sqRefStrIdx]     = sqSum;
                } else {
                    s_asqDpTbl[sqRefStrIdx]     = 0;
                }
            }
            printf("dp : [%lld] %lld \n", sqRefStrIdx, s_asqDpTbl[sqRefStrIdx]);
        }
        printf("\n");
        memcpy(s_asqDpTblPrev, s_asqDpTbl, sizeof(s_asqDpTblPrev));
    }

    return 0;
}


int main(void)
{
    string str;
    SQWORD sqInput_N = inputSQWORD();

    cin >> str;

    string strFirstHalf  = str.substr(0, sqInput_N);
    string strSecondHalf = str.substr(sqInput_N, sqInput_N * 2);
    reverse(strSecondHalf.begin(), strSecondHalf.end());

    cout << strFirstHalf  << '\n';
    cout << strSecondHalf << '\n';

    SQWORD sqBitPtnMax = 0x1 << sqInput_N;
    for (SQWORD sqBitMap = 0; sqBitMap < sqBitPtnMax; sqBitMap++) {
        string strR;
        string strB;
        separateString(strFirstHalf, sqInput_N, sqBitMap, strR, strB);
        searchString(strSecondHalf, strR, strB);
    } 

    return 0;
}