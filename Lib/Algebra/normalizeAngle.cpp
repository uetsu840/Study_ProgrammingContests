#include "common.h"

/**
 *  角度の扱い 
 */
static inline DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    while ((2.0 * d_PI) < dAngle) {
        dRet -= (2.0 * d_PI);
    }

    return dRet;
}
