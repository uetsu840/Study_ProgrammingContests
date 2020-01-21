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
struct PARAMS {
    string strS;
    SQWORD sqN;
    SQWORD sqSingleLen;
};


static bool compStr(
    const string &str1,
    const string &str2)
{
    if (str2.size() < str1.size()) {
        return true;
    }
    if (str2.size() > str1.size()) {
        return false;
    }

    for (SQWORD sqIdx = 0; sqIdx < str1.size(); sqIdx++) {
        if (str2[sqIdx] < str1[sqIdx]) {
            return true;
        }
        if (str2[sqIdx] > str1[sqIdx]) {
            return false;
        }
    }
    return true;
}

static string AddStr(
    const string &str1,
    const string &str2)
{
    SQWORD sqLen1 = str1.size();
    SQWORD sqLen2 = str2.size();
    string strRet;

    SQWORD sqIdx1 = sqLen1 - 1;
    SQWORD sqIdx2 = sqLen2 - 1;

    SQWORD sqCarry = 0;
    while(1) {
        SQWORD sqNum1;
        SQWORD sqNum2;
        if (sqIdx1 < 0) {
            sqNum1 = 0;
        } else {
            sqNum1 = str1[sqIdx1] - '0';
        }

        if (sqIdx2 < 0) {
            sqNum2 = 0;
        } else {
            sqNum2 = str2[sqIdx2] - '0';
        }
//        printf("%lld %lld\n", sqNum1, sqNum2);

        SQWORD sqNum3 = sqNum1 + sqNum2 + sqCarry;
        if (10 <= sqNum3) {
            sqNum3 -= 10;
            sqCarry = 1;
        } else {
            sqCarry = 0;
        }

        strRet += (char)(sqNum3 + '0');

        sqIdx1--;
        sqIdx2--;
        if ((sqIdx1 < 0) && (sqIdx1 < 0) && (0 == sqCarry)) {
            break;
        }
    }

    reverse(strRet.begin(), strRet.end());
    return strRet;
}

static string DivStrBy2(const string &str)
{
    SQWORD sqPtr = 0;
    string strRet = "";
    SQWORD sqBorrow = 0;
    while(1) {
        SQWORD sqNum = (str[sqPtr] - '0') + 10 * sqBorrow;
        sqBorrow = sqNum % 2;
        SQWORD sqAns = sqNum / 2;

        if (0 == sqPtr) {
            if (0 < sqAns) {
                strRet += (char)('0' + sqAns);
            }
        } else {
            strRet += (char)('0' + sqAns);
        }
        sqPtr++;

        if (str.size() <= sqPtr) {
            break;
        }
    }

    return strRet;
}


bool isConfigurable(
    string &strJudge,
    const PARAMS *pstParam)
{
    SQWORD sqStrLen = pstParam->strS.size();
    SQWORD sqPtr = 0;
    SQWORD sqSepCnt = 0;
    for (;;) {
        if (sqStrLen <= sqPtr) {
            if (pstParam->sqN + 1 == sqSepCnt) {
                return true;
            }
            return false;
        }

        SQWORD sqLenH = min(pstParam->sqSingleLen + (SQWORD)1, (sqStrLen - sqPtr));
        string strH = pstParam->strS.substr(sqPtr, sqLenH);

        if (compStr(strJudge, strH)) {
            sqPtr += sqLenH;
            sqSepCnt++;
            continue;
        }

        SQWORD sqLenL = min(pstParam->sqSingleLen, (sqStrLen - sqPtr));
        string strL = pstParam->strS.substr(sqPtr, sqLenL).c_str();

        if (compStr(strJudge, strL)) {
            sqPtr += sqLenL;
            sqSepCnt++;
            continue;
        }

        return false;
    }
}

static string binarySearch(
    bool (*pfJudge)(string&, const PARAMS*),
    const string &strInitLower, 
    const string &strInitUpper, 
    const PARAMS *pstParam)
{
    string strOk = strInitUpper;
    string strNg = strInitLower;

    while (!(AddStr(strNg, "1") == strOk)) {
        string strMid = DivStrBy2(AddStr(strOk, strNg));

        if (pfJudge(strMid, pstParam)) {
            strOk = strMid;
        } else {
            strNg = strMid;
        }
    }
    return strOk;
}


int main(void)
{
    PARAMS stParam;
    stParam.sqN = inputSQWORD();
    cin >> stParam.strS;

    SQWORD sqLen = stParam.strS.size();
    stParam.sqSingleLen = sqLen / (stParam.sqN + 1);

    string strAns = binarySearch(isConfigurable, "0", stParam.strS, &stParam);
    cout << strAns;
    return 0;
}
