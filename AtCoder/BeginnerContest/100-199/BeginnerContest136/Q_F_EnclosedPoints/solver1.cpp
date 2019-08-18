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
#define ANS_MOD (998244353LL)
const SQWORD mod = ANS_MOD;
struct mint {
    SQWORD x;
    mint(SQWORD x=0) : x((x % mod + mod) % mod) {}
    mint& operator+=(const mint a) {
        if ((x += a.x) >= mod) x -= mod;
        return *this;
    }
    mint& operator-= (const mint a) {
        x += (mod - a.x);
        if (x >= mod) {
            x -= mod;
        }
        return *this;
    }
    mint& operator*= (const mint a) {
        (x *= a.x);
        x %= mod;
        return *this;
    }
    mint operator+ (const mint a) const {
        mint res(*this);
        return (res+=a);
    }
    mint operator-(const mint a) const {
        mint res(*this);
        return (res-=a);
    }
    mint operator*(const mint a) const {
        mint res(*this);
        return (res*=a);
    }
    mint pow(SQWORD t) const {
        if (0 == t) {
            return 1;
        }
        mint a = pow(t>>1);
        a *= a;
        if (t & 1) {
            a *= *this;
        }
        return a;
    }

    // for prime mod
    mint inv() const {
        return pow(mod-2);
    }
    mint& operator/=(const mint a) {
        return (*this) *= a.inv();
    }
    mint operator/(const mint a) const {
        mint res(*this);
        return res/=a;
    }
};

/*----------------------------------------------*/

/**
 *  BIT
 *      引数のインデックスは 0 ～ N-1、
 *      内部のインデックスは 1 ～ N であることに注意する。
 * 
 */
struct BinaryIndexedTree {
    SDWORD lBitN;
    vector<SQWORD> vecsqBitN; 

    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        vecsqBitN.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < vecsqBitN.size(); dwIdx++) {
            vecsqBitN[dwIdx] = 0;
        }
    }

    SQWORD Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        SQWORD sqSum = 0;
        while (0 < lCur) {
            sqSum += vecsqBitN[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return sqSum;
    }

    void Add(SDWORD lIdx, SQWORD sqX)
    {
        while (lIdx <= lBitN) {
            vecsqBitN[lIdx] += sqX;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(SQWORD sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (sum < Sum(sqMid));
            } else {
                bJudge = (sum <= Sum(sqMid));
            }

            if (bJudge) {
                sqUb = sqMid;
            } else {
                sqLb = sqMid;
            }
        }
        return sqUb;
    }


    /**
    *  累積和が指定した数より大きくなるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumUpperBound(SQWORD sum) 
    {
        return binSearchExec(sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(SQWORD sum) 
    {
        return binSearchExec(sum, false);
    }

    SDWORD End() {
        return lBitN;
    }
};

/*----------------------------------------------*/

struct POINT {
    SQWORD sqX;
    SQWORD sqY;
    SQWORD sqIdx;

    POINT (SQWORD x, SQWORD y, SQWORD i) : sqX(x), sqY(y), sqIdx(i) {};
};

bool isLessPntX(const POINT a, const POINT b)
{
    return a.sqX < b.sqX;
}

bool isLessPntY(const POINT a, const POINT b)
{
    return a.sqY < b.sqY;
}
enum QUADRANT_TYPE {
    QUAD_FIRST,
    QUAD_SECOND,
    QUAD_THIRD,
    QUAD_FORTH,
    QUAD_NUM
};

#define MAX_PNTS    (200000)
static SQWORD s_aasqPntCnt[MAX_PNTS][QUAD_NUM];

static SQWORD calcSinglePoint(
    SQWORD sqFirst,
    SQWORD sqSecond,
    SQWORD sqThird,
    SQWORD sqForth)
{
    mint ans1 = (mint(2).pow(sqFirst)  - 1);
    mint ans2 = (mint(2).pow(sqSecond) - 1);
    mint ans3 = (mint(2).pow(sqThird)  - 1);
    mint ans4 = (mint(2).pow(sqForth)  - 1);
    mint ans;

    ans = mint(1)  
            + ans1 
            + ans2
            + ans3
            + ans4
            + ans1 * ans2
            + ans1 * ans3 * mint(2)
            + ans1 * ans4
            + ans2 * ans3
            + ans2 * ans4 * mint(2)
            + ans3 * ans4
            + ans1 * ans2 * ans3 * mint(2)
            + ans1 * ans2 * ans4 * mint(2)
            + ans1 * ans3 * ans4 * mint(2)
            + ans2 * ans3 * ans4 * mint(2)
            + ans1 * ans2 * ans3 * ans4 * mint(2); 
//    printf("%lld %lld %lld %lld:: %d\n", sqFirst, sqSecond, sqThird, sqForth, ans.x);

    return (SQWORD)(ans.x);
}

int main(void)
{
    SQWORD sqInput_N = inputSQWORD();
    vector<POINT> vPnts;

    /* 入力 */
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();

        vPnts.emplace_back(sqX, sqY, sqIdx);
    }
    /* 座標圧縮とソート */
    sort(vPnts.begin(), vPnts.end(), isLessPntY);
    for (SQWORD sqOrder = 1; sqOrder <= sqInput_N; sqOrder++) {
        vPnts[sqOrder - 1].sqY = sqOrder;
    }
    sort(vPnts.begin(), vPnts.end(), isLessPntX);
    for (SQWORD sqOrder = 1; sqOrder <= sqInput_N; sqOrder++) {
        vPnts[sqOrder - 1].sqX = sqOrder;
    }

    /* X座標を昇順、Y座標をカウントしながら第2象限、第3象限を分ける */
    {
        BinaryIndexedTree bit_r(sqInput_N);
        SQWORD sqPntCnt = 0;
        for (auto it = vPnts.begin(); it != vPnts.end(); ++it) {
            SQWORD sq3rdQuadCnt = bit_r.Sum(it->sqY);
            SQWORD sq2ndQuadCnt = sqPntCnt - sq3rdQuadCnt;

            s_aasqPntCnt[it->sqIdx][QUAD_SECOND] = sq2ndQuadCnt;
            s_aasqPntCnt[it->sqIdx][QUAD_THIRD]  = sq3rdQuadCnt;

            bit_r.Add(it->sqY, 1);
            sqPntCnt++;
        }
    }

    /* X座標を降順、Y座標をカウントしながら第2象限、第3象限を分ける */
    {
        BinaryIndexedTree bit_l(sqInput_N);
        SQWORD sqPntCnt = 0;
        for (auto it = vPnts.rbegin(); it != vPnts.rend(); ++it) {
            SQWORD sq4thQuadCnt = bit_l.Sum(it->sqY);
            SQWORD sq1stQuadCnt = sqPntCnt - sq4thQuadCnt;

            s_aasqPntCnt[it->sqIdx][QUAD_FIRST] = sq1stQuadCnt;
            s_aasqPntCnt[it->sqIdx][QUAD_FORTH] = sq4thQuadCnt;

            bit_l.Add(it->sqY, 1);
            sqPntCnt++;
        }
    }

    mint ans = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD *psqSingle = s_aasqPntCnt[sqIdx];
        ans += mint(calcSinglePoint(psqSingle[QUAD_FIRST],
                                     psqSingle[QUAD_SECOND],
                                     psqSingle[QUAD_THIRD],
                                     psqSingle[QUAD_FORTH]));
    }

    printf("%lld\n", ans.x);

    return 0;
}