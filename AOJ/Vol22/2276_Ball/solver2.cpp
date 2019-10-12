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
static inline QWORD MAX(QWORD a, QWORD b)    { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b)    { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b)    { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b)    { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; };

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

#define ANS_MOD (1000000007)

static const DOUBLE d_PI = 3.14159265358979323846;
//static const DOUBLE d_PI = M_PI;


static inline bool DoubleIsEqual(DOUBLE a, DOUBLE b) {
    return ABS(a-b) < DOUBLE_EPS;
}

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

#define EPS 1e-10
DOUBLE double_add(DOUBLE a, DOUBLE b) {
    if (ABS(a + b) < EPS * (ABS(a) + ABS(b))) {
        return 0;
    }
    return a+b;
}

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;
    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D() : dX(0), dY(0) {};

    /* 加算 */
	VECTOR_2D& operator+= (const VECTOR_2D a)
    { 
        dX = dX + a.dX;
        dY = dY + a.dY;
        return *this;
    };
    const VECTOR_2D operator+ (const VECTOR_2D a) const {
		return VECTOR_2D(dX + a.dX, dY + a.dY); 
    };

    /* 減算 */
	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    const VECTOR_2D operator- (const VECTOR_2D a) const {
		return VECTOR_2D(dX - a.dX, dY - a.dY);
    };

    /* 定数倍 */
	VECTOR_2D& operator*= (const DOUBLE a)
    { 
        dX = dX * a;
        dY = dY * a;
        return *this;
    };
    const VECTOR_2D operator* (const DOUBLE a) const {
		return VECTOR_2D(dX * a, dY * a); 
    };

    /* 比較 */
    bool operator< (const VECTOR_2D &a) {
        if (dX == a.dX) {
            return (dY < a.dY);
        }
        return dX < a.dX;
    }

    DOUBLE norm(void) const {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void) const {
        return sqrtl(norm());
    }

    DOUBLE angle(void) const {
        return atan2(dY, dX);
    }

    DOUBLE dotproduct(const VECTOR_2D a) const {
        return double_add(dX * a.dX, dY * a.dY);
    }

    DOUBLE crossproduct(const VECTOR_2D a) const {
        return double_add(dX * a.dY, -dY * a.dX);
    }
};



/*----------------------------------------------*/

struct BALL_ST {
    DOUBLE dBegin;
    DOUBLE dEnd;

    BALL_ST (DOUBLE b, DOUBLE e) : dBegin(b), dEnd(e) {};
}; 

bool cmpBegin (const BALL_ST &a, const BALL_ST &b) {
    return a.dBegin < b.dBegin;
}

bool cmpEnd (const BALL_ST &a, const BALL_ST &b) {
    return a.dEnd < b.dEnd;
}

/**
 *  角度の扱い 
 */
static inline DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    if (DoubleIsEqual(2.0 * d_PI, dAngle)) {
        dRet = 0.0;
    } else {
        while ((2.0 * d_PI) <= dAngle) {
            dRet -= (2.0 * d_PI);
        }
    }

    return dRet;
}

#define MAX_N (1500)
int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    vector<BALL_ST> vecBalls;
    vecBalls.emplace_back(0, 0);    /* dp用にインデックスを1はじまりにするためのダミー */
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        SQWORD sqR = inputSQWORD();

        VECTOR_2D stVec((DOUBLE)sqX, (DOUBLE)sqY);
        DOUBLE dAngle = stVec.angle();
        DOUBLE dDist  = stVec.dist();
        DOUBLE dRange = asin((DOUBLE)sqR / dDist);

        DOUBLE dTheta1 = dAngle - dRange;
        DOUBLE dTheta2 = dAngle + dRange;
    
        dTheta1 = normalizeAngle(dTheta1);
        dTheta2 = normalizeAngle(dTheta2);

        DOUBLE dBegin, dEnd;

        if (d_PI <= dTheta1 && d_PI <= dTheta2) {
            dBegin = 0.0;
            dEnd = 0.0;
        } else if (d_PI <= dTheta1) {
            dBegin = 0.0;
            dEnd = dTheta2;
        } else if (d_PI <= dTheta2){
            dBegin = dTheta1;
            dEnd = d_PI;
        } else {
            dBegin = dTheta1;
            dEnd = dTheta2;
        }
        vecBalls.emplace_back(dBegin, dEnd);
    }
    sort(vecBalls.begin(), vecBalls.end(), cmpBegin);
	/* 含まれてしまうものは除く */
	int sz = 0;
	for (int i = 1; i <= sqN; i++) {
		bool isIinJ = false;
		for (int j = 1; j <= sqN; j++) {
			if (i == j) continue;
			if ((vecBalls[j].dBegin < vecBalls[i].dBegin) && (vecBalls[i].dEnd < vecBalls[j].dEnd)) {
				isIinJ = true;
			}
		}
		if (!isIinJ) {
			vecBalls[sz++] = vecBalls[i];
		}
	}

    /**
     *  dp[j][k]    左端がk番目のボールで、
     *              j個を使っているときの最大の角度
     *  dp[j][k] = max(dp[j-1][0] + angle, dp[j-1][1] + angle, dp[j-1][2] + angle ,,,, dp[j-1][k-1] + angle)
     * 
     */
    static DOUBLE s_aadDpTbl[MAX_N + 1][MAX_N + 1];
    for (SQWORD sqIdxJ = 0; sqIdxJ <= sqK; sqIdxJ++) {
        for (SQWORD sqIdxK = 0; sqIdxK <= sz; sqIdxK++) {
            s_aadDpTbl[sqIdxJ][sqIdxK] = 0.0;
        }
    }

    DOUBLE dAns = 0.0;
    for (SQWORD sqIdxJ = 0; sqIdxJ < sqK; sqIdxJ++) {
        for (SQWORD sqIdxK = 0; sqIdxK < sz; sqIdxK++) {
            DOUBLE dPrevEnd = vecBalls[sqIdxK].dEnd;
            for (SQWORD sqNextBallIdx = sqIdxK + 1; sqNextBallIdx <= sz; sqNextBallIdx++) {
                BALL_ST stBall = vecBalls[sqNextBallIdx];
                DOUBLE dAddAngle = 0;
                if (dPrevEnd <= stBall.dBegin) {
                    /* 重なってない */
                    dAddAngle = stBall.dEnd - stBall.dBegin;
//                printf("> [%lld %lld %lld] %llf ", sqIdxJ, sqIdxK, sqNextBallIdx, dAddAngle / d_PI);
                    s_aadDpTbl[sqIdxJ + 1][sqNextBallIdx] 
                        = MAX(s_aadDpTbl[sqIdxJ + 1][sqNextBallIdx], s_aadDpTbl[sqIdxJ][sqIdxK] + dAddAngle);
                } else {
                    /* 重なってる */
                     dAddAngle = stBall.dEnd - dPrevEnd;
                     s_aadDpTbl[sqIdxJ + 1][sqNextBallIdx] 
                         = MAX(s_aadDpTbl[sqIdxJ + 1][sqNextBallIdx], s_aadDpTbl[sqIdxJ][sqIdxK] + dAddAngle);
//                printf("< [%lld %lld %lld] %llf ", sqIdxJ, sqIdxK, sqNextBallIdx, dAddAngle / d_PI);
                }

                dAns = max(s_aadDpTbl[sqIdxJ + 1][sqNextBallIdx], dAns);
            }
        }
//        printf("\n");
    }

    printf("%0.11f\n", dAns / d_PI);

    return 0;
}