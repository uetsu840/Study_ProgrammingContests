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
/**
 *  rolling hash 
 */

#define MS  (2)
const long long mod[] = {999999937LL, 1000000007LL}, base = 9973;
struct hash_val {
    SQWORD asqHash[MS];

    const bool operator== (const hash_val &v) {
        return 0 == memcmp(this, &v, sizeof(*this));
    };
};

struct rolling_hash {
    int n;
    vector<long long> hs[MS], pw[MS];
    rolling_hash(){}
    rolling_hash(const string &s) {
        n = s.size();
        for (int i = 0; i < MS; i++) {
            hs[i].assign(n+1,0);
            pw[i].assign(n+1,0);
            hs[i][0] = 0;
            pw[i][0] = 1;
            for (int j = 0; j < n; j++) {
                pw[i][j+1] = pw[i][j]*base%mod[i];
                hs[i][j+1] = (hs[i][j]*base+s[j])%mod[i];
            }
        }
    }
    hash_val hash(int l, int r) { 
        hash_val ret;
        for (int i = 0; i < MS; i++) {
            ret.asqHash[i] = ((hs[i][r]-hs[i][l]*pw[i][r-l])%mod[i]+mod[i])%mod[i]; 
        }
        return ret;
    }

    bool match(int l1, int r1, int l2, int r2) {
        bool ret = false;
        hash_val h1 = hash(l1,r1);
        hash_val h2 = hash(l2,r2);

        return (h1 == h2);
    }

    bool match(int l, int r, hash_val h) {
        bool ret = 1;
        return (h == hash(l,r));
    }
};

/*----------------------------------------------*/

typedef struct {
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    DWORD   dwSize;

    void initUnionFind(
        DWORD dwSize)
    {
        dwSize = dwSize;
        vdwPar.resize(dwSize);
        vdwRank.resize(dwSize);
        vdwCnt.resize(dwSize);
    
        for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vdwPar[dwIdx]  = dwIdx;
            vdwRank[dwIdx] = 0;
            vdwCnt[dwIdx]  = 1;
        }
    }

    DWORD ufGetCnt(DWORD sqIdx) {
        return vdwCnt[ufGetParent(sqIdx)];
    }


    DWORD ufGetParent(DWORD dwIdx) const
    {
        return vdwPar[dwIdx];
    }

    DWORD ufGetRank(DWORD dwIdx) const
    {
        return vdwRank[dwIdx];
    }

    void ufSetParent(DWORD dwIdx, DWORD dwParent)
    {
        vdwPar[dwIdx] = dwParent; 
        if (ufGetRank(dwIdx) == ufGetRank(dwParent)) {
            (vdwRank[dwParent])++;
        }
    }

    DWORD ufGetRoot(DWORD dwIdx) const
    {
        if (ufGetParent(dwIdx) == dwIdx) {
            return dwIdx;
        } else {
            DWORD dwParent = ufGetParent(dwIdx);
            DWORD dwRoot = ufGetRoot(dwParent);
            return dwRoot;
        }
    }

    bool ufUnite(DWORD dwX, DWORD dwY)
    {
        DWORD dwRootX = ufGetRoot(dwX);
        DWORD dwRootY = ufGetRoot(dwY);

        if (dwRootX == dwRootY) {
            return false;
        }

        if (ufGetRank(dwRootX) < ufGetRank(dwRootY)) {
            ufSetParent(dwRootX, dwRootY);
            (vdwCnt[dwRootY]) += (vdwCnt[dwRootX]);
        } else {
            ufSetParent(dwRootY, dwRootX);
            (vdwCnt[dwRootX]) += (vdwCnt[dwRootY]);
        }

        return true;
    }

    bool ufIsSame(DWORD dwX, DWORD dwY) const
    {
        return (ufGetRoot(dwX)  == ufGetRoot(dwY));
    }
} ST_UNION_FIND;

/*----------------------------------------------*/

static void getStrMatchPos(
    string s, 
    string pat, 
    vector<SQWORD> &vsqRet)
{
    rolling_hash hash_str(s);
    rolling_hash hash_pat(pat);
    SQWORD size_str = s.size();
    SQWORD size_pat = pat.size();
    hash_val pattern_hash_val;
    pattern_hash_val = hash_pat.hash(0, size_pat);

    for (SQWORD i = 0; i < size_str - size_pat; i++) {
        if (hash_str.hash(i, i+size_pat) == pattern_hash_val) {
            vsqRet.emplace_back(i);
        } 
    }
}


int main(void)
{

    string str_s, str_t;

    cin >> str_s;
    cin >> str_t;

    SQWORD sqSLen = str_s.size();
    SQWORD sqTLen = str_t.size();

    string str_s_plus = str_s + str_s + str_s;  /* 最低3個は繰り返す */
    while (str_s_plus.size() < str_t.size() * 2) {
        str_s_plus += str_s;
    }

    vector<SQWORD> vecMatchPos;
    getStrMatchPos(str_s_plus, str_t, vecMatchPos);

    ST_UNION_FIND Uf;
    Uf.initUnionFind(sqSLen);
    
    bool bIsForever = false;
    for (auto pos: vecMatchPos) {
        if (pos < str_s.size()) {
            bool bFound = std::binary_search(vecMatchPos.begin(), vecMatchPos.end(), (pos + sqTLen) % sqSLen);
            if (bFound) {
                if (!Uf.ufUnite(pos, (pos + sqTLen) % sqSLen)) {
                    bIsForever = true;
                    break;
                }
            }
        }
    }
    if (bIsForever) {
        printf("-1\n");
    } else {
        SQWORD sqSizeMax = 0;
        for (SDWORD lIdx = 0; lIdx < sqSLen; lIdx++) {
            if (std::binary_search(vecMatchPos.begin(), vecMatchPos.end(), lIdx)) {
                sqSizeMax = max(sqSizeMax, (SQWORD)Uf.ufGetCnt(lIdx));
            }
        }
        printf("%lld\n", sqSizeMax);
    }

    return 0;
}