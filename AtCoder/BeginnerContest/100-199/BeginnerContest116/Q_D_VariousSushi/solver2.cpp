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

int main()
{   
    SQWORD  sqN = inputSQWORD();
    SQWORD  sqK = inputSQWORD();

    vector<SUSHI_ONE_ST> vInput;
    static priority_queue<SUSHI_ONE_ST> vTypeDels[MAX_SUSHI_NUM];
    for (SDWORD lIdx = 0; lIdx < sqN; lIdx++) {
        SUSHI_ONE_ST  stSushi;
        stSushi.lIdx  = lIdx; 
        stSushi.lType = inputSDWORD();
        stSushi.lDel  = inputSDWORD();

        vInput.emplace_back(stSushi); 
        vTypeDels[stSushi.lType - 1].push(stSushi);
    }

    /* 種類別に先頭を取り出す */
    vector<SUSHI_ONE_ST> vTypeTop;
    for (DWORD dwIdx = 0; dwIdx < sqN; dwIdx++) {
        if (0 < vTypeDels[dwIdx].size()) {
            SUSHI_ONE_ST stSushi = vTypeDels[dwIdx].top();
            vTypeDels[dwIdx].pop();
            vTypeTop.emplace_back(stSushi);
        }
    }
    sort(vTypeTop.begin(), vTypeTop.end());
    reverse(vTypeTop.begin(), vTypeTop.end());

    /* 先頭からK個取り出す */
    vector<SUSHI_ONE_ST> vTypeTopSel;
    for (DWORD dwIdx = 0; dwIdx < min(sqK, (SQWORD)(vTypeTop.size())); dwIdx++) {
        SUSHI_ONE_ST s = vTypeTop[dwIdx];
//        printf("idx [%d] type[%d] del[%d]\n", s.lIdx, s.lType, s.lDel);
        vTypeTopSel.emplace_back(s);
    }

    /* 残りの寿司を vector に並べる */
    vector<SUSHI_ONE_ST> vRestSushi;
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(vTypeDels); dwIdx++) {
        while(0 < vTypeDels[dwIdx].size()) {
            SUSHI_ONE_ST stSushi = vTypeDels[dwIdx].top();
            vTypeDels[dwIdx].pop();

            vRestSushi.emplace_back(stSushi);
        }
    }
    sort(vRestSushi.begin(), vRestSushi.end());
    reverse(vRestSushi.begin(), vRestSushi.end());

    /* K個に足りない寿司を選ぶ */
    SQWORD sqRestSelIdx = 0;
    vector<SUSHI_ONE_ST> vDupSelSushi;
    for (SQWORD sqRestIdx = vTypeTopSel.size(); sqRestIdx < sqK; sqRestIdx++)
    {
        vDupSelSushi.emplace_back(vRestSushi[sqRestSelIdx]);
        sqRestSelIdx++;
    }


    /**
     * 入れ替え
     *      [d_i - d_j] - (2x - 1) > 0 なら種類を減らして入れ替える
     */
    SQWORD sqTypeTopIdx = vTypeTopSel.size() - 1;
    while (sqRestSelIdx < vRestSushi.size()) {
        SQWORD sqScore = (vRestSushi[sqRestSelIdx].lDel - vTypeTopSel[sqTypeTopIdx].lDel)
                        - (2 * (vTypeTopSel.size()) - 1);
//        printf("chg : %lld %lld\n", sqRestSelIdx, sqScore);
        if (0 < sqScore) {
            SUSHI_ONE_ST stNewSushi = vRestSushi[sqRestSelIdx];
            vTypeTopSel.erase(vTypeTopSel.begin() + sqTypeTopIdx);
            vDupSelSushi.emplace_back(stNewSushi);
            sqTypeTopIdx--;
            sqRestSelIdx++;
        } else {
            break;
        }
    }

    /* 合計の計算 */
    SQWORD sqBonus = vTypeTopSel.size() * vTypeTopSel.size();

    SQWORD sqDel = 0;
    for (auto s: vTypeTopSel) {
        sqDel += s.lDel;
    }
    for (auto s: vDupSelSushi) {
        sqDel += s.lDel;
    }

    printf("%lld\n", sqBonus + sqDel);
}