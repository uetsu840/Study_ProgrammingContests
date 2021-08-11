#pragma GCC optimize ("O3")

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
using namespace std;

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

static inline SQWORD ABS(SQWORD a) { return 0 < a ? a : -a; }

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

struct ST_TIMETABLE {
    SQWORD sqDst;
    SQWORD sqC;
    SQWORD sqD;

    ST_TIMETABLE(SQWORD dst, SQWORD c, SQWORD d): sqDst(dst), sqC(c), sqD(d) {};
};

struct ST_CITY_ARRIVAL_INFO {
    SQWORD sqCityIdx;
    SQWORD sqTime;
};

bool operator> (const ST_CITY_ARRIVAL_INFO &a, const ST_CITY_ARRIVAL_INFO &b)
{
    return a.sqTime > b.sqTime;
}



/*----------------------------------------------*/

struct PROBLEM_ONE {
    SQWORD sqC;
    SQWORD sqD;
};

static bool isStartTimeThresholdGreater(SQWORD sqTime, const PROBLEM_ONE &stProb)
{
    bool bStart = false;
    if ((stProb.sqD / (sqTime + 1)) <= 1 + (stProb.sqD / (sqTime + 2))) {
        bStart = true;
    }
#if 0
    printf("try : %lld [%lld %lld] res: %lld\n", sqTime, (stProb.sqD / (sqTime + 1)), 
                                                (stProb.sqD / (sqTime + 2)), bStart);
#endif
    return bStart;
}

static SQWORD binarySearch(
    bool (*pfJudge)(SQWORD, const PROBLEM_ONE&),
    SQWORD sqInitLower, 
    SQWORD sqInitUpper, 
    const PROBLEM_ONE &stParam)
{
    SQWORD sqNg = sqInitLower;
    SQWORD sqOk = sqInitUpper;

    while (1 < sqOk - sqNg) {
//        printf("%lld %lld\n", sqOk, sqNg);
        SQWORD sqMid = (sqNg + sqOk) / 2LL;
        if (pfJudge(sqMid, stParam)) {
            sqOk = sqMid;
        } else {
            sqNg = sqMid;
        }
    }
    return sqOk;
}


inline SQWORD getArrivalTime(SQWORD startTime, SQWORD sqC, SQWORD sqD) 
{
    PROBLEM_ONE stProb;
    stProb.sqC = sqC;
    stProb.sqD = sqD;

    SQWORD sqStartTime = binarySearch(isStartTimeThresholdGreater, -1, MAX_SQWORD - 10, stProb);
//    printf("STRT:%lld : %lld\n", sqStartTime, startTime);
    if (sqStartTime <= startTime) {
        sqStartTime = startTime;
    }
    return sqStartTime + sqC + (sqD / (sqStartTime + 1));
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();

    vector<vector<ST_TIMETABLE>> vstTimeTbl(sqN);

    for (SQWORD sqIdx = 0; sqIdx < sqM; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();
        SQWORD sqC = inputSQWORD();
        SQWORD sqD = inputSQWORD();

        sqA--;
        sqB--;

        vstTimeTbl[sqA].emplace_back(sqB, sqC, sqD);
        vstTimeTbl[sqB].emplace_back(sqA, sqC, sqD);
    }

    priority_queue<ST_CITY_ARRIVAL_INFO, 
                    vector<ST_CITY_ARRIVAL_INFO>,
                    greater<ST_CITY_ARRIVAL_INFO>> pqArrivalInfo;
    vector<SQWORD> vsqMinArriveTime(sqN, MAX_SQWORD);

    pqArrivalInfo.push(ST_CITY_ARRIVAL_INFO{0, 0});
    while (!pqArrivalInfo.empty()){
        ST_CITY_ARRIVAL_INFO stCur = pqArrivalInfo.top();
        pqArrivalInfo.pop();

        if (vsqMinArriveTime[stCur.sqCityIdx] == MAX_SQWORD) {
            vsqMinArriveTime[stCur.sqCityIdx] = stCur.sqTime;
            for (auto d: vstTimeTbl[stCur.sqCityIdx]) {
                SQWORD sqStartTime = stCur.sqTime;
                SQWORD sqArriveTime = getArrivalTime(stCur.sqTime, d.sqC, d.sqD);
//                printf("------%lld > %lld\n", sqStartTime, sqArriveTime);

                if (sqArriveTime < vsqMinArriveTime[d.sqDst]) {
                    pqArrivalInfo.push(ST_CITY_ARRIVAL_INFO{d.sqDst, sqArriveTime});
                }
            }
        }
    }

    if (MAX_SQWORD == vsqMinArriveTime[sqN - 1]) {
        printf("-1\n");
    } else {
        printf("%lld\n", vsqMinArriveTime[sqN - 1]);
    }
    return 0;
}
