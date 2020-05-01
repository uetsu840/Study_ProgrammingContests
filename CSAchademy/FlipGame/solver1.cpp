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


/*----------------------------------------------*/

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;
    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D() : dX(0), dY(0) {};

	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    const VECTOR_2D operator- (const VECTOR_2D a) const {
		return VECTOR_2D(dX - a.dX, dY - a.dY);
    };

    DOUBLE norm(void)
    {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void)
    {
        return sqrt(norm());
    }

    DOUBLE angle(void)
    {
        return atan2(dY, dX);
    }
};


struct PROBLEM_ONE {
    SQWORD sqN;
    vector<VECTOR_2D> vecCenter;
};

const DOUBLE d_PI = 3.14159265358979;

static DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    while ((2.0 * d_PI) < dRet) {
        dRet -= (2.0 * d_PI);
    }

    return dRet;
}


enum PNT_EVENT_TYPE {
    PNT_EVENT_ADD = 0,
    PNT_EVENT_SUB
};

struct INTERSECTION {
    DOUBLE dAngle;
    SQWORD sqCircleIdx;
    PNT_EVENT_TYPE  eEv;

    INTERSECTION(DOUBLE d, SQWORD idx, PNT_EVENT_TYPE e): dAngle(d), sqCircleIdx(idx), eEv(e) {};
};

bool operator< (const INTERSECTION &a, const INTERSECTION &b)
{
    return a.dAngle < b.dAngle;
}


static SQWORD getMaxIntersect(
    const PROBLEM_ONE *pstPro,
    DOUBLE dRadius)
{
    SQWORD sqMaxIntersect = 0;


    for (SQWORD sqBaseIdx = 0; sqBaseIdx < pstPro->sqN; sqBaseIdx++) {
        vector<INTERSECTION> vecstIntersection;
        SQWORD sqIncludeCnt = 0;
        VECTOR_2D stBase = pstPro->vecCenter[sqBaseIdx];
        for (SQWORD sqTgtIdx = 0; sqTgtIdx < pstPro->sqN; sqTgtIdx++) {
            VECTOR_2D stTgt = pstPro->vecCenter[sqTgtIdx];
            if (sqTgtIdx != sqBaseIdx) {
                VECTOR_2D stRel = stTgt - stBase;
                DOUBLE dDist = stRel.dist();

                if (dDist < dRadius * 2.0) {
                    DOUBLE dCenterAngle = stRel.angle();
                    DOUBLE dRelAngle = acos(dDist / (dRadius * 2.0));
                    DOUBLE dAngle1 = normalizeAngle(dCenterAngle - dRelAngle);
                    DOUBLE dAngle2 = normalizeAngle(dCenterAngle + dRelAngle);

                    vecstIntersection.push_back(INTERSECTION{dAngle1, sqTgtIdx, PNT_EVENT_ADD});
                    vecstIntersection.push_back(INTERSECTION{dAngle2, sqTgtIdx, PNT_EVENT_SUB});
                }
            }
        }
        sort(vecstIntersection.begin(), vecstIntersection.end());
        set<SQWORD> setsqCurCrossing;
        SQWORD sqCnt = 0;
        for (SDWORD lCircle = 0; lCircle < 2; lCircle++) {
            for (auto intsect: vecstIntersection) {
                SQWORD sqCircleIdx = intsect.sqCircleIdx;
                if (PNT_EVENT_ADD == intsect.eEv) {
                    /* 追加 */
                    setsqCurCrossing.insert(sqCircleIdx);
                } else {
                    /* 削除 */
                    setsqCurCrossing.erase(sqCircleIdx);
                }
                sqCnt = max((SQWORD)setsqCurCrossing.size(), sqCnt);
            }
        }

        sqMaxIntersect = max(sqCnt, sqMaxIntersect);
    }
    return sqMaxIntersect;
}


/*----------------------------------------------*/

static void printBoard(const vector<vector<SQWORD>> &vvBoard)
{
    for (auto r: vvBoard) {
        for (auto c: r) {
            printf("%lld ", c);
        }
        printf("\n");
    }
}


static void flipRow(vector<vector<SQWORD>> &vvBoard, SQWORD sqRow)
{
    for (SQWORD sqCol = 0; sqCol < vvBoard[sqRow].size(); sqCol++) {
        vvBoard[sqRow][sqCol] ^= 1;
    }
}


static void cntAndFlipCol(vector<vector<SQWORD>> &vvBoard, SQWORD sqCol, SQWORD &sqScore)
{
    SQWORD sqRowNum = vvBoard.size();
    SQWORD sqColNum = vvBoard[0].size();

    /* count */
    SQWORD sqBitZeroCount = 0;
    for (SQWORD sqRow = 0; sqRow < vvBoard.size(); sqRow++) {
        if (0 == vvBoard[sqRow][sqCol]) {
            sqBitZeroCount++;
        }
    }

    /* flip column if more than half of cells is '0' */
#if 0
    if ((sqRowNum / 2) < sqBitZeroCount) {
        for (SQWORD sqRow = 0; sqRow < vvBoard.size(); sqRow++) {
            vvBoard[sqRow][sqCol] ^= 1;
        }
    }
#endif

    sqScore += (max(sqBitZeroCount, sqRowNum - sqBitZeroCount)) << (sqColNum - sqCol - 1);
}



int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<vector<SQWORD>> vvBoard(sqN, vector<SQWORD>(sqM, 0));

    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        for (SQWORD sqCol = 0; sqCol < sqM; sqCol++) {
            vvBoard[sqRow][sqCol] = inputSQWORD();
        }
    }

    /* set lsb to all 1 */
    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        if (0 == vvBoard[sqRow][0]) {
            flipRow(vvBoard, sqRow);
        }
    }

    /* flip sub bits greedy */
    SQWORD sqScore = 0;
    for (SQWORD sqCol = 0; sqCol < sqM; sqCol++) {
        cntAndFlipCol(vvBoard, sqCol, sqScore);
//        printBoard(vvBoard);
    }

    printf("%lld\n", sqScore);

    return 0;
}