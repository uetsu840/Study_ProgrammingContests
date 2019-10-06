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
static inline DOUBLE ABS(DOUBLE a) {return 0 < a ? a : -a;};

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

static const DOUBLE d_PI = 3.14159265358979323846;

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

class Matrix {
private:
    SQWORD nRows;
    SQWORD nCols;
    vector<SQWORD> vecsqVal;

    SQWORD getIdx(SQWORD r, SQWORD c) {
        return r * nCols + c;
    }
    SQWORD op_sub(SQWORD a, SQWORD b) {
        return (a^b);
    }

public:
    Matrix(SQWORD r, SQWORD c) : nRows(r), nCols(c) {
        vecsqVal.resize(r * c);
    };
    void setValue(SQWORD r, SQWORD c, SQWORD v) {
        SQWORD sqIdx = getIdx(r, c);
        vecsqVal[sqIdx] = v;
    };
    SQWORD getValue(SQWORD r, SQWORD c) {
        SQWORD sqIdx = getIdx(r, c);
        return vecsqVal[sqIdx];
    }
    SQWORD getRowNum(void) {return nRows;};
    SQWORD getColNum(void) {return nCols;};
    void swapRows(SQWORD r1, SQWORD r2) {
        for (SQWORD sqColIdx = 0; sqColIdx < nCols; sqColIdx++) {
            SQWORD sqIdx1 = getIdx(r1, sqColIdx);
            SQWORD sqIdx2 = getIdx(r2, sqColIdx);
            swap(vecsqVal[sqIdx1], vecsqVal[sqIdx2]);
        }
    }
    void diffRows(SQWORD r1, SQWORD r2) {
        for (SQWORD sqColIdx = 0; sqColIdx < nCols; sqColIdx++) {
            SQWORD sqIdx1 = getIdx(r1, sqColIdx);
            SQWORD sqIdx2 = getIdx(r2, sqColIdx);
            vecsqVal[sqIdx1] = op_sub(vecsqVal[sqIdx1], vecsqVal[sqIdx2]);
        }
    }
    void print(void) 
    {  
        printf("---!\n");
        for (SQWORD sqRowIdx = 0; sqRowIdx < nRows; sqRowIdx++) {
            for (SQWORD sqColIdx = 0; sqColIdx < nCols; sqColIdx++) {
                SQWORD sqIdx = getIdx(sqRowIdx, sqColIdx);
                printf("%d ", vecsqVal[sqIdx]);
            }
            printf("\n");
        }
        printf("---!\n");
    }

    bool solve1DEquation(SQWORD& sqRank) {
        sqRank = 0;
        for (SQWORD sqColIdx = 0; sqColIdx < nCols; sqColIdx++) {
            bool bPivot = true;
            if (0 == getValue(sqRank, sqColIdx)) {
                /* search non zero pivot  ->  swap rows */
                SQWORD sqSearchRow = sqRank + 1;
                for (; sqSearchRow < nRows; sqSearchRow++) {
                    if (0 != getValue(sqSearchRow, sqColIdx)) {
                        break;
                    }
                }
                if (sqSearchRow == nRows) {
                    /* All Zero */
                    bPivot = false;
                } else {
                    swapRows(sqRank, sqSearchRow);
                }
            }
            /* pivot */
            if (bPivot) {
                for (SQWORD sqTargetRow = 0; sqTargetRow < nRows; sqTargetRow++) {
                    if (sqTargetRow != sqRank) {
                        if (1 == getValue(sqTargetRow, sqColIdx)) {
                            diffRows(sqTargetRow, sqRank);
                        }
                    }
                }
                sqRank++;
            }
        }
        return true;
    }
};

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
        return sqrt(norm());
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
/**
 *  角度の扱い 
 */
static DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    while ((2.0 * d_PI) < dAngle) {
        dRet -= (2.0 * d_PI);
    }

    return dRet;
}


/*----------------------------------------------*/
/**
 *  凸包を求める 
 */
vector<VECTOR_2D> convex_hull(vector<VECTOR_2D> &pnts)
{
    sort(pnts.begin(), pnts.end());

    SQWORD sqK = 0;
    SQWORD sqN = pnts.size();
    vector<VECTOR_2D> qs(sqN * 2);      /* 構築中の凸包 */

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        while ((1 < sqK) && 
            ((qs[sqK - 1] - qs[sqK - 2]).crossproduct(pnts[sqIdx] - qs[sqK - 1]) <= 0)) {
            sqK--;
        }
        qs[sqK++] = pnts[sqIdx];
    }

    for (SQWORD sqIdx = sqN - 2, sqT = sqK; 0 <= sqIdx; sqIdx--) {
        while ((sqT < sqK) &&
                ((qs[sqK - 1] - qs[sqK - 2]).crossproduct(pnts[sqIdx] - qs[sqK - 1]) <= 0)) {
            sqK--;
        }
        qs[sqK++] = pnts[sqIdx];
    }

    qs.resize(sqK - 1);
    return qs;   
}



/*----------------------------------------------*/
int main(void)
{
    SQWORD sqN = inputSQWORD();

    map<pair<SQWORD, SQWORD>, SQWORD> mapPnts;

    vector<VECTOR_2D> vecPnts;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        
        vecPnts.emplace_back((SQWORD)sqX, (SQWORD)sqY);
        mapPnts.insert(make_pair(make_pair(sqX, sqY), sqIdx));
    }

    vector<VECTOR_2D> vecConvex;
    vecConvex = convex_hull(vecPnts);
    
    SQWORD sqConvexPntNum = vecConvex.size();
    vector<DOUBLE> vecdAngles;
    for (SQWORD sqIdx = 0; sqIdx < sqConvexPntNum; sqIdx++) {
        SQWORD sqPntCur = sqIdx;
        SQWORD sqPntNext = (sqIdx + 1) % sqConvexPntNum;

        VECTOR_2D stTmp = (vecConvex[sqPntCur] - vecConvex[sqPntNext]);
        VECTOR_2D stDir(-stTmp.dY, stTmp.dX);
        DOUBLE dAngle = stDir.angle();
        vecdAngles.emplace_back(dAngle);
    }

    vector<DOUBLE> vecdDiffAngle;
    for (SQWORD sqIdx = 0; sqIdx < sqConvexPntNum; sqIdx++) {
        SQWORD sqAngleIdxCur = sqIdx;
        SQWORD sqAngleIdxNext = (sqIdx + 1) % sqConvexPntNum;

        DOUBLE dDiffAngle = vecdAngles[sqAngleIdxNext] - vecdAngles[sqAngleIdxCur];
        dDiffAngle = normalizeAngle(dDiffAngle);
        vecdDiffAngle.emplace_back(dDiffAngle);
    }
#if 0
    for (auto angle: vecdDiffAngle) {
        printf("-- %f\n", angle);
    }
#endif

    vector<DOUBLE> vecdAns(sqN, 0.0);
    for (SQWORD sqIdx = 1; sqIdx <= sqConvexPntNum; sqIdx++) {
        SQWORD sqAngleIdx = sqIdx - 1;
        SQWORD sqConvexPntIdx = sqIdx % sqConvexPntNum;

        VECTOR_2D pnt = vecConvex[sqConvexPntIdx];
        SQWORD sqPntIdx = mapPnts[make_pair((SQWORD)pnt.dX, (SQWORD)pnt.dY)];

//        printf(">>> %lld %lld %f\n", sqPntIdx, sqAngleIdx, vecdDiffAngle[sqAngleIdx]);

        vecdAns[sqPntIdx] = vecdDiffAngle[sqAngleIdx] / (2 * d_PI);
    }

    for (auto ans: vecdAns) {
        printf("%0.10f\n", ans);
    }

    return 0;
}