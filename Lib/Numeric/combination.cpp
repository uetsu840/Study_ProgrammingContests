#include "common.h"

template<typename T>
class Combination {
    vector<T> _vecComb;
public:
    Combination(SQWORD sqN) {
        /* nCjを事前計算する */
        _vecComb.resize(sqN + 1);

        T sqComb((SQWORD)1);
        _vecComb[0] = sqComb;
        for (SQWORD sqJ = 1; sqJ <= sqN; sqJ++) {
            sqComb *= MODINT(sqN - sqJ + 1);
            sqComb /= MODINT(sqJ);
            _vecComb[sqJ] = sqComb;
        }
    }

    T GetVal(SQWORD sqJ) 
    {
        return _vecComb[sqJ];
    }
};