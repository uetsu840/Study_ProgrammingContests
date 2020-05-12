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
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

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
static void printVector(vector<SQWORD> v)
{
    for (auto m: v) {
        printf("%lld ", m);
    }
    printf("\n");
}

/*-----------------------------------------------------*/

enum CH_COL {
    COL_W,
    COL_B
};

#define MAX_H   (10)
#define MAX_W   (1000)
#define SQWORD_INF  (1000000000000000000)

static CH_COL s_aaeColorMap[MAX_H][MAX_W];
static SQWORD s_aasqWhiteSum[MAX_H + 1][MAX_W + 1];
static SQWORD s_sqMinCutCnt = SQWORD_INF;


static SQWORD getWhiteCnt(
    SQWORD sqR1,
    SQWORD sqR2,
    SQWORD sqC1,
    SQWORD sqC2)
{
    SQWORD sqCnt = s_aasqWhiteSum[sqR2][sqC2]
                    - s_aasqWhiteSum[sqR2][sqC1] 
                    - s_aasqWhiteSum[sqR1][sqC2]
                    + s_aasqWhiteSum[sqR1][sqC1];

//    printf("[%lld %lld]-[%lld %lld]: %lld\n", sqR1, sqC1, sqR2, sqC2, sqCnt);

    return sqCnt;
}

static void divideRow(SQWORD sqRowDivMap, SQWORD sqH, SQWORD sqW, SQWORD sqK)
{
    vector<SQWORD> vsqStartRows;
    vsqStartRows.emplace_back(0);
    for (SQWORD sqRowIdx = 0; sqRowIdx < sqH; sqRowIdx++) {
        if (sqRowDivMap & (0x1 << sqRowIdx)) {
            vsqStartRows.emplace_back(sqRowIdx + 1);
        } 
    }
    vsqStartRows.emplace_back(sqH);

//    printVector(vsqStartRows);

    SQWORD sqCutCol = 0;
    SQWORD sqCutCnt = 0;
    for (SQWORD sqColIdx = 0; sqColIdx <= sqW; sqColIdx++) {
        for (SQWORD sqBlockIdx = 0; sqBlockIdx < vsqStartRows.size() - 1; sqBlockIdx++) {
            SQWORD sqRow1 = vsqStartRows[sqBlockIdx];
            SQWORD sqRow2 = vsqStartRows[sqBlockIdx + 1];

            SQWORD sqCol1 = sqCutCol;
            SQWORD sqCol2 = sqColIdx;

            if (sqK < getWhiteCnt(sqRow1, sqRow2, sqCol1, sqCol2)) {
                if (sqColIdx == sqCutCol + 1) {
                    /* 1列でKを超える */
                    return;
                }
//                printf("cut %lld\n", sqCutCol);
                sqCutCol = sqColIdx - 1;
                sqCutCnt++;
                break;
            }
        }
    }
    s_sqMinCutCnt = min(s_sqMinCutCnt, (SQWORD)(sqCutCnt + vsqStartRows.size() - 2));
//    printf("--> %lld\n", sqCutCnt);
}


int main(void)
{
    SQWORD sqH = inputSQWORD();
    SQWORD sqW = inputSQWORD();
    SQWORD sqK = inputSQWORD();

    for (SQWORD sqRowIdx = 0; sqRowIdx < sqH; sqRowIdx++) {
        string strS;
        cin >> strS;
        for (SQWORD sqColIdx = 0; sqColIdx < sqW; sqColIdx++) {
            if (strS[sqColIdx] == '1') {
                s_aaeColorMap[sqRowIdx][sqColIdx] = COL_W;
            } else {
                s_aaeColorMap[sqRowIdx][sqColIdx] = COL_B;
            }
        }
    }

    /* 2次元累積和 */
    for (SQWORD sqRowIdx = 0; sqRowIdx < sqH; sqRowIdx++) {
        for (SQWORD sqColIdx = 0; sqColIdx < sqW; sqColIdx++) {
            SQWORD sqCell = 0;
            if (COL_W == s_aaeColorMap[sqRowIdx][sqColIdx]) {
                sqCell = 1;
            }
            s_aasqWhiteSum[sqRowIdx + 1][sqColIdx + 1]
                = s_aasqWhiteSum[sqRowIdx][sqColIdx + 1]
                    + s_aasqWhiteSum[sqRowIdx + 1][sqColIdx]
                    - s_aasqWhiteSum[sqRowIdx][sqColIdx] 
                    + sqCell;
        }
    }

    SQWORD sqMaxDiv = ((SQWORD)1 << (sqH - 1)) - 1;
    for (SQWORD sqRowDivMap = 0; sqRowDivMap <= sqMaxDiv; sqRowDivMap++) {
        divideRow(sqRowDivMap, sqH, sqW, sqK);
    }

    printf("%lld\n", s_sqMinCutCnt);

    return 0;
}