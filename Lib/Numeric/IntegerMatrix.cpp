#include "common.h"

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