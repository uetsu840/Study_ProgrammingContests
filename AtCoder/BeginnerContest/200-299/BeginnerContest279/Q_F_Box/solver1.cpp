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

typedef struct {
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    DWORD   dwSize;

    void initUnionFind(
        DWORD dwSize)
    {
        dwSize = dwSize;
        vdwPar.resize(dwSize);
        vdwRank.resize(dwSize);
        vdwCnt.resize(dwSize);
    
        for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vdwPar[dwIdx]  = dwIdx;
            vdwRank[dwIdx] = 0;
            vdwCnt[dwIdx]  = 1;
        }
    }

    DWORD ufGetCnt(DWORD sqIdx) {
        return vdwCnt[ufGetRoot(sqIdx)];
    }


    DWORD ufGetParent(DWORD dwIdx) const
    {
        return vdwPar[dwIdx];
    }

    DWORD ufGetRank(DWORD dwIdx) const
    {
        return vdwRank[dwIdx];
    }

    void ufSetParent(DWORD dwIdx, DWORD dwParent)
    {
        vdwPar[dwIdx] = dwParent; 
        if (ufGetRank(dwIdx) == ufGetRank(dwParent)) {
            (vdwRank[dwParent])++;
        }
    }

    DWORD ufGetRoot(DWORD dwIdx) const
    {
        if (ufGetParent(dwIdx) == dwIdx) {
            return dwIdx;
        } else {
            DWORD dwParent = ufGetParent(dwIdx);
            DWORD dwRoot = ufGetRoot(dwParent);
            return dwRoot;
        }
    }

    bool ufUnite(DWORD dwX, DWORD dwY)
    {
        DWORD dwRootX = ufGetRoot(dwX);
        DWORD dwRootY = ufGetRoot(dwY);

        if (dwRootX == dwRootY) {
            return false;
        }

        if (ufGetRank(dwRootX) < ufGetRank(dwRootY)) {
            ufSetParent(dwRootX, dwRootY);
            (vdwCnt[dwRootY]) += (vdwCnt[dwRootX]);
        } else {
            ufSetParent(dwRootY, dwRootX);
            (vdwCnt[dwRootX]) += (vdwCnt[dwRootY]);
        }

        return true;
    }

    bool ufIsSame(DWORD dwX, DWORD dwY) const
    {
        return (ufGetRoot(dwX)  == ufGetRoot(dwY));
    }
} ST_UNION_FIND;

/*----------------------------------------------------------------------*/


#define BOX_NULL    ((SQWORD)100100100100100100)
#define BALL_NULL   ((SQWORD)100100100100100102)

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    ST_UNION_FIND uf;

    /* 箱は0はじまり、ボールは番号そのまま */

    uf.initUnionFind(sqQ + sqN + 1);
    vector<SQWORD> vsqParentBox(sqQ + sqN + 1, BOX_NULL);       /* (親の)ボールが入っている箱の番号 */
    vector<SQWORD> vsqBoxBallIdx(sqN, BOX_NULL);                /* k番目の箱に入っているボールの番号 */

    SQWORD sqK = 0;
    /* 初期化 */
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqBallNo = sqK + 1;
        vsqBoxBallIdx[sqIdx] = sqBallNo;
        vsqParentBox[sqBallNo] = sqIdx;
        sqK++;
    }

    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqOp = inputSQWORD();
        switch (sqOp) {
        case 1: 
            {
                SQWORD sqX = inputSQWORD() - 1;
                SQWORD sqY = inputSQWORD() - 1;

                SQWORD sqBallX = vsqBoxBallIdx[sqX];
                SQWORD sqBallY = vsqBoxBallIdx[sqY];
                if ((sqBallX != BALL_NULL)
                    && (sqBallY != BALL_NULL)) {
                    uf.ufUnite(sqBallX, sqBallY);
                    SQWORD sqRootBall = uf.ufGetRoot(sqBallY);
                    vsqParentBox[sqRootBall] = sqX;
                    vsqBoxBallIdx[sqY] = BALL_NULL;
                } else if (sqBallX != BALL_NULL) {
                    /* Yが空 */
                } else if (sqBallY != BALL_NULL) {
                    /* Xが空  YがXに移動する */
                    SQWORD sqRootBall = uf.ufGetRoot(sqBallY);
                    vsqParentBox[sqRootBall] = sqX;
                    vsqBoxBallIdx[sqX] = sqBallY;
                    vsqBoxBallIdx[sqY] = BALL_NULL;
                } else {
                    /* 両方とも空 */
                }
            }
            break;
        case 2:
            {
                SQWORD sqX = inputSQWORD() - 1;
                SQWORD sqBallPrev = vsqBoxBallIdx[sqX];
                SQWORD sqNewBall = ++sqK;
                if (sqBallPrev == BALL_NULL) {
                    vsqBoxBallIdx[sqX] = sqNewBall;
                    vsqParentBox[sqNewBall] = sqX;
                } else {
                    uf.ufUnite(sqBallPrev, sqNewBall);
                    SQWORD sqParentBall = uf.ufGetRoot(sqNewBall);
                    vsqParentBox[sqParentBall] = sqX;
                }
            }
            break;
        case 3:
            {
                SQWORD sqBall = inputSQWORD();
                SQWORD sqRoot = uf.ufGetRoot(sqBall);

                printf("%lld\n", vsqParentBox[sqRoot] + 1);
            }
            break;
        }
    }

    return 0;
}
