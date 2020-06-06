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



static bool isGreaterRatio(
    SQWORD sqP_A,
    SQWORD sqP_B,
    SQWORD sqQ_A,
    SQWORD sqQ_B)
{
    return (LDOUBLE)sqP_A * (LDOUBLE)sqQ_B
             < (LDOUBLE)sqP_B * (LDOUBLE)sqQ_A;
}

/*----------------------------------------------*/

struct ST_SARDINE {
    SQWORD sqA;
    SQWORD sqB;

    ST_SARDINE(SQWORD a, SQWORD b) : sqA(abs(a)), sqB(abs(b)) {};
};

bool operator< (const ST_SARDINE &p, const ST_SARDINE &q) {
    return isGreaterRatio(p.sqA, p.sqB, q.sqA, q.sqB);
}

/*----------------------------------------------*/

struct ST_S_GROUP {
    SQWORD sqCnt;
    SQWORD sqM;
    SQWORD sqN;
    
    ST_S_GROUP(SQWORD c, SQWORD m, SQWORD n) : sqCnt(c), sqM(m), sqN(n) {};
};

static void get_sardine_groups(
    vector<ST_SARDINE> &vS,
    vector<ST_S_GROUP> &vstGroups)
{
    if (0 == vS.size()) {
        return;
    }

    sort(vS.begin(), vS.end());

    SQWORD sqSAPrev = -1;
    SQWORD sqSBPrev = -1;
    SQWORD sqCnt = 1;
    for (SQWORD sqIdx = 0; sqIdx < vS.size(); sqIdx++) {
        SQWORD sqA = vS[sqIdx].sqA;
        SQWORD sqB = vS[sqIdx].sqB;
        SQWORD sqGcdCur = __gcd(sqA, sqB);
        SQWORD sqSA = sqA / sqGcdCur;
        SQWORD sqSB = sqB / sqGcdCur;

        if (0 < sqIdx) {
            if ((sqSA == sqSAPrev) && (sqSB == sqSBPrev)) {
                sqCnt++;
            } else {
                vstGroups.emplace_back(sqCnt, sqSAPrev, sqSBPrev);
                sqCnt = 1;
            }
        }
        sqSAPrev = sqSA;
        sqSBPrev = sqSB;
    }
    vstGroups.emplace_back(sqCnt, sqSAPrev, sqSBPrev);
}


int main(void)
{
    SQWORD sqN = inputSQWORD();

    vector<ST_SARDINE> vstSardZZ;
    vector<ST_SARDINE> vstSardZM;
    vector<ST_SARDINE> vstSardMZ;
    vector<ST_SARDINE> vstSardP;
    vector<ST_SARDINE> vstSardN;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();

        if (sqA < (SQWORD)0) {
            sqA *= (SQWORD)-1;
            sqB *= (SQWORD)-1;
        }
        if ((0 == sqA) && (0 == sqB)) {
            vstSardZZ.emplace_back(sqA, sqB);
        } else if (0 == sqA) {
            vstSardZM.emplace_back(sqA, sqB);
        } else if (0 == sqB) {
            vstSardMZ.emplace_back(sqA, sqB);
        } else {
            if (0 < sqB) {
                vstSardP.emplace_back(sqA, sqB);
            } else {
                vstSardN.emplace_back(sqA, sqB);
            }
        }
    }

    vector<ST_S_GROUP> vGrP;
    vector<ST_S_GROUP> vGrN;
    get_sardine_groups(vstSardP, vGrP);
    get_sardine_groups(vstSardN, vGrN);

    auto it_p = vGrP.begin();
    auto it_n = vGrN.rbegin();


    vector<SQWORD> vecPow2;
    vecPow2.emplace_back(1);
    for (SQWORD sqPow = 0; sqPow < sqN; sqPow++) {
        SQWORD sqNum = *(vecPow2.rbegin());
        vecPow2.emplace_back((sqNum * 2) % ANS_MOD);
    }

    MODINT miPtn(1);
    SQWORD sqSum = 0;
    for (;;) {
        if (it_p == vGrP.end()) {
            while (it_n != vGrN.rend()) {
                miPtn *= MODINT(vecPow2[it_n->sqCnt]);
                it_n++;
            }
            break;
        } 
        if (it_n == vGrN.rend()) {
            while (it_p != vGrP.end()) {
                miPtn *= MODINT(vecPow2[it_p->sqCnt]);
                it_p++;
            }
            break;
        }
        if ((it_p->sqM == it_n->sqN) && (it_p->sqN == it_n->sqM)) {
            miPtn *= (MODINT(vecPow2[it_p->sqCnt])
                      + (MODINT(vecPow2[it_n->sqCnt])) - MODINT(1));
            sqSum += (it_p->sqCnt + it_n->sqCnt);
            it_p++;
            it_n++;
        } else if (isGreaterRatio(it_p->sqM, it_p->sqN, it_n->sqN, it_n->sqM)) {
            miPtn *= MODINT(vecPow2[it_p->sqCnt]);
            it_p++;
        } else {
            miPtn *= MODINT(vecPow2[it_n->sqCnt]);
            it_n++;
        }
    }

    /* 0 を含むパターン */
    miPtn *= (MODINT(vecPow2[vstSardZM.size()])
                + MODINT(vecPow2[vstSardMZ.size()]) - MODINT(1));

    /* [0,0] 他と組み合わせてはいけない */
    miPtn += vstSardZZ.size();

    printf("%lld\n", miPtn - 1);

    return 0;
}
