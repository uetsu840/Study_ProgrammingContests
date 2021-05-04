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
#include <time.h>       /* time */
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

#define TILE_WIDTH  (50)
#define TILE_HEIGHT (50)   

int main(void)
{
    SQWORD Sx = inputSQWORD() + 1;  /* 1-indexed */
    SQWORD Sy = inputSQWORD() + 1;  /* 1-indexed */

    static SQWORD s_aasqTileNo[TILE_WIDTH + 2][TILE_HEIGHT + 2];
    static SQWORD s_aasqTilePnts[TILE_WIDTH + 2][TILE_HEIGHT + 2];
    static SQWORD s_asqTileSize[TILE_WIDTH * TILE_HEIGHT + 1];

    /* タイル番号0は壁を示す */
    for (SQWORD sqRow = 1; sqRow <= TILE_HEIGHT; sqRow++) {
        for (SQWORD sqCol = 1; sqCol <= TILE_WIDTH; sqCol++) {
            SQWORD sqNo = inputSQWORD();
            s_aasqTileNo[sqRow][sqCol] = sqNo + 1;
            s_asqTileSize[sqNo + 1]++;
        }
    }

    for (SQWORD sqRow = 1; sqRow <= TILE_HEIGHT; sqRow++) {
        for (SQWORD sqCol = 1; sqCol <= TILE_WIDTH; sqCol++) {
            SQWORD sqPnt = inputSQWORD();
            s_aasqTilePnts[sqRow][sqCol] = sqPnt;
        }
    }

    static bool s_aasqIsTileVisited[TILE_WIDTH * TILE_HEIGHT + 1];
    typedef struct {
        SQWORD x;
        SQWORD y;
    } POSITION;
    static POSITION s_aasqDir[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 

    string strDir = "UDLR";
    string strAns;
    SQWORD sqMaxScore = 0;
    SQWORD sqRandomSeed = 0;
    for (SQWORD sqTry = 0; sqTry < 1; sqTry++) {
        SQWORD sqCurScore = 0;
        string strCurAns = "";
        memset(s_aasqIsTileVisited, 0, sizeof(s_aasqIsTileVisited));
        s_aasqIsTileVisited[0] = true;

        SQWORD sqCurX = Sx;
        SQWORD sqCurY = Sy;

        while(1) {
//            printf("%d %d\n", sqCurX, sqCurY);
            srand(sqRandomSeed++); 

            SQWORD sqCurTileNo = s_aasqTileNo[sqCurX][sqCurY];
            s_aasqIsTileVisited[sqCurTileNo] = true;
            sqCurScore += s_aasqTilePnts[sqCurX][sqCurY];

            bool bFound = false;
            for (SQWORD sqDirTry = 0; sqDirTry < 99; sqDirTry++) {
                SQWORD sqDirIdx = rand() % 4;
                SQWORD sqNextX = sqCurX + s_aasqDir[sqDirIdx].x;
                SQWORD sqNextY = sqCurY + s_aasqDir[sqDirIdx].y;

                SQWORD sqNextTileNo = s_aasqTileNo[sqNextX][sqNextY];

                if (!s_aasqIsTileVisited[sqNextTileNo]) {
                    if ((sqDirTry < 10) && (s_aasqTilePnts[sqNextX][sqNextY] < 80)) {
                        ;   /* skip */
                    } else if ((sqDirTry < 10) && (1 < s_asqTileSize[sqNextTileNo])) {
                        ;   /* skip */
                    } else if ((sqDirTry < 50) && (s_aasqTilePnts[sqNextX][sqNextY] < 50)) {
                        ;   /* skip */
                    } else {
                        strCurAns += strDir[sqDirIdx];
                        sqCurX = sqNextX;
                        sqCurY = sqNextY;
                        bFound = true;
                        break;
                    
                    }
                }
            }
            if (!bFound) {
                break;
            }     
        }
//        printf("--- %s %lld\n", strCurAns.c_str(), sqCurScore);
        if (sqMaxScore < sqCurScore) {
            strAns = strCurAns;
            sqMaxScore = sqCurScore;
        }
    }
    printf("%s\n", strAns.c_str());
    return 0;
}
