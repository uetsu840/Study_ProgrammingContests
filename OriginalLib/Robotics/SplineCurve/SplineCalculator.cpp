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
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}
static inline SQWORD DIV_UP(SQWORD a, SQWORD x) {return (a + (x - 1)) / x;}

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

/*-----------------------------------------------------*/
#include "matrix.h"

class SplineCaluculator {
    SQWORD sqPointNum;
    SQWORD sqDimension;
    SQWORD sqDiides;
    SQWORD m_sqMatrixDim;       /* (Imaxに対応) */
    SQWORD m_sqCoxDim;
    SQWORD m_sqOrder;           /* 次数 */

    vector<DOUBLE> m_vdfDataX;
    vector<DOUBLE> m_vdfQ;
    Matrix<DOUBLE> m_matBd;

private:
    /**
    *   const vector<double>& knot : ノット列
    *   unsigned int j : ノット列の開始番号
    *   unsigned int p : 次数
    *   double t : 計算対象の独立変数
    */
    double CalcBSplineBasisFunc( const vector<double>& knot, unsigned int j, unsigned int p, double t ) {
        if ( knot.size() == 0 ) return( NAN );

        // ノット列のデータ長が充分でない場合は nan を返す
        unsigned int m = knot.size() - 1;
        if ( m < j + p + 1 ) {
            printf("          <>%lld %lld %lld\n", m, j, p);
            return(NAN);
        } 

        // 正値をとる範囲外ならゼロを返す
        if ( ( t < knot[j] ) || ( t > knot[j + p + 1] )) {
            return 0;
        };
        // p = 0 かつ knot[j] <= t <= knot[j + p + 1] なら 1 を返す
        if ( p == 0 ) {
            return 1;            
        };
        // p = 1 の場合、三角の頂点の値は特別扱い
        if ((p == 1) && (t == knot[j + 1])) {
            return 1;
        };

        // 漸化式の計算
        double d1 = 0.0, d2 = 0.0;
        double d1_diff = knot[j + p] - knot[j];
        if (DOUBLE_EPS < d1_diff) {
            d1 = ( t - knot[j] ) * CalcBSplineBasisFunc( knot, j, p - 1, t ) / d1_diff;
        }
        double d2_diff = knot[j + p + 1] - knot[j + 1];
        if (DOUBLE_EPS < d2_diff) {
            d2 = ( knot[j + p + 1] - t ) * CalcBSplineBasisFunc( knot, j + 1, p - 1, t ) / d2_diff;
        }
        return( d1 + d2 );
    }

#if 0
    /**
     *  @brief  de Boor-Cox の漸化式により B-スプラインを計算する
     */
    void calcDeBoorCox(
        SQWORD sqOrder, 
        DOUBLE dfT,                     /* x または t の値 */
        SQWORD *psqSetK, 
        const vector<DOUBLE> &vdfQ, 
        vector<DOUBLE> &vdfB)
    {
        printf("    :T = %2.2f dim[%d]\n", dfT, m_sqCoxDim);
        for (SQWORD sqIdx = 0; sqIdx <= m_sqCoxDim; sqIdx++) {
            vdfB[sqIdx] = 0.0;
        }

        for (SQWORD sqIdx = 0; sqIdx < m_sqCoxDim; sqIdx++) {
            if ((vdfQ[sqIdx] <= dfT) && (dfT < vdfQ[sqIdx + 1])) {
                vdfB[sqIdx] = 1.0;
                *psqSetK = sqIdx;
            }
        }
        if ((vdfQ[m_sqCoxDim - 1] <= dfT) && (dfT <= vdfQ[m_sqCoxDim] + DOUBLE_EPS)) {
            vdfB[m_sqCoxDim - 1] = 1.0;
            *psqSetK = m_sqCoxDim - 1;
        }

        printf("B: ");
        for (SQWORD sqIdx = 0; sqIdx < vdfB.size(); sqIdx++) {
            printf("%1.1f ", vdfB[sqIdx]);
        }
        printf("\n");



        for (SQWORD sqCurOrder = 1; sqCurOrder < sqOrder; sqCurOrder++) {
            vector<DOUBLE> vdfBNext(m_sqCoxDim + 1, 0.0);

            printf("setk: %lld\n", *psqSetK);
            for (SQWORD sqIdx = 0; sqIdx <= *psqSetK; sqIdx++) {
                DOUBLE dfA1 = 0.0;
                DOUBLE dfA2 = 0.0;

                DOUBLE dfA1Div = vdfQ[sqIdx + (sqCurOrder + 1)] - vdfQ[sqIdx + 1];
                if (DOUBLE_EPS < dfA1Div) {
                    dfA1 = (vdfQ[sqIdx + (sqCurOrder + 1)] - dfT) * vdfB[sqIdx + 1] / dfA1Div;
                }

                DOUBLE dfA2Div = vdfQ[sqIdx + sqCurOrder] - vdfQ[sqIdx];
                if (DOUBLE_EPS < dfA2Div) {
                    dfA2 = (dfT - vdfQ[sqIdx]) * vdfB[sqIdx] / dfA2Div;
                }
                printf("%1.2lf, %1.2lf, %1.2llf %1.2llf\n", dfA1Div, dfA2Div, dfA1, dfA2);
                vdfBNext[sqIdx] = dfA1 + dfA2;
            }
            swap(vdfBNext, vdfB);
        }
    }
#endif

public:
    void SetParameter(const vector<DOUBLE> &vdfDataX, SQWORD sqOrder) {
        m_vdfDataX = vdfDataX;
        m_sqOrder = sqOrder;
//        m_sqMatrixDim = vdfDataX.size() + sqOrder * 2;
        m_sqMatrixDim = vdfDataX.size() + (sqOrder - 1);
        m_sqCoxDim = m_sqMatrixDim;
        m_matBd.init(m_sqMatrixDim, m_sqMatrixDim, 0.0);
    }

    void setQ() {
        m_vdfQ.clear();

        for (SQWORD sqIdx = 0; sqIdx < m_sqOrder; ++sqIdx) {
            m_vdfQ.emplace_back(*(m_vdfDataX.begin()));
        }
        for (SQWORD sqIdx = 0; sqIdx < m_vdfDataX.size(); ++sqIdx) {
            m_vdfQ.emplace_back((m_vdfDataX[sqIdx]));
        }
        for (SQWORD sqIdx = 0; sqIdx < m_sqOrder; ++sqIdx) {
            m_vdfQ.emplace_back(*(m_vdfDataX.rbegin()));
        }
    }

    void makeBMatrix() {
#if 0
        for (SQWORD sqLine = 0; sqLine < m_vdfDataX.size(); sqLine++) {
            SQWORD sqSetK;
            vector<DOUBLE> vdfB(m_sqCoxDim, 0.0);

            DOUBLE dfT = m_vdfDataX[sqLine];
            calcDeBoorCox(m_sqOrder, dfT, &sqSetK, m_vdfQ, vdfB);
            for (SQWORD sqIdx = 0; sqIdx < vdfB.size(); ++sqIdx) {
                m_matBd[sqLine][sqIdx] = vdfB[sqIdx];
            }
        }
#endif
        for (SQWORD sqLine = 0; sqLine < m_vdfDataX.size(); sqLine++) {
            DOUBLE dfT = m_vdfDataX[sqLine];
            for (SQWORD sqIdx = 0; sqIdx < m_vdfDataX.size() + (m_sqOrder - 1); ++sqIdx) {
                DOUBLE dfB = CalcBSplineBasisFunc(m_vdfQ, sqIdx, m_sqOrder, dfT);
                m_matBd[sqLine][sqIdx] = dfB;
                printf("t:[%1.2f]  %lld %lld: %llf\n", dfT, sqLine, sqIdx, dfB);
            }
        }
    }

    void printQ() {
        for (SQWORD sqIdx = 0; sqIdx < m_vdfQ.size(); ++sqIdx) {
            cout << m_vdfQ[sqIdx] << " ";
        }
        cout << endl;
    }

    void printBMatrix() {
        m_matBd.Print();
    }
};


/*----------------------------------------------*/
int main(void)
{
    SQWORD sqCountX = inputSQWORD();

    vector<DOUBLE> vdfX;
    vector<DOUBLE> vdfY;
    for (SQWORD sqIdx = 0; sqIdx < sqCountX; sqIdx++) {
        vdfX.emplace_back(inputFP());
        vdfY.emplace_back(inputFP());
    }

    SQWORD sqOrder = inputSQWORD();


#if 0
    Matrix<DOUBLE> mat;
    mat.init(2,2);
    mat[0][0] = 1.0;
    mat[1][1] = 2.0;
    mat.Print();
#endif

    SplineCaluculator spline;
    spline.SetParameter(vdfX, sqOrder);
    spline.setQ();
    spline.printQ();

    spline.makeBMatrix();
    spline.printBMatrix();
}
