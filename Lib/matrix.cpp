/*----------------------------------------------*/
/**
 * 行列
 */
#define MAX_ROW_COL (10)
template<typename T>
class Matrix {
    SDWORD lRow;
    SDWORD lCol;
    T _aaMem[MAX_ROW_COL][MAX_ROW_COL];       /* note: row, col starts with 0 */
 
public:
    Matrix(SDWORD r = 0, SDWORD c = 0, T _Init = 0) {
        lRow = r, lCol = c;
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                _aaMem[lRowIdx][lColIdx] = 0;
            }
        }
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                _aaMem[lRowIdx][lColIdx] = _Init;
            }
        }
    }
 
    void SetValue(SDWORD r, SDWORD c, T _Val) 
    {
        _aaMem[r][c] = _Val;
    }

    T GetValue(SDWORD r, SDWORD c) 
    {
        return _aaMem[r][c];
    }

    Matrix operator* (const Matrix &b) 
    {
        Matrix res = Matrix(lRow, b.lCol);
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < b.lCol; lColIdx++) {
                T _Val = 0;
                for (SDWORD lCellIdx = 0; lCellIdx < b.lRow; lCellIdx++) {
                    _Val += _aaMem[lRowIdx][lCellIdx] * (b._aaMem[lCellIdx][lColIdx]);
                }
                res._aaMem[lRowIdx][lColIdx] = _Val;
            }
        }
        return res;
    }
 
    Matrix operator ^ (SQWORD k)
    {
        Matrix res(lRow, lCol, 0);
        res.unit_matrix();
        Matrix x = *this;
        while (k) {
            if (k & 1) res = res * x;
            x = x * x;
            k >>= 1;
        }
        return res;
    }

    void unit_matrix(void) {
       for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                if (lRowIdx == lColIdx) {
                    _aaMem[lRowIdx][lColIdx] = 1;
                } else {
                    _aaMem[lRowIdx][lColIdx] = 0;
                }
            }
        }
    }

    void Print(void) {
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                cout << _aaMem[lRowIdx][lColIdx].getVal() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};
