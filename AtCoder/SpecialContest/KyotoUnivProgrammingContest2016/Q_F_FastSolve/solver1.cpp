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

struct RANGE_ST {
    SQWORD sqVal;
    SQWORD sqParseLen;
};

enum COMP_DIR_TYPE {
    COMP_DIR_GREAT,
    COMP_DIR_SMALL,
    COMP_DIR_NONE
};

static RANGE_ST parseNumber(
    const string    str,
    COMP_DIR_TYPE   eDir,
    SQWORD          sqBorder)
{
    SQWORD sqNum = 0;
    RANGE_ST ret;

    sqNum = atoi(str.c_str());
    /* 右辺値の場合には、borderとdirを考慮して必要な分だけ読む */
    if (COMP_DIR_GREAT == eDir) {
        /* ^ → 2桁よまないとわからない */
        ret.sqVal = sqNum;
        ret.sqParseLen = strlen(str.c_str());
    } else {
        /* _ → 1桁目が大きい時点で判定 */
        if (sqBorder <= 9) {
            if (sqBorder <= (str[0] - '0')) {
                ret.sqVal = sqBorder;
                ret.sqParseLen = 1;
            } else {
                ret.sqVal = sqNum;
                ret.sqParseLen = strlen(str.c_str());
            }
        } else {
            ret.sqVal = sqNum;
            ret.sqParseLen = strlen(str.c_str());
        }
    }


//    printf("num >%lld\n", sqNum);

    return ret;
}


static void separateLR(
    const string str,
    string &lhs,
    string &rhs)
{
    SQWORD sqDepth = 0;
    SQWORD sqLen = str.size();
    for (SQWORD sqIdx = 2; sqIdx < str.size(); sqIdx++) {
        if (str[sqIdx] == '(') {
            sqDepth++;
        } else if (str[sqIdx] == ')') {
            sqDepth--;
        }
        if (0 == sqDepth) {
            if (str[sqIdx] == ',') {
                lhs = str.substr(2, sqIdx - 2);
                rhs = str.substr(sqIdx + 1, sqLen - sqIdx - 2);

                printf("L: %s R: %s\n", lhs.c_str(), rhs.c_str());
                return;
            }
        }
    }
}

static RANGE_ST parseExpression(
    const string    str,
    COMP_DIR_TYPE   eDir,
    SQWORD          sqBorder)
{
    RANGE_ST ret;
    switch (str[0]) {
    case '_':
        {
            string lhs, rhs;
            RANGE_ST stR, stL;
            separateLR(str, lhs, rhs);
            stL = parseExpression(lhs, eDir, sqBorder);
            if (COMP_DIR_SMALL == eDir) {
                stR = parseExpression(rhs, COMP_DIR_SMALL, min(sqBorder, stL.sqVal));
            } else if (COMP_DIR_GREAT == eDir) {

            } else {

            }
        }
        break;
    case '^':
        {
            string lhs, rhs;
            RANGE_ST stR, stL;
            separateLR(str, lhs, rhs);
            stL = parseExpression(lhs);
            stR = parseExpression(rhs);
        }
        break;
    default:
        ret = parseNumber(str);
        break;
    }

    return ret;
}


static void parseStatement(const string str)
{
    string exp = str.substr(0, str.length() - 1);
    parseExpression(exp);
}

static void solve(string strS)
{
    parseStatement(strS);
}

int main(void)
{
    SQWORD sqQ = inputSQWORD();

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        string strS;
        cin >> strS;

        solve(strS);
    }



    return 0;
}