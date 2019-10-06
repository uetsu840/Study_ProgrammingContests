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

/**
 *  BIT
 *      引数のインデックスは 0 ～ N-1、
 *      内部のインデックスは 1 ～ N であることに注意する。
 * 
 */
struct BinaryIndexedTree {
    SDWORD lBitN;
    vector<SQWORD> vecsqBitN; 

    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        vecsqBitN.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < vecsqBitN.size(); dwIdx++) {
            vecsqBitN[dwIdx] = 0;
        }
    }

    SQWORD Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        SQWORD sqSum = 0;
        while (0 < lCur) {
            sqSum += vecsqBitN[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return sqSum;
    }

    void Add(SDWORD lIdx, SQWORD sqX)
    {
        while (lIdx <= lBitN) {
            vecsqBitN[lIdx] += sqX;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(SQWORD sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (sum < Sum(sqMid));
            } else {
                bJudge = (sum <= Sum(sqMid));
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
    SDWORD findSumUpperBound(SQWORD sum) 
    {
        return binSearchExec(sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(SQWORD sum) 
    {
        return binSearchExec(sum, false);
    }

    SDWORD End() {
        return lBitN;
    }
};

/*----------------------------------------------*/
#define MAX_N_POINTS  (50)

struct POINT {
    SQWORD sqX;
    SQWORD sqY;

    SQWORD sqIdxX;  /* インデックス(BITに入れるので1はじまり) */
    SQWORD sqIdxY;  /* インデックス(BITに入れるので1はじまり) */
    POINT(SQWORD x, SQWORD y) : sqX(x), sqY(y) {};
 };

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    vector<POINT> vecstPoint;
    set<SQWORD> setsqX;
    set<SQWORD> setsqY;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vecstPoint.emplace_back(sqX, sqY);
        setsqX.insert(sqX);
        setsqY.insert(sqY);
    }

    SQWORD sqIdxX = 1;
    vector<SQWORD> vecsqPosX;
    vecsqPosX.emplace_back(0);
    for (auto posX: setsqX) {
        for (auto it = vecstPoint.begin(); it != vecstPoint.end(); ++it) {
            if (it->sqX == posX) {
                it->sqIdxX = sqIdxX;
            }
        }
        vecsqPosX.emplace_back(posX);
        sqIdxX++;
    }
    SQWORD sqIdxY = 1;
    vector<SQWORD> vecsqPosY;
    vecsqPosY.emplace_back(0);
    for (auto posY: setsqY) {
        for (auto it = vecstPoint.begin(); it != vecstPoint.end(); ++it) {
            if (it->sqY == posY) {
                it->sqIdxY = sqIdxY;
            }
        }
        vecsqPosY.emplace_back(posY);
        sqIdxY++;
    }

    vector<SQWORD> avecIdxY[MAX_N_POINTS + 1];
    for (auto pnt: vecstPoint) {
//        printf("%lld %lld\n", pnt.sqIdxX, pnt.sqIdxY);
        avecIdxY[pnt.sqIdxX].emplace_back(pnt.sqIdxY);
    }

    SQWORD sqAns = MAX_SQWORD;
    SQWORD sqPosXNum = setsqX.size();
    SQWORD sqPosYNum = setsqY.size();
    for (SQWORD sqIdxX_L = 1; sqIdxX_L <= sqPosXNum; sqIdxX_L++) {
        BinaryIndexedTree bit(sqPosXNum);
        for (SQWORD sqIdxX_R = sqIdxX_L; sqIdxX_R <= sqPosXNum; sqIdxX_R++) {
//            printf("--- [%lld %lld]\n", sqIdxX_L, sqIdxX_R);
            for (auto posY: avecIdxY[sqIdxX_R]) {
                bit.Add(posY, 1);
            }
            for (SQWORD sqIdxY_L = 1; sqIdxY_L <= sqPosYNum; sqIdxY_L++) {
                SQWORD sqBase = bit.Sum(sqIdxY_L - 1);
                SQWORD sqIdxY_R = bit.findSumLowerBound(sqBase + sqK);

                if (sqIdxY_R != bit.End()) {
//                    printf("X[%lld %lld] Y[%lld %lld]\n", sqIdxX_L, sqIdxX_R, sqIdxY_L, sqIdxY_R);
                    SQWORD sqDiffX = vecsqPosX[sqIdxX_R] - vecsqPosX[sqIdxX_L];
                    SQWORD sqDiffY = vecsqPosY[sqIdxY_R] - vecsqPosY[sqIdxY_L];
                    sqAns = min(sqAns, sqDiffX * sqDiffY);
                }
            }
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}