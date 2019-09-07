#include "common.h"

static void calcPrimeFactorication(
    SQWORD sqNum, 
    vector<pair<SQWORD, SQWORD>> &vlPrimes)
{
    if (1 == sqNum) {
        return;
    }
    if (2 == sqNum) {
        vlPrimes.emplace_back(make_pair(2, 1));
        return;
    }
    if (3 == sqNum) {
        vlPrimes.emplace_back(make_pair(3, 1));
        return;
    }

    SQWORD sqCur = sqNum;
    SQWORD sqUpper = sqrt(sqNum) + 1;
    for (SQWORD sqDiv = 2; sqDiv <= sqUpper; sqDiv++) {
        SDWORD lPowerCnt = 0;
        while(0 == sqCur % sqDiv) {
            sqCur /= sqDiv;
            lPowerCnt++;
        }
        if (0 < lPowerCnt) {
            vlPrimes.emplace_back(make_pair(sqDiv, lPowerCnt));
        }
        if (1 == sqCur) {
            break;
        }
    }
    if (1 < sqCur) {
        vlPrimes.emplace_back(make_pair(sqCur, 1));
    }
}