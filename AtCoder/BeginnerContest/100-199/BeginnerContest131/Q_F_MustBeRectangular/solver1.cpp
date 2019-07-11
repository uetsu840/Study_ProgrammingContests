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


#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)
#define DIVISOR (1000000007)

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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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
    M_BOOL bRead = M_FALSE;
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
            bRead = M_TRUE;
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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/
#define MAX_COORDINATE  (100000)
#define N_MAX_POINTS    (100000)

struct POINT {
    SQWORD sqX;
    SQWORD sqY;
    SQWORD sqIdx;
};

static bool s_abIsVisited[N_MAX_POINTS];

static SQWORD SearchGroupPointsWithBfs(
    const vector<POINT> &vecPoints,
    const vector<SQWORD> *pvecsqRelationX,
    const vector<SQWORD> *pvecsqRelationY,
    set<SQWORD> &setCoordX,
    set<SQWORD> &setCoordY,
    SQWORD sqStartPntIdx)
{
    POINT stPntCur = vecPoints[sqStartPntIdx];
    s_abIsVisited[sqStartPntIdx] = true;
    SQWORD sqNum;
    SQWORD sqRet = 1;
    for (auto next_pnt_x: pvecsqRelationX[stPntCur.sqX]) {
        /* X座標が共通 */
        if (!s_abIsVisited[next_pnt_x]) {
//            printf("X: %lld -> %lld\n", stPntCur.sqIdx, next_pnt_x);
            setCoordY.insert(vecPoints[next_pnt_x].sqY);
            sqRet += SearchGroupPointsWithBfs(vecPoints, 
                                                pvecsqRelationX, 
                                                pvecsqRelationY, 
                                                setCoordX, 
                                                setCoordY, 
                                                next_pnt_x);
        }
    }
    for (auto next_pnt_y: pvecsqRelationY[stPntCur.sqY]) {
        /* Y座標が共通 */
        if (!s_abIsVisited[next_pnt_y]) {
//            printf("Y: %lld -> %lld\n", stPntCur.sqIdx, next_pnt_y);
            setCoordX.insert(vecPoints[next_pnt_y].sqX);
            sqRet += SearchGroupPointsWithBfs(vecPoints, 
                                                pvecsqRelationX, 
                                                pvecsqRelationY, 
                                                setCoordX, 
                                                setCoordY, 
                                                next_pnt_y);
        }
    }
    return sqRet;
}

int main(void)
{
    SQWORD sqInput_N = inputSQWORD();

    vector<POINT> vecPoints;

    static vector<SQWORD> s_asqPntsX[MAX_COORDINATE + 1];
    static vector<SQWORD> s_asqPntsY[MAX_COORDINATE + 1];

    for (SQWORD sqIdx = 0; sqIdx < sqInput_N; sqIdx++) {
        SQWORD sqInput_x = inputSQWORD();
        SQWORD sqInput_y = inputSQWORD();

        POINT pnt;
        pnt.sqX = sqInput_x;
        pnt.sqY = sqInput_y;
        pnt.sqIdx = sqIdx;
        vecPoints.emplace_back(pnt);
        s_asqPntsX[sqInput_x].emplace_back(pnt.sqIdx);
        s_asqPntsY[sqInput_y].emplace_back(pnt.sqIdx);
    }

    for (SDWORD lIdx = 0; lIdx < ArrayLength(s_abIsVisited); lIdx++) {
        s_abIsVisited[lIdx] = false;
    }

    SQWORD sqAns = 0;
    for (SQWORD sqPntIdx = 0; sqPntIdx < sqInput_N; sqPntIdx++) {
        if (!s_abIsVisited[sqPntIdx]) {
            set<SQWORD> setCoordX;
            set<SQWORD> setCoordY;
            setCoordX.insert(vecPoints[sqPntIdx].sqX);
            setCoordY.insert(vecPoints[sqPntIdx].sqY);
            SQWORD sqPntNum = SearchGroupPointsWithBfs(vecPoints, s_asqPntsX, s_asqPntsY, 
                                                        setCoordX, setCoordY, sqPntIdx); 
//            printf("%lld -> <%lld> %lld %lld\n", sqPntIdx, sqPntNum, setCoordX.size(), setCoordY.size());
            sqAns += setCoordX.size() * setCoordY.size() - sqPntNum;
        }
    }
    printf("%lld\n", sqAns);
}