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

static const DOUBLE d_PI = 3.14159265357989;

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

#define N_MAX_DOGS  (10)
struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;

    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D(const VECTOR_2D &v) : dX(v.dX), dY(v.dY) {};
    VECTOR_2D() : dX(0), dY(0) {};

    /* 加算 */
	VECTOR_2D& operator+= (const VECTOR_2D a)
    { 
        dX = dX + a.dX;
        dY = dY + a.dY;
        return *this;
    };
    VECTOR_2D& operator+ (const VECTOR_2D a) const {
		VECTOR_2D res(*this);
		return (res += a);
    };

    /* 減算 */
	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    VECTOR_2D& operator- (const VECTOR_2D a) const {
		VECTOR_2D res(*this);
		return (res -= a);
    };

    /* 定数倍 */
	VECTOR_2D& operator*= (const DOUBLE a)
    { 
        dX = dX * a;
        dY = dY * a;
        return *this;
    };
    VECTOR_2D& operator* (const DOUBLE a) const {
		VECTOR_2D res(*this);
		return (res *= a);
    };

    DOUBLE norm(void) const {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void) const {
        return sqrt(norm());
    }

    DOUBLE angle(void) const {
        return atan2(dY, dX);
    }
};

/*----------------------------------------------*/

struct FRESBEE_ST {
    VECTOR_2D stStrtPos;
    VECTOR_2D stVel;

    FRESBEE_ST(const VECTOR_2D &p, const VECTOR_2D &v) : stStrtPos(p), stVel(v) {};
};


static bool solve2DEquation(
    DOUBLE dA, DOUBLE dB, DOUBLE dC, DOUBLE &dAns1, DOUBLE &dAns2)
{
    if (abs(dA) < DOUBLE_EPS) {
        /* 1d equation */
        if (abs(dB) < DOUBLE_EPS) {
            return false;
        } else {
            dAns1 = - dC / dB;
            dAns2 = dAns1;
            return true;
        }
    }

    DOUBLE dSq = dB * dB - 4.0 * dA * dC;

    if (dSq < 0) {
        return false;
    }
    DOUBLE dAns_a = (-dB + sqrt(dSq)) / (2.0 * dA);
    DOUBLE dAns_b = (-dB - sqrt(dSq)) / (2.0 * dA);
   
    dAns1 = max(dAns_a, dAns_b);
    dAns2 = min(dAns_a, dAns_b);
    return true;
}


static DOUBLE getCatchTime(
    DOUBLE dTheta, 
    DOUBLE dDist,
    DOUBLE dVelFres,
    DOUBLE dVelDog,
    DOUBLE &dTime)
{
    DOUBLE dA = (dVelFres * dVelFres - dVelDog * dVelDog);
    DOUBLE dB = -2.0 * dDist * cos(dTheta) * dVelFres;
    DOUBLE dC = dDist * dDist;

    DOUBLE dAns1, dAns2;

    if (!solve2DEquation(dA, dB, dC, dAns1, dAns2)) {
//        printf("2d equation false1\n");
        return false;
    }

    if (dAns2 < 0) {
        if (dAns1 < 0) {
//            printf("2d equation false2\n");
            dTime = MAX_DOUBLE;
            return false;
        } else {
            dTime = dAns1;
        }
    } else {
        dTime = dAns2;
    }

//    printf("2d equation -> time %f\n", dTime);
    return true;
}


static bool getCatchTimeOneDog(
    SQWORD sqDogIdx,
    DOUBLE dVelDog,
    const vector<VECTOR_2D> &vecstDogPos,
    const FRESBEE_ST &stFresbee,
    DOUBLE &dCatchTime)
{
    VECTOR_2D stDogPos = vecstDogPos[sqDogIdx];
    VECTOR_2D stFtoD;
    stFtoD = stDogPos - stFresbee.stStrtPos;
    DOUBLE dAngleDog     = atan2(stFtoD.dY, stFtoD.dX);
    DOUBLE dAngleFresbee = atan2(stFresbee.stVel.dY, stFresbee.stVel.dX);
    DOUBLE dTheta = min(abs(dAngleDog - dAngleFresbee), (2.0 * d_PI - abs(dAngleDog - dAngleFresbee)));

    DOUBLE dDist = stFtoD.dist();
    DOUBLE dVelFres = sqrt(stFresbee.stVel.norm());
//    printf("theta = %lf dist = %lf, dVel = %lf\n", dTheta, dDist, dVelFres);
    DOUBLE dTime = MAX_DOUBLE;

    bool bCatchable = getCatchTime(dTheta, dDist, dVelFres, dVelDog, dTime);
    dCatchTime = dTime;

    return bCatchable;
}

static bool solve()
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

//    printf("solve ---> %d %d\n", sqN, sqM);

    if ((0 == sqN) && (0 == sqM)) {
        return false;
    }

    vector<VECTOR_2D> vecPosDogs;
    vector<DOUBLE> vecdVelDogs;
    vector<FRESBEE_ST> vecstFresbee;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        DOUBLE dX = inputFP();
        DOUBLE dY = inputFP();
        DOUBLE dV = inputFP();
        vecPosDogs.emplace_back(dX, dY);
        vecdVelDogs.emplace_back(dV);
    }
    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        DOUBLE dFx = inputFP();
        DOUBLE dFy = inputFP();
        DOUBLE dFvx = inputFP();
        DOUBLE dFvy = inputFP();
        vecstFresbee.emplace_back(VECTOR_2D(dFx, dFy), VECTOR_2D(dFvx, dFvy));
    }

    vector<SQWORD> vecCatchCnt(sqN);
    for (SQWORD sqIter = 0; sqIter < sqM; sqIter++) {
        vector<DOUBLE> vecdCatchTime(sqN, MAX_DOUBLE);
        for (SQWORD sqDogIdx = 0; sqDogIdx < sqN; sqDogIdx++) {
            DOUBLE dCurCatchTime;
            bool bCatchable = getCatchTimeOneDog(sqDogIdx, vecdVelDogs[sqDogIdx], vecPosDogs, 
                                                    vecstFresbee[sqIter], vecdCatchTime[sqDogIdx]);
#if 0
            if (bCatchable) {
                printf(">>> %llf\n", vecdCatchTime[sqDogIdx]);
            } else {
                printf(">>> NG\n");
            }
#endif
        }

        auto min_iter = min_element(vecdCatchTime.begin(), vecdCatchTime.end());
        vecCatchCnt[min_iter - vecdCatchTime.begin()] ++;

        DOUBLE dMinCatchTime = *min_iter;
//        printf("time [%llf]\n", dMinCatchTime);

        /* 犬の位置を更新する */
        for (SQWORD sqDogIdx = 0; sqDogIdx < sqN; sqDogIdx++) {
            DOUBLE dCatchTime = vecdCatchTime[sqDogIdx];
            if (dCatchTime != MAX_DOUBLE) {
                VECTOR_2D stInitPos   = vecPosDogs[sqDogIdx];
                VECTOR_2D stTargetPos = vecstFresbee[sqIter].stStrtPos + vecstFresbee[sqIter].stVel * dCatchTime;
                VECTOR_2D stNextPos   = stInitPos + (stTargetPos - stInitPos) * (dMinCatchTime / dCatchTime);
//                printf("catch [%llf %llf] tgt[%llf %llf]\n", dCatchTime, dMinCatchTime, stTargetPos.dX, stTargetPos.dY);

                vecPosDogs[sqDogIdx] = stNextPos;
            }
//            printf("iter[%d] dog[%d] %llf %llf\n", sqIter, sqDogIdx, vecPosDogs[sqDogIdx].dX, vecPosDogs[sqDogIdx].dY);
        }
    }
    for (SDWORD lIdx = 0; lIdx < sqN; lIdx++) {
        printf("%lld", vecCatchCnt[lIdx]);
        if (lIdx != sqN - 1) {
            printf(" ");
        } else {
            printf("\n\r");
        }
    }

    return true;
}

int main(void)
{
    while(1) {
        if (!solve()) {
            break;
        }
    }
}