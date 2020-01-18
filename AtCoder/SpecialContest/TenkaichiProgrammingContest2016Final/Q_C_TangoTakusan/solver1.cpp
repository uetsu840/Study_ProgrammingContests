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

/**
 * KMP法 
 */
class KMP {
public:
    vector<SDWORD> makeTable(const string& s) {
        SDWORD n = s.size();
        vector<SDWORD> ret(n+1);
        ret[0] = -1;
        SDWORD j = -1;
        for (SDWORD i = 0; i < n; i++) {
            while ((0 <= j) && (s[i] != s[j])) {
                j = ret[j];
            } 
            j++;

            /* KMP法にするコード */
            if (s[i+1] == s[j]) {
                ret[i+1] = ret[j];
            } else {
                ret[i+1] = j;
            }
        }
        return ret;
    }

    /**
     *  str の中に word とマッチする場所のリストを返す
     * ret のそれぞれの要素 el は, 「str[el] からの文字列が word と一致する」ことを示す
     */
    vector<SDWORD> wordSearch(const string& str, const string& word) {
        vector<SDWORD> table = makeTable(word), ret;

        SDWORD m = word.size();
        SDWORD i = 0;
        SDWORD j = 0;
        SDWORD n = str.size();
        while (j < n) {
            while ((-1 < i) && (word[i]) != str[j]) {
                i = table[i];
            }
            i++;
            j++;
            if (m <= i) {
                ret.push_back(j - i);
                i = table[i];
            }
        }
        return ret;
    }
};

#define MAX_STR_LEN (200000)

int main(void)
{
    string strS;
    cin >> strS;

    SQWORD sqM = inputSQWORD();

    vector<vector<SQWORD>> vvMatchPat(strS.size());
    vector<SQWORD> vStrSize(sqM ,0);
    vector<SQWORD> vStrWeight(sqM, 0);

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        string strP;
        cin >> strP;

        vStrSize[sqIdx] = strP.size();

        KMP kmp;
        vector<SDWORD> vMatch;
        vMatch = kmp.wordSearch(strS, strP);

        for (auto pos: vMatch) {
            vvMatchPat[pos].emplace_back(sqIdx);
        }
    }

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqWeight = inputSQWORD();
        vStrWeight[sqIdx] = sqWeight;
    }

    static SQWORD s_asqDp[MAX_STR_LEN + 1];

    for (SQWORD sqIdx = 0; sqIdx < strS.size(); sqIdx++) {
        s_asqDp[sqIdx + 1] = max(s_asqDp[sqIdx + 1], s_asqDp[sqIdx]);
        for (auto pat: vvMatchPat[sqIdx]) {
            SQWORD sqLen = vStrSize[pat];
            SQWORD sqWeight = vStrWeight[pat];
            s_asqDp[sqIdx + sqLen] = max(s_asqDp[sqIdx + sqLen], s_asqDp[sqIdx] + sqWeight);
//            printf(">> <%lld %lld> %lld %lld ", sqIdx, sqIdx + sqLen, s_asqDp[sqIdx], s_asqDp[sqIdx + sqLen]);
        }
    }

    SQWORD sqAns = 0;
    for (SQWORD sqIdx = 0; sqIdx <= strS.size(); sqIdx++) {
        sqAns = max(sqAns, s_asqDp[sqIdx]);
    }

    printf("%lld\n", sqAns);


    return 0;
}