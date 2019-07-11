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

#define DOUBLE_EPS  (1.0e-12)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) {return (0 < a) ? a : (-1LL) * a; }
static inline SQWORD ABS(DOUBLE a) {return (0 < a) ? a : ((DOUBLE)-1.0) * a; }


#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)
#define DIVISOR (1000000007)

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

/*-------------------------------------------*/

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;

    VECTOR_2D(DOUBLE x, DOUBLE y) {
        dX = x;
        dY = y;
    };

    VECTOR_2D() {
        dX = 0;
        dY = 0;
    }
};

VECTOR_2D operator-(const VECTOR_2D &a, const VECTOR_2D &b)
{
    VECTOR_2D ret;

    ret.dX = a.dX - b.dX;
    ret.dY = a.dY - b.dY;
    return ret;
}

struct MATRIX_2D {
    DOUBLE dA00;
    DOUBLE dA01;
    DOUBLE dA10;
    DOUBLE dA11;

    MATRIX_2D(DOUBLE a00, DOUBLE a01, DOUBLE a10, DOUBLE a11) {
        dA00 = a00;
        dA01 = a01;
        dA10 = a10;
        dA11 = a11;
    }

    MATRIX_2D() {}

    DOUBLE getDeterminant(void)
    {
        return (dA00 * dA11 - dA10 * dA01);
    }

    bool isNominal(void) 
    {
        return (DOUBLE_EPS < ABS(getDeterminant()));
    }

    MATRIX_2D getInverse()
    {
        MATRIX_2D ret;
        DOUBLE dDet = getDeterminant();

        ret.dA00 = dA11 / dDet;
        ret.dA01 = - dA01 / dDet;
        ret.dA10 = - dA10 / dDet;
        ret.dA11 = dA00 / dDet;
    
        return ret;
    }
};

VECTOR_2D operator*(const MATRIX_2D &A, const VECTOR_2D &x)
{
    VECTOR_2D ret;
    ret.dX = A.dA00 * x.dX + A.dA01 * x.dY;
    ret.dY = A.dA10 * x.dX + A.dA11 * x.dY;
    
    return ret;
}


struct LINE {
    VECTOR_2D stPos1;
    VECTOR_2D stPos2;
    VECTOR_2D stDir;

    LINE(VECTOR_2D v1, VECTOR_2D v2)
    {
        stPos1 = v1;
        stPos2 = v2;
        stDir = v2 -  v1;
    }
};

static bool isIntersect(LINE l1, LINE l2)
{
    MATRIX_2D stMat(l1.stDir.dX, - l2.stDir.dX, l1.stDir.dY, - l2.stDir.dY);
    if (!stMat.isNominal()) {
        return false;
    }

    MATRIX_2D stInv = stMat.getInverse();
    VECTOR_2D stVec = l2.stPos1 - l1.stPos1;
    VECTOR_2D stVecST = stInv * stVec;
    DOUBLE s = stVecST.dX;
    DOUBLE t = stVecST.dY;

    if ((0 < s) && (s < 1)
        && (0 < t) && (t < 1)) {
        return true;
    }

    return false;
}

int main()
{
    SQWORD sqInput_Ax = inputSQWORD();
    SQWORD sqInput_Ay = inputSQWORD();
    SQWORD sqInput_Bx = inputSQWORD();
    SQWORD sqInput_By = inputSQWORD();

    VECTOR_2D vecA(sqInput_Ax, sqInput_Ay);
    VECTOR_2D vecB(sqInput_Bx, sqInput_By);

    LINE chopLine(vecA, vecB);

    SQWORD sqInput_N = inputSQWORD();
    vector<VECTOR_2D> vecstPnts;
    
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();

        vecstPnts.emplace_back((DOUBLE)sqX, (DOUBLE)sqY);
    }

    SQWORD sqAns = 0;
    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        LINE stLine(vecstPnts[sqIdx], vecstPnts[(sqIdx + 1) % sqInput_N]);

        if (isIntersect(chopLine, stLine)) {
            sqAns++;
        }
    }

    printf("%lld\n", sqAns / 2 + 1);

    return 0;
}
