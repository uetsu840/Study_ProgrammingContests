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
#define N_MAX_NODE    (600)

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<SQWORD> s_avecsqOutEdge[N_MAX_NODE + 1];
    vector<SQWORD> s_avecsqInEdge[N_MAX_NODE + 1];
    
    for (SQWORD sqIdxE = 0; sqIdxE < sqM; sqIdxE++) {
        SQWORD sqS = inputSQWORD();
        SQWORD sqT = inputSQWORD();

        s_avecsqOutEdge[sqS].emplace_back(sqT);
        s_avecsqInEdge[sqT].emplace_back(sqS);
    }

    /* 頂点番号の若い順に、入ってくる辺を合計して頂点の通過確率を計算する。 */
    static DOUBLE s_adProba[N_MAX_NODE+1];
    s_adProba[1] = 1.0;
    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        DOUBLE dProbCur = s_adProba[sqNode];
        SQWORD sqOutEdgeNum = s_avecsqOutEdge[sqNode].size();

        for (auto next: s_avecsqOutEdge[sqNode]) {
            s_adProba[next] += dProbCur / (DOUBLE)sqOutEdgeNum;
        }
    }

    /* 先頭から各頂点までの距離の期待値を求める */
    static DOUBLE s_adDistExValF[N_MAX_NODE+1];
    s_adDistExValF[1] = 0.0;
    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        DOUBLE dExValFCur = s_adDistExValF[sqNode];
        SQWORD sqOutEdgeNum = s_avecsqOutEdge[sqNode].size();
        for (auto next: s_avecsqOutEdge[sqNode]) {
            DOUBLE dEdgeExVal = s_adProba[sqNode] / (DOUBLE)sqOutEdgeNum;
            s_adDistExValF[next] += dExValFCur / (DOUBLE)sqOutEdgeNum + dEdgeExVal;
        }
    }

    /* 各頂点から出口までの期待値を求める */
    static DOUBLE s_adDistExValR[N_MAX_NODE+1];
    for (SQWORD sqStart = 1; sqStart <= sqN; sqStart++) {
        vector<DOUBLE> vecdProbaPartial(N_MAX_NODE+1, 0);
        vecdProbaPartial[sqStart] = 1.0;
        for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
            DOUBLE dProbCur = vecdProbaPartial[sqNode];
            SQWORD sqOutEdgeNum = s_avecsqOutEdge[sqNode].size();

            for (auto next: s_avecsqOutEdge[sqNode]) {
                vecdProbaPartial[next] += dProbCur / (DOUBLE)sqOutEdgeNum;
            }
        }

        printf("edge ex %d\n", sqStart);
        vector<DOUBLE> vecdDistExVal(N_MAX_NODE+1 ,0.0);
        vecdDistExVal[sqStart] = 0.0;
        for (SQWORD sqNode = sqStart; sqNode <= sqN; sqNode++) {
            DOUBLE dExValCur = vecdDistExVal[sqNode];
            SQWORD sqOutEdgeNum = s_avecsqOutEdge[sqNode].size();
            for (auto next: s_avecsqOutEdge[sqNode]) {
                DOUBLE dEdgeExVal = vecdProbaPartial[sqNode] / (DOUBLE)sqOutEdgeNum;
                vecdDistExVal[next] += dExValCur / (DOUBLE)sqOutEdgeNum + dEdgeExVal;
                printf("next %d add %f\n", next, dExValCur / (DOUBLE)sqOutEdgeNum + dEdgeExVal);
            }
        }
        s_adDistExValR[sqStart] = vecdDistExVal[sqN];
    }

    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        printf("prob:%lf ex-f:%lf ex-r:%lf\n", 
            s_adProba[sqNode], s_adDistExValF[sqNode], s_adDistExValR[sqNode]);
    }

    /* 期待値が最も大きな辺を探す */
    DOUBLE dExValInit = s_adDistExValF[sqN];
    pair<SQWORD, SQWORD> pairsqLargestEdge = make_pair(0, 0);
    DOUBLE dScoreMin = dExValInit;
    for (SQWORD sqNode = 1; sqNode <= sqN; sqNode++) {
        for (auto next: s_avecsqOutEdge[sqNode]) {
            SQWORD sqEdgeNumCur  = s_avecsqOutEdge[sqNode].size(); 
            SQWORD sqEdgeNumNext = s_avecsqInEdge[next].size();
            DOUBLE dProba = s_adProba[sqNode] / sqEdgeNumCur;
            DOUBLE dExValTtl = (s_adDistExValF[sqNode]
                                + s_adDistExValR[next] * dProba
                                + dProba);

            DOUBLE dCurScore = (1.0 / (1.0 - dProba)) * (dExValInit - dExValTtl);
            if (dCurScore <= dScoreMin) {
                printf("%lld %lld %lf \n", sqNode, next, dExValTtl);
                dScoreMin = dCurScore;
            }                              
        }
    }

    printf("%0.10f\n", dScoreMin);

    return 0;
}