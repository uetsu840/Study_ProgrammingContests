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

/*----------------------------------------------*/
/**
 *  mod による操作ライブラリ
 */
#define ANS_MOD (1000000007)

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

/*----------------------------------------------*/
/*----------------------------------------------*/

SQWORD pow_sq(SQWORD u, SQWORD t) 
{
    if (!t) return 1;
    SQWORD a = pow_sq(u, t>>1);
    a *= a;
    if (t & 1) a *= u;
    return a;
}
/*----------------------------------------------*/

static vector<SQWORD> vsqPow3Tbl;

static void Normalize(SQWORD &sqL, SQWORD &sqR)
{
    /* normalize */
    for (SQWORD sqIdx = vsqPow3Tbl.size(); 0 <= sqIdx; sqIdx--) {
        if (vsqPow3Tbl[sqIdx] * 2 <= sqL) {
            sqL -= (vsqPow3Tbl[sqIdx] * 2);
            sqR -= (vsqPow3Tbl[sqIdx] * 2);
        }
    }
}

int main(void)
{

    SQWORD sqPow3 = 1;
    for (SQWORD sqIdx = 0; sqIdx < 30; sqIdx++) {
//        printf(">> %lld\n", sqPow3);
        vsqPow3Tbl.emplace_back(sqPow3);
        sqPow3 *= (SQWORD)3;
    }

    /* for debug */
    SQWORD sqQ = inputSQWORD();

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqA = inputSQWORD() - 1;
        SQWORD sqB = inputSQWORD() - 1;
        SQWORD sqC = inputSQWORD() - 1;
        SQWORD sqD = inputSQWORD() - 1;
        if (sqC < sqA) {
            swap(sqA, sqC);
        }
        if (sqD < sqB) {
            swap(sqB, sqD);
        }
        Normalize(sqA, sqC);
        Normalize(sqB, sqD);

        SQWORD sqBoxLevel = 0;
        SQWORD sqAns = -1;
        for (SQWORD sqIdx = vsqPow3Tbl.size(); 0 <= sqIdx; sqIdx--) {
            SQWORD sqBoxL = vsqPow3Tbl[sqIdx];
            SQWORD sqBoxR = vsqPow3Tbl[sqIdx] * 2;
            if ((sqA < sqBoxL) && (sqBoxR <= sqC)) {
                if ((sqBoxL <= sqB) && (sqD < sqBoxR)) {
                    printf("Hit - x[%lld]\n", sqIdx);
                    SQWORD sqPathYLen = min((SQWORD)(sqB - sqBoxL + sqD - sqBoxL + 2),
                                            (SQWORD)(sqBoxR - sqB + sqBoxR - sqD + 2)); 
                    sqAns = (sqC - sqA) + sqPathYLen;
                    sqBoxLevel = sqIdx;
                    break;
                }
            }
            if ((sqB < sqBoxL) && (sqBoxR <= sqD)) {
                if ((sqBoxL <= sqA) && (sqC < sqBoxR)) {
                    SQWORD sqPathXLen = min((SQWORD)(sqA - sqBoxL + sqC - sqBoxL + 2),
                                            (SQWORD)(sqBoxR - sqA + sqBoxR - sqC + 2)); 
                    sqAns = (sqD - sqB) + sqPathXLen;
                    sqBoxLevel = sqIdx;
                    break;
                }
            }
        }
        if (-1 == sqAns) {
            sqAns = (sqC - sqA) + (sqD - sqB);
        }
        printf("%lld\n", sqAns);
    }

    return 0;
}