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


/**
 *  BIT
 *      インデックスは 1 ～ N であることに注意する。
 * 
 */
template<typename T>
class BinaryIndexedTree {
    SDWORD lBitN;
    vector<T> _vecBit; 

public:
    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        _vecBit.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < _vecBit.size(); dwIdx++) {
            _vecBit[dwIdx] = 0;
        }
    }

    T Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        T _sum = 0;
        while (0 < lCur) {
            _sum += _vecBit[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return _sum;
    }

    void Add(SDWORD lIdx, T _x)
    {
        while (lIdx <= lBitN) {
            _vecBit[lIdx] += _x;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(T _sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (_sum < Sum(sqMid));
            } else {
                bJudge = (_sum <= Sum(sqMid));
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
    SDWORD findSumUpperBound(T _sum) 
    {
        return binSearchExec(_sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(T _sum) 
    {
        return binSearchExec(_sum, false);
    }

    SDWORD End() {
        return lBitN;
    }

    void DebugPrint(SDWORD lMax) {
        for (SDWORD lIdx = 0; lIdx <= lMax; lIdx++) {
            printf("%lld ", Sum(lIdx));
        }
        printf("\n");
    }
};


/*----------------------------------------------*/

struct INPUT_ST {
    SQWORD sqPow2;
    SQWORD sqPow5;

    INPUT_ST(SQWORD a, SQWORD b) : sqPow2(a), sqPow5(b) {};
};

bool operator< (const INPUT_ST &a, const INPUT_ST &b) {
    return a.sqPow2 < b.sqPow2;
}

#define     MAX_DP_NUM  (10)
#define     DP_OFFSET   (11)

int main(void)
{

    SQWORD sqN = inputSQWORD();
    
    vector<INPUT_ST> vecInput;


    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        string strS;
        cin >> strS;
        SQWORD sqDp = 0;
        for (SQWORD sqCur = 0; sqCur < strS.length(); sqCur++) {
            if (strS[sqCur] == '.') {
                sqDp = strS.length() - sqCur - 1;
                break;
            }
        }
        SQWORD sqInput = 0;
        if (0 == sqDp) {
            sqInput = atoi(strS.c_str());
        } else {
            SQWORD sqSep = strS.length() - sqDp - 1;
//            printf("---> %lld %lld\n", sqDp, sqSep);
            sqInput = atoi((strS.substr(0, sqSep)
                             + strS.substr(sqSep + 1, strS.length())).c_str());
        }

        SQWORD sqTwo = 0;
        SQWORD sqFive = 0;

        for (;;) {
            if (0 < (sqInput % 2)) {
                break;
            } else {
                sqInput /= 2;
                sqTwo++;
            }
        }
        for (;;) {
            if (0 < (sqInput % 5)) {
                break;
            } else {
                sqInput /= 5;
                sqFive++;
            }
        }
        sqTwo -= sqDp;
        sqFive -= sqDp;
        vecInput.emplace_back(sqTwo, sqFive);
    }

    sort(vecInput.begin(), vecInput.end());

    for (auto i: vecInput) {
        printf("%lld %lld\n", i.sqPow2, i.sqPow5);
    }

    BinaryIndexedTree<SQWORD> bit(MAX_DP_NUM + DP_OFFSET);
    SQWORD sqIdxMinus = 0;
    SQWORD sqIdxPlus = vecInput.size() - 1;

    SQWORD sqAns = 0;
    for (SQWORD sqCurDp = -10; sqCurDp < 0; sqCurDp++) {
        /* マイナス側の数を数える */
        SQWORD sqCntMinus = 0;
        for (;;) {
            if (vecInput[sqIdxMinus].sqPow2 == sqCurDp) {
                sqCntMinus++;
            } else {
                break;
            }
        }

        /* プラス側をBITに放り込む */
        for (;;) {
            if (vecInput[sqIdxPlus].sqPow2 == sqCurDp * -1) {
                bit.Add(vecInput[sqIdxPlus].sqPow5 + DP_OFFSET, 1);
            } else {
                break;
            }
        }
        sqAns += bit.Sum()
    }


    }

    return 0;
}