#include "common.h"
#include "vector2D.h"

/*----------------------------------------------*/
/**
 *  凸包を求める 
 */
vector<VECTOR_2D> convex_hull(vector<VECTOR_2D> &pnts)
{
    sort(pnts.begin(), pnts.end());

    SQWORD sqK = 0;
    SQWORD sqN = pnts.size();
    vector<VECTOR_2D> qs(sqN * 2);      /* 構築中の凸包 */

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        while ((1 < sqK) && 
            ((qs[sqK - 1] - qs[sqK - 2]).crossproduct(pnts[sqIdx] - qs[sqK - 1]) <= 0)) {
            sqK--;
        }
        qs[sqK++] = pnts[sqIdx];
    }

    for (SQWORD sqIdx = sqN - 2, sqT = sqK; 0 <= sqIdx; sqIdx--) {
        while ((sqT < sqK) &&
                ((qs[sqK - 1] - qs[sqK - 2]).crossproduct(pnts[sqIdx] - qs[sqK - 1]) <= 0)) {
            sqK--;
        }
        qs[sqK++] = pnts[sqIdx];
    }

    qs.resize(sqK - 1);
    return qs;   
}

