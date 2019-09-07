#include "common.h"

/*----------------------------------------------*/
static void calcPrimeFactorication(SQWORD sqNum, vector<pair<SQWORD, SQWORD>> &vlPrimes)
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

static void getDivisorsOne(
    vector<pair<SQWORD, SQWORD>> vpairlPrimes,  
    vector<SQWORD> &vsqDivisors,                /*!<[out] 約数 */
    SQWORD sqCur, 
    SQWORD sqOrgNumber)
{
    if (vpairlPrimes.empty()) {
        vsqDivisors.emplace_back(sqCur);
        return;
    }

    auto prime = vpairlPrimes.back();
    vpairlPrimes.pop_back();
    SQWORD sqDiv = sqCur;

    for (SDWORD lPow = 0; lPow <= prime.second; lPow++) {
        getDivisorsOne(vpairlPrimes, vsqDivisors, sqDiv, sqOrgNumber);
        sqDiv *= prime.first;
    }
}

static void getAllDivisors(SQWORD sqBoxNo)
{
    vector<pair<SQWORD, SQWORD>> vpairlPrimes;
    vector<SQWORD> vsqAllDivisors;

    calcPrimeFactorication(sqBoxNo, vpairlPrimes);   
    getDivisorsOne(vpairlPrimes, vsqAllDivisors, 1, sqBoxNo);
}
