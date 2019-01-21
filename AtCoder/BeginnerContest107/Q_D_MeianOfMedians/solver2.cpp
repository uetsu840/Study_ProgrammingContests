#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using BYTE = unsigned char;
using SBYTE = char;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0xFFFFFFFFFFFFFFFF)

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

#define MAX_ARY_NUM     (100000)
static DWORD s_adwInputAry[MAX_ARY_NUM];
static DWORD s_dwInputAryLen;

/**
 *  BIT
 *      引数のインデックスは 0 ～ N-1、
 *      内部のインデックスは 1 ～ N であることに注意する。
 * 
 */
#define MAX_BIT_NUM (MAX_ARY_NUM + 1)
static SDWORD s_alBit[MAX_BIT_NUM + 1];
static SDWORD s_lBitN;

static void bitInit(SDWORD lNum)
{
    s_lBitN = lNum;
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_alBit); dwIdx++) {
        s_alBit[dwIdx] = 0;
    }
}

static SDWORD bitSum(SDWORD lIdx)
{
    SDWORD lSum = 0;
    while (0 < lIdx) {
        lSum += s_alBit[lIdx];
        lIdx -= (lIdx & (-lIdx));     /* 最後の1ビット */
    }
    return lSum;
}

static SDWORD bitAdd(SDWORD lIdx, SDWORD lX)
{
    while (lIdx <= s_lBitN) {
        s_alBit[lIdx] += lX;
        lIdx += (lIdx & (-lIdx));
    }
}

/**
*   判定
*       true:   上側に超過
*       false:  下側に超過
*/
static bool isMedianOfMedians(SQWORD sqTryIdx, vector<SDWORD> &vVals, vector<SDWORD> &vSortedVals)
{
    SDWORD alBinaryAry[MAX_ARY_NUM+1];
    SDWORD alCumSum[MAX_ARY_NUM+1];

    if (s_dwInputAryLen <= sqTryIdx) {
        return true;
    }
    SQWORD sqVal = vSortedVals[sqTryIdx];

    /* 指定した値より大きいか小さいかで ±1 に置換 */
    for (DWORD dwIdx = 0; dwIdx < s_dwInputAryLen; dwIdx++) {
        if ((DWORD)sqVal <= vVals[dwIdx]) {
            alBinaryAry[dwIdx] = 1;
        } else {
            alBinaryAry[dwIdx] = -1;
        }
    }

    /* 累積和 */
    SDWORD lCumSum = 0;
    for (DWORD dwIdx = 0; dwIdx <= s_dwInputAryLen; dwIdx++) {
        alCumSum[dwIdx] = lCumSum;
        lCumSum += alBinaryAry[dwIdx];
    }

    /* 値の範囲を 0-nにするため、要素の最大と最小を求め、オフセットを計算する。 */
    vector<SDWORD> vSortedCumSum(alCumSum, alCumSum + (s_dwInputAryLen + 1));
    sort(vSortedCumSum.begin(), vSortedCumSum.end());
    SDWORD lMin = *(vSortedCumSum.begin());
    SDWORD lMax = *(vSortedCumSum.end() - 1);
    SDWORD lOfs = 0 - lMin + 1;
    DWORD dwCumSumLen = lMax - lMin + 1;
 
    /* 転倒数を求める */
    bitInit(dwCumSumLen);
    SQWORD sqCorssNum = 0;
    for (DWORD dwJ = 0; dwJ < (s_dwInputAryLen + 1); dwJ++) {
        sqCorssNum += dwJ - bitSum(alCumSum[dwJ] + lOfs);
        bitAdd(alCumSum[dwJ] + lOfs, 1);
    }
    QWORD qwPairNum = (QWORD)(s_dwInputAryLen + 1) * (QWORD)s_dwInputAryLen / 2LL;

    if (qwPairNum / 2 < sqCorssNum) {
        return true;
    } else {
        return false;
    }
}

/**
*   2分探索
*/
static SQWORD binarySearch(
    bool (*judge)(SQWORD, vector<SDWORD>&, vector<SDWORD>&), 
    SQWORD sqInitLb, 
    SQWORD sqInitUb, 
    vector<SDWORD> &vVals,
    vector<SDWORD> &vSortedVals)
{
    SQWORD sqLb = sqInitLb;
    SQWORD sqUb = sqInitUb;

    while (1LL < sqUb - sqLb) {
        SQWORD sqMid = (sqUb + sqLb) / 2LL;
        if (judge((QWORD)sqMid, vVals, vSortedVals)) {
            sqUb = sqMid;
        } else {
            sqLb = sqMid;
        }
    }
    return sqUb;
}


/**
 * 以下を行う
 *  ・値を1 -  の ソートされた unique value にする。
 */
static void convertToUniqueValue(
    const vector<SDWORD> &vOrg, 
    vector<SDWORD> &vUniqueVals,
    vector<SDWORD> &vSortedUniqueVals,
    SDWORD  lUniqueStartVal,
    unordered_map<SDWORD, SDWORD> &mMapFwd,
    unordered_map<SDWORD, SDWORD> &mMapRev)
{
    DWORD dwSize = vOrg.size();
    vector<SDWORD> vSortedVals;
    DWORD adwUniqueValAry[MAX_ARY_NUM];

    /* 値を 1-N の unique_value にする */
    vSortedVals.assign(vOrg.begin(), vOrg.end());
    sort(vSortedVals.begin(), vSortedVals.end());

    DWORD dwPrevVal = MAX_DWORD;
    DWORD dwUniqueVal = 1;
    for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
        if (dwPrevVal != vSortedVals[dwIdx]) {
            mMapFwd[vSortedVals[dwIdx]] = dwUniqueVal;
            mMapRev[dwUniqueVal] = vSortedVals[dwIdx];
            dwUniqueVal++;     
        }
        dwPrevVal = vSortedVals[dwIdx];
    }
    for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
        adwUniqueValAry[dwIdx] = mMapFwd[vOrg[dwIdx]];
    }
    vUniqueVals.assign(adwUniqueValAry, adwUniqueValAry + dwSize);

    vSortedUniqueVals.assign(adwUniqueValAry, adwUniqueValAry + dwSize);
    sort(vSortedUniqueVals.begin(), vSortedUniqueVals.end());
}

int main()
{
    DWORD dwInput_N;
    DWORD dwAnswer;
    unordered_map<SDWORD, SDWORD> mInputIdxVal;
    unordered_map<SDWORD, SDWORD> mInputIdxValRev;
    vector<SDWORD> vInput;
    vector<SDWORD> vUniqueVals;
    vector<SDWORD> vSortedUniqueVal;

    /* 入力 */
    dwInput_N = inputSDWORD();
    s_dwInputAryLen = dwInput_N;

    for(DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        DWORD dwVal = inputSDWORD();
        s_adwInputAry[dwIdx] = dwVal;
    }
    vInput.assign(s_adwInputAry, s_adwInputAry + dwInput_N);

    /* 計算の過程で数値は意味がないので、unique_value に変換する。(マップは保持) */
    convertToUniqueValue(vInput, vUniqueVals, vSortedUniqueVal, 1, mInputIdxVal, mInputIdxValRev);

    /* 2分探索 */
    DWORD dwBound = binarySearch(isMedianOfMedians, -1, MAX_DWORD, vUniqueVals, vSortedUniqueVal) - 1;

    /* 計算結果のインデックスを idx -> unique_val -> value の順に復元 */
    DWORD dwBoundUniqueVal = vSortedUniqueVal[dwBound];
    dwAnswer = mInputIdxValRev[dwBoundUniqueVal];
    printf("%d\n", dwAnswer);

    return 0;
}
