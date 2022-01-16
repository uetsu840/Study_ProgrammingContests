/*----------------------------------------------*/
/**
 * 行列
 */
#define MAX_ROW_COL (100)
template<typename T>
class Matrix {
    SDWORD lRow;
    SDWORD lCol;
    T *_pMem;       /* note: row, col starts with 0 */
 
private:
    T *getMemPtr(SDWORD r, SDWORD c)
    {
        return _pMem + (r * lRow + c);
    }

public:
    Matrix() {};

    void init(SDWORD r = 0, SDWORD c = 0, T _Init = 0) {
        lRow = r, lCol = c;

        _pMem = reinterpret_cast<T*>(malloc(sizeof(T) * lRow * lCol));

        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                *(getMemPtr(lRowIdx, lColIdx)) = 0;
            }
        }
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                *(getMemPtr(lRowIdx, lColIdx)) = 0;
            }
        }
    }

    T *operator[](SDWORD r)
    {
        return _pMem + (r * lRow);
    }

    void SetValue(SDWORD r, SDWORD c, T _Val) 
    {
        this[r][c] = _Val;
    }

    T GetValue(SDWORD r, SDWORD c) 
    {
        return this[r][c];
    }

    Matrix operator* (const Matrix &b) 
    {
        Matrix res = Matrix(lRow, b.lCol);
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < b.lCol; lColIdx++) {
                T _Val = 0;
                for (SDWORD lCellIdx = 0; lCellIdx < b.lRow; lCellIdx++) {
                    _Val += (*this)[lRowIdx][lCellIdx] * (b[lCellIdx][lColIdx]);
                }
                res[lRowIdx][lColIdx] = _Val;
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
                    (*this)[lRowIdx][lColIdx] = 1;
                } else {
                    (*this)[lRowIdx][lColIdx] = 0;
                }
            }
        }
    }

    void Print(void) {
        for (SDWORD lRowIdx = 0; lRowIdx < lRow; lRowIdx++) {
            for (SDWORD lColIdx = 0; lColIdx < lCol; lColIdx++) {
                printf("%1.2f ", (*this)[lRowIdx][lColIdx]);
            }
            cout << endl;
        }
        cout << endl;
    }
};
