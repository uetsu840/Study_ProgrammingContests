#pragma GCC optimize ("O3")
using namespace std;

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
#include <cstdint>
#include <stack>

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

/*----------------------------------------------*/

/*----------------------------------------------*/

struct ST_FISH {
    SQWORD sqW;
    SQWORD sqX;
    SQWORD sqV;

    ST_FISH(SQWORD w, SQWORD x, SQWORD v): sqW(w), sqX(x), sqV(v) {};
};

bool getIntersectTime(const ST_FISH &fbase, const ST_FISH &fcur, SQWORD sqA, DOUBLE &t1, DOUBLE &t2)
{
    SQWORD sqDiffX = fcur.sqX - fbase.sqX;
    SQWORD sqDiffV = fcur.sqV - fbase.sqV;

    if (0 == sqDiffV) {
        if ((0 <= sqDiffX) &&  (sqDiffX <= sqA)) {
            t1 = 0;
            t2 = INFINITY;
            return true;
        } else {
            return false;
        }
    }

    if (0 < sqDiffX) {
        /* f_base->  f_cur-> */
        if (0 < sqDiffV) {
            t1 = (DOUBLE)(0   - sqDiffX) / (DOUBLE)sqDiffV;
            t2 = (DOUBLE)(sqA - sqDiffX) / (DOUBLE)sqDiffV;
        } else {
            t1 = (DOUBLE)(sqDiffX - sqA) / (DOUBLE)(- sqDiffV);
            t2 = (DOUBLE)(sqDiffX -   0) / (DOUBLE)(- sqDiffV);
        }
    } else {
        /* f_cur -> f_base */
        if (0 < sqDiffV) {
            t1 = (DOUBLE)(- sqDiffX)       / (DOUBLE)(sqDiffV);
            t2 = (DOUBLE)(- sqDiffX + sqA) / (DOUBLE)(sqDiffV);
        } else {
            return false;
        }      
    }
//    printf("::::          [%d %d] >[%f %f]\n", sqDiffX, sqDiffV, t1, t2);

    if ((t1 < 0) && (t2 < 0)) {
        return false;
    }
    if (t1 < 0) {
        t1 = 0;
    }
//    printf("[%d %d] >[%f %f]\n", sqDiffX, sqDiffV, t1, t2);
    return true;
}

struct ST_INTERSECT {
    DOUBLE dT;
    SQWORD sqW;
    ST_INTERSECT(DOUBLE t, SQWORD w): dT(t),sqW(w) {};
};

bool operator< (const ST_INTERSECT &a, const ST_INTERSECT &b) {
    if (ABS(a.dT - b.dT) < DOUBLE_EPS) {
       return a.sqW > b.sqW; 
    }

    return a.dT < b.dT;
}
 


int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqA = inputSQWORD();

    vector<ST_FISH> vstFish;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqW = inputSQWORD();
        SQWORD sqX = inputSQWORD();
        SQWORD sqV = inputSQWORD();
        vstFish.emplace_back(sqW, sqX, sqV);
    }

    SQWORD sqAns = 0;
    for (SQWORD sqBaseFish = 0; sqBaseFish < sqN; sqBaseFish++) {
//        printf("--------- base [%d]\n", sqBaseFish);
        vector<ST_INTERSECT> vIntersect;
        const ST_FISH &fBase =  vstFish[sqBaseFish];
        for (SQWORD sqCurFish = 0; sqCurFish < sqN; sqCurFish++) {
            if (sqBaseFish != sqCurFish) {
                DOUBLE t1, t2;

                const ST_FISH &fTgt =  vstFish[sqCurFish];

                if (getIntersectTime(fBase, fTgt, sqA, t1, t2)) {
                    vIntersect.emplace_back(t1, fTgt.sqW);
                    vIntersect.emplace_back(t2, -fTgt.sqW);
                }
            }
        }
        sort(vIntersect.begin(), vIntersect.end());
        SQWORD sqScore = fBase.sqW;
        SQWORD sqMaxSxore = sqScore;
//        printf(">>> %lld\n", sqScore);
        for (auto p: vIntersect) {
            sqScore += p.sqW;
//            printf("%f %lld %lld\n", p.dT, p.sqW, sqMaxSxore);
            sqMaxSxore = MAX(sqScore, sqMaxSxore);
        }
        sqAns = MAX(sqMaxSxore, sqAns);
    }
    printf("%lld\n", sqAns);

    return 0;
}
