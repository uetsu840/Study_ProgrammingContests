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


#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; };

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)
#define DIVISOR (1000000007)

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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}


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

/*------------------------------------------------*/

static void getInput(SQWORD sqN, vector<VECTOR_2D> &vPnts)
{
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        DOUBLE dX = (DOUBLE)inputSQWORD();
        DOUBLE dY = (DOUBLE)inputSQWORD();
        vPnts.emplace_back(VECTOR_2D{dX, dY});
    }
}

static DOUBLE getFeature(vector<VECTOR_2D> &vPnts)
{
    SQWORD sqNumPnt = vPnts.size();

    SQWORD sqSumX = 0;
    SQWORD sqSumY = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqNumPnt; sqIdx++) {
        sqSumX += vPnts[sqIdx].dX;
        sqSumY += vPnts[sqIdx].dY;
    }
    VECTOR_2D stCOG;
    stCOG.dX = (DOUBLE)sqSumX / (DOUBLE)sqNumPnt;
    stCOG.dY = (DOUBLE)sqSumY / (DOUBLE)sqNumPnt;

    DOUBLE dMaxNorm = 0;
    for (auto pnt : vPnts) {
        dMaxNorm = max(dMaxNorm, (pnt - stCOG).norm());
    }
    return dMaxNorm;
}


int main(void)
{
    vector<VECTOR_2D> vPntsA;
    vector<VECTOR_2D> vPntsB;

    SQWORD sqN = inputSQWORD();

    getInput(sqN, vPntsA);
    getInput(sqN, vPntsB);

    DOUBLE dSizeA = getFeature(vPntsA);
    DOUBLE dSizeB = getFeature(vPntsB);

    printf("%0.10f\n", sqrt(dSizeB / dSizeA));

    return 0;
}