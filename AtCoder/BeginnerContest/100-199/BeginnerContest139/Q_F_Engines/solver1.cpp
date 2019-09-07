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
#define ANS_MOD (1000000007)
SQWORD MODINT::MOD = ANS_MOD;

/*----------------------------------------------*/

class Combination {
    vector<MODINT> vecmsqComb;
public:
    Combination(SQWORD sqN) {
        /* nCjを事前計算する */
        vecmsqComb.resize(sqN + 1);

        MODINT sqComb((SQWORD)1);
        vecmsqComb[0] = sqComb;
        for (SQWORD sqJ = 1; sqJ <= sqN; sqJ++) {
            sqComb *= MODINT(sqN - sqJ + 1);
            sqComb /= MODINT(sqJ);
            vecmsqComb[sqJ] = sqComb;
        }
    }

    MODINT GetVal(SQWORD sqJ) 
    {
        return vecmsqComb[sqJ];
    }
};

/*----------------------------------------------*/

int main(void)
{
    SQWORD sqL = inputSQWORD();
    SQWORD sqR = inputSQWORD();

    /**
     *  L <= x <= y <= R
     *  x と y の最上位ビットは同じ場所
     *  x & y = x
     *  (x, y) = (1, 1), (0, 1), (0, 0)
     */
    /**
     *  dp[i][j][k][l] 
     *      i: i桁目
     *      j: 0: L == x  / 1: L < x
     *      k: 0: y == R  / 1: y < R
     *      l: 最上位ビットを踏んだ
     */
    static MODINT s_amsqDp[64][2][2][2];

    s_amsqDp[63][0][0][0] = 1;
    for (SDWORD lBit = 62; 0 <= lBit; lBit--) {
        SDWORD lLb = (sqL >> lBit) & 1;
        SQWORD lRb = (sqR >> lBit) & 1;
        for (SDWORD lJ = 0; lJ <= 1; lJ++)
        for (SDWORD lK = 0; lK <= 1; lK++)
        for (SDWORD lMsb = 0; lMsb <= 1; lMsb++) {
            MODINT msqPreDp = s_amsqDp[lBit+1][lJ][lK][lMsb];
            for (SDWORD lX = 0; lX <= 1; lX++)
            for (SDWORD lY = 0; lY <= 1; lY++) {
                if ((lX == 1) & (lY == 0)) {
                    continue;
                }
                SDWORD lNextJ = lJ;
                SDWORD lNextK = lK;
                SDWORD lNextMsb = lMsb;

                if (!lMsb && (lX != lY)) {
                    continue;
                }
                if (lX && lY) {
                    lNextMsb = 1;
                }
                if ((lJ == 0) && (lX == 0) && (lLb == 1)) {
                    continue;
                }
                if ((lX == 1) && (lLb == 0)) {
                    lNextJ = 1;
                }


                if ((lK == 0) && (lY == 1) && (lRb == 0)) {
                    continue;
                }
                if ((lY == 0) && (lRb == 1)) {
                    lNextK = 1;
                }

                s_amsqDp[lBit][lNextJ][lNextK][lNextMsb] += msqPreDp;
            }
        }
    }
    MODINT sqAns = 0;
    for (SDWORD lJ = 0; lJ <= 1; lJ++)
    for (SDWORD lK = 0; lK <= 1; lK++)
    for (SDWORD lMsb = 0; lMsb <= 1; lMsb++) {
        sqAns += s_amsqDp[0][lJ][lK][lMsb];
    }
    printf("%lld\n", sqAns);

    return 0;
}