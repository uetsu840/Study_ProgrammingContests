#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using BYTE = unsigned char;
using SBYTE = char;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MAX_QWORD (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFF)
#define MAX_WORD  (0xFFFF)
#define MAX_BYTE  (0xFF)


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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

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

static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber;
            }
        }
    }
}


static void printb(QWORD qwVal) {
  QWORD mask = (QWORD)1 << (sizeof(qwVal) * BYTE_BITS - 1);
  do {
      if (mask & qwVal) {
          putchar('*');
      } else {
          putchar('.');
      }
  } while (mask >>= 1);
  printf("\n");
}

#define MAX_HEIGHT  (50)
#define MAX_WIDTH   (50)
#define INF         (MAX_SDWORD)

static QWORD s_aqwMap[MAX_HEIGHT + 2];
static DWORD s_adwDist[MAX_HEIGHT + 2][MAX_WIDTH + 2];

typedef pair<DWORD, DWORD> Pos;

static const SDWORD s_alDX[4] = {1, 0, -1, 0};
static const SDWORD s_alDY[4] = {0, 1, 0, -1};

static M_BOOL isMovable(DWORD dwRow, DWORD dwCol)
{
    QWORD qwMask = 1ULL << dwCol;
    if (!(s_aqwMap[dwRow] & qwMask)) {
        return M_TRUE;
    }

    return M_FALSE;
}

static DWORD bfs(DWORD dwStartRow, DWORD dwStartCol, DWORD dwGoalRow, DWORD dwGoalCol)
{
    queue<Pos> que;

    /* initialize distance */
    for (DWORD dwRow = 0; dwRow <= MAX_HEIGHT+1; dwRow++) {
        for (DWORD dwCol = 0; dwCol <= MAX_WIDTH+1; dwCol++) {
            s_adwDist[dwRow][dwCol] = INF;
        }
    }
    
    /* Initialize Start point */
    que.push(Pos(dwStartRow, dwStartCol));
    s_adwDist[1][1] = 0;

    /* loop while que is not empty */
    while (que.size()) {
        /* get top of queue */
        Pos p = que.front();
        que.pop();

        /* if reached to goal, stop loop */
        if ((p.first == dwGoalRow) && (p.second == dwGoalCol)) {
            break;
        }

        DWORD dwNextDist = s_adwDist[p.first][p.second] + 1;
        for (DWORD dwDirIdx = 0; dwDirIdx < 4; dwDirIdx++) {
            DWORD dwNextX = (DWORD)((SDWORD)p.first  + s_alDX[dwDirIdx]);
            DWORD dwNextY = (DWORD)((SDWORD)p.second + s_alDY[dwDirIdx]);
            
            if ((INF == s_adwDist[dwNextX][dwNextY])
                && isMovable(dwNextX, dwNextY)) {
                que.push(Pos(dwNextX, dwNextY));
                s_adwDist[dwNextX][dwNextY] = dwNextDist;
            }
        }
    }
    return s_adwDist[dwGoalRow][dwGoalCol];
}

int main()
{
    char acMap[MAX_WIDTH + 3];
    DWORD dwInput_H;
    DWORD dwInput_W;
    DWORD dwRoomCnt = 0;

    dwInput_H = inputSDWORD();
    dwInput_W = inputSDWORD();
    s_aqwMap[0] = MAX_QWORD;
    for (DWORD dwRow=1; dwRow<=dwInput_H; dwRow++) {
        inputString(&(acMap[1]));
        s_aqwMap[dwRow] = MAX_QWORD;
        for (DWORD dwCol = 1; dwCol <= dwInput_W; dwCol++) {
            if ('.' == acMap[dwCol]) {
                s_aqwMap[dwRow] &= (~(1ULL << dwCol));
                dwRoomCnt++;
            }
        }
    }
    s_aqwMap[dwInput_H + 1] = MAX_QWORD;

    DWORD dwDist = bfs(1, 1, dwInput_H, dwInput_W);
    if (INF == dwDist) {
        printf("-1");
    } else {
        printf("%d\n", dwRoomCnt - (dwDist + 1));
    } 
    return 0;
}
