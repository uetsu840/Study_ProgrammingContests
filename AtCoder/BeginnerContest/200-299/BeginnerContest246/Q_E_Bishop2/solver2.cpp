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

/*-----------------------------------------------------*/

typedef enum {
    BOARD_VACANT = 0,
    BOARD_PAWN
} BOARD_STATE;

struct BOARD_POS {
    SQWORD sqRow;
    SQWORD sqCol;

    BOARD_POS(SQWORD x, SQWORD y) : sqRow(x), sqCol(y) {};
};

bool operator< (const BOARD_POS &a, const BOARD_POS &b) {
    return a.sqCol < b.sqCol;
}

#define MAX_N   (1500)
#define MAX_COST (100100100100100100)
/*----------------------------------------------*/
int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqAx = inputSQWORD();
    SQWORD sqAy = inputSQWORD();
    SQWORD sqBx = inputSQWORD();
    SQWORD sqBy = inputSQWORD();

    /* 0-indexed */
    sqAx--;
    sqAy--;
    sqBx--;
    sqBy--;

    static BOARD_STATE s_aaeBoard[MAX_N][MAX_N];

    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        string strS;
        cin >> strS;
        for (SQWORD sqCol = 0; sqCol < sqN; sqCol++) {
            if (strS[sqCol] == '.') {
                s_aaeBoard[sqRow][sqCol] = BOARD_VACANT;             
            } else {
                s_aaeBoard[sqRow][sqCol] = BOARD_PAWN;
            }
        }
    }

    /* 位相が違う */
    if (0 < (sqAx + sqAy + sqBx + sqBy) % 2) {
        printf("-1\n");
        return 0;
    }

    static SQWORD s_aasqCost[MAX_N][MAX_N];
    s_aasqCost[sqAx][sqAy] = 0;

    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        for (SQWORD sqCol = 0; sqCol < sqN; sqCol++) {
            s_aasqCost[sqRow][sqCol] = MAX_COST;
        }
    }

    queue<BOARD_POS> que;
    que.push(BOARD_POS(sqAx, sqAy));
    s_aasqCost[sqAx][sqAy] = 0;
    static vector<pair<SQWORD, SQWORD>> moveDir = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
    while (!que.empty()) {
        BOARD_POS v = que.front();
        que.pop();

        SQWORD sqCurCost = s_aasqCost[v.sqRow][v.sqCol];

        for (auto m: moveDir) {
            SQWORD sqDr = m.first;
            SQWORD sqDc = m.second;

            SQWORD sqOfs = 1;
            while (1) {
                SQWORD sqNextRow = v.sqRow + sqOfs * sqDr;
                SQWORD sqNextCol = v.sqCol + sqOfs * sqDc;
                if ((sqNextRow < 0) || (sqN <= sqNextRow)) {
                    break;
                }
                if ((sqNextCol < 0) || (sqN <= sqNextCol)) {
                    break;
                }
                if (s_aaeBoard[sqNextRow][sqNextCol] == BOARD_PAWN) {
                    break;
                }
                if (s_aasqCost[sqNextRow][sqNextCol] == MAX_COST) {
                    s_aasqCost[sqNextRow][sqNextCol] = sqCurCost + 1;
                    que.push(BOARD_POS{sqNextRow, sqNextCol});
                }
                sqOfs++;
            }
        }
        if (s_aasqCost[sqBx][sqBy] != MAX_COST) {
            break;
        }
    }

    if (MAX_COST == s_aasqCost[sqBx][sqBy]) {
        printf("-1\n");
    } else {
        printf("%lld\n", s_aasqCost[sqBx][sqBy]);
    }


    return 0;
}
