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
using LDOUBLE = long double;
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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

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
/**
 *  mod による操作ライブラリ
 */
#define ANS_MOD (998244353)

class MODINT {
    static SQWORD MOD;
    SQWORD m_x;

public:
    MODINT(SQWORD val) {
        m_x = (val % MOD + MOD) % MOD;
    };
    MODINT() {
        m_x = 0;
    }
    static void Init(SQWORD sqMod) {
        MOD = sqMod;
    }

	MODINT& operator+= (const MODINT a)
    {
        m_x = (m_x + a.m_x) % MOD; 
        return *this;
    };
	MODINT& operator-= (const MODINT a)
    { 
        m_x = (m_x - a.m_x + MOD) % MOD; 
        return *this;
    };
	MODINT& operator*= (const MODINT a)
    {
        m_x = (m_x * a.m_x) % MOD;
        return *this;
    };
    MODINT pow(SQWORD t) const {
        if (!t) return 1;
        MODINT a = pow(t>>1);
        a *= a;
        if (t&1) a *= *this;
        return a;
    }
	MODINT operator+ (const MODINT a) const {
		MODINT res(*this);
		return (res += a);
	}
	MODINT operator- (const MODINT a) const {
		MODINT res(*this);
		return (res -= a);
	}
	MODINT operator* (const MODINT a) const {
		MODINT res(*this);
		return (res *= a);
	}
	MODINT operator/ (const MODINT a) const {
		MODINT res(*this);
		return (res /= a);
	}

    /* 逆元 */
    MODINT inv() const {
        return pow(MOD-2);
    }

    /* 除算 */
    MODINT& operator/=(const MODINT a) {
        return (*this) *= a.inv();
    } 

    /* 整数版 */
	MODINT& operator+= (const SQWORD a) {*this += MODINT(a); return *this;};
	MODINT& operator-= (const SQWORD a) {*this -= MODINT(a); return *this;};
	MODINT& operator*= (const SQWORD a) {*this *= MODINT(a); return *this;};
	MODINT& operator/= (const SQWORD a) {*this /= MODINT(a); return *this;};

    SQWORD getVal() { return m_x; };
};
SQWORD MODINT::MOD = ANS_MOD;

/*-----------------------------------------------------*/

/*----------------------------------------------*/
struct POS_ST {
    SQWORD sqX;
    SQWORD sqY;

    POS_ST(SQWORD x, SQWORD y): sqX(x), sqY(y) {};
};


struct DFS_SEARCH_ST {
    bool aabIsVisited[6][6];
    bool aabMap[6][6];

    DFS_SEARCH_ST (SQWORD sqBitmap) {
        for (SQWORD sqX = 0; sqX < 6; sqX++) {
            for (SQWORD sqY = 0; sqY < 6; sqY++) {
                aabIsVisited[sqX][sqY] = false;
                aabMap[sqX][sqY] = false;
            }
        }

        for (SQWORD sqBit = 0; sqBit <= 15; sqBit++) {
            SQWORD sqX = sqBit / 4 + 1;
            SQWORD sqY = sqBit % 4 + 1;
            if (sqBitmap & (0x1 << sqBit)) {
                aabMap[sqX][sqY] = true;
            }
        }
    }
};

static vector<pair<SQWORD, SQWORD>> vpairSrchDir = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

static SQWORD countBlocks(DFS_SEARCH_ST &stSrch, SQWORD sqCurX, SQWORD sqCurY)
{
    if (stSrch.aabIsVisited[sqCurX][sqCurY]) {
        return 0;
    }
    if (!stSrch.aabMap[sqCurX][sqCurY]) {
        return 0;
    }

    SQWORD sqVisitCount = 1;
    stSrch.aabIsVisited[sqCurX][sqCurY] = true;

    for (auto d: vpairSrchDir) {
        SQWORD sqNextX = sqCurX + d.first;
        SQWORD sqNextY = sqCurY + d.second;
        sqVisitCount += countBlocks(stSrch, sqNextX, sqNextY);
    }
    return sqVisitCount;
}

static SQWORD countOutsides(DFS_SEARCH_ST &stSrch, SQWORD sqCurX, SQWORD sqCurY)
{
    if (stSrch.aabIsVisited[sqCurX][sqCurY]) {
        return 0;
    }
    if (stSrch.aabMap[sqCurX][sqCurY]) {
        return 0;
    }

    SQWORD sqVisitCount = 1;
    stSrch.aabIsVisited[sqCurX][sqCurY] = true;

    for (auto d: vpairSrchDir) {
        SQWORD sqNextX = sqCurX + d.first;
        SQWORD sqNextY = sqCurY + d.second;
        if ((0 <= sqNextX) && (sqNextX <= 5) && (0 <= sqNextY) && (sqNextY <= 5)) {
            sqVisitCount += countOutsides(stSrch, sqNextX, sqNextY);
        }
    }
    return sqVisitCount;
}

static bool testOnePattern(SQWORD sqBitmap, vector<POS_ST> &villages)
{
    DFS_SEARCH_ST stMapInfoIsland(sqBitmap);

    SQWORD sqNumIsland = 0;
    for (SQWORD sqStartX = 1; sqStartX <= 4; sqStartX++) {
        for (SQWORD sqStartY = 1; sqStartY <= 4; sqStartY++) {
            if (1 <= countBlocks(stMapInfoIsland, sqStartX, sqStartY)) {
                sqNumIsland++;
            }
        }
    }
    if (1 != sqNumIsland) {
        return false;
    }

    for (auto v: villages) {
        SQWORD sqX = v.sqX + 1;
        SQWORD sqY = v.sqY + 1;
        if (!stMapInfoIsland.aabMap[sqX][sqY]) {
            return false;
        }
    }

    DFS_SEARCH_ST stMapInfoOutside(sqBitmap);
    SQWORD sqNumSea = 0;
    for (SQWORD sqStartX = 0; sqStartX <= 5; sqStartX++) {
        for (SQWORD sqStartY = 0; sqStartY <= 5; sqStartY++) {
            if (1 <= countOutsides(stMapInfoOutside, sqStartX, sqStartY)) {
                sqNumSea++;
            }
        }
    }
    if (1 != sqNumSea) {
//        printf("%04x\n", sqBitmap);
        return false;
    }

    return true;
}

int main(void)
{
    vector<POS_ST> vstVillages;

    for (SQWORD sqX = 0; sqX < 4; sqX++) {
        for (SQWORD sqY = 0; sqY < 4; sqY++) {
            SQWORD sqVal = inputSQWORD();
            if (1 == sqVal) {
                vstVillages.emplace_back(sqX, sqY);
            }
        }
    }

    SQWORD sqAns = 0;
    for (SQWORD sqBitmap = 0; sqBitmap <= 65535; sqBitmap++) {
        if (testOnePattern(sqBitmap, vstVillages)) {
            sqAns++;
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}
