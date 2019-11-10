#include "common.h"

/**
 *  BIT
 *      インデックスは 1 ～ N であることに注意する。
 * 
 */
template<typename T>
class BinaryIndexedTree {
    SDWORD lBitN;
    vector<T> _vecBit; 

public:
    BinaryIndexedTree(SDWORD lNum)
    {
        lBitN = lNum + 1;
        _vecBit.resize(lBitN + 1);
        for (DWORD dwIdx = 0; dwIdx < _vecBit.size(); dwIdx++) {
            _vecBit[dwIdx] = 0;
        }
    }

    T Sum(const SDWORD lIdx)
    {
        if (lIdx < 1) {
            return 0;
        }

        SDWORD lCur = lIdx;
        T _sum = 0;
        while (0 < lCur) {
            _sum += _vecBit[lCur];
            lCur -= (lCur & (-lCur));     /* 最後の1ビット */
        }
        return _sum;
    }

    void Add(SDWORD lIdx, T _x)
    {
        while (lIdx <= lBitN) {
            _vecBit[lIdx] += _x;
            lIdx += (lIdx & (-lIdx));
        }
    }

    SDWORD binSearchExec(T _sum, bool bIsUb) 
    {
        SQWORD sqLb = 0;
        SQWORD sqUb = lBitN;

        while (1LL < sqUb - sqLb) {
            SQWORD sqMid = (sqUb + sqLb) / 2LL;
            bool bJudge;
            if (bIsUb) {
                bJudge = (_sum < Sum(sqMid));
            } else {
                bJudge = (_sum <= Sum(sqMid));
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
    SDWORD findSumUpperBound(T _sum) 
    {
        return binSearchExec(_sum, true);
    }

    /**
    *  累積和が指定した数以上になるインデックスを求める。 
    *  (最小より小さい、最大より大きいとうまく動かないかも…)
    */
    SDWORD findSumLowerBound(T _sum) 
    {
        return binSearchExec(_sum, false);
    }

    SDWORD End() {
        return lBitN;
    }

    void DebugPrint(SDWORD lMax) {
        for (SDWORD lIdx = 0; lIdx <= lMax; lIdx++) {
            printf("%lld ", Sum(lIdx));
        }
        printf("\n");
    }
};
