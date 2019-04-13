#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <algorithm>    // std::binary_search, std::sort
#include <vector>       // std::vector
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

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0xFFFFFFFFFFFFFFFF)

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

#define MAX_ANT_NUM     (100000)
static SQWORD s_asqPathR[MAX_ANT_NUM];
static SQWORD s_asqPathL[MAX_ANT_NUM];
static SQWORD s_asqAntInitPos[MAX_ANT_NUM];
static SQWORD s_asqAntLastPos[MAX_ANT_NUM];
static DWORD s_dwPathNum_R;     /* W == 1 */
static DWORD s_dwPathNum_L;     /* W == 2 */
static DWORD s_dwPathRFirstIdx;

void solve(SQWORD sqInput_N, SQWORD sqInput_L, SQWORD sqInput_T) 
{
    SQWORD sqCycle = (2LL * sqInput_T + s_asqPathR[0]) / sqInput_L;
    SQWORD sqPos   = (2LL * sqInput_T + s_asqPathR[0]) % sqInput_L; 

    /* 右向き、パス0のパスLとの衝突の回数を求める */
    vector<SQWORD> path_l(s_asqPathL, s_asqPathL + s_dwPathNum_L);
    
    auto path_l_ofs = lower_bound(path_l.begin(), path_l.end(), s_asqPathR[0]);
    DWORD dwPathLColOfs = path_l_ofs - path_l.begin();

    auto path_l_col = lower_bound(path_l.begin(), path_l.end(), sqPos);
    DWORD dwPathLColIdx = path_l_col - path_l.begin();
    SQWORD sqPathColNum = (SQWORD)(dwPathLColIdx - dwPathLColOfs) + sqCycle * (SQWORD)s_dwPathNum_L;

    /* T秒後のパス0上には、Idx から dwAntsIdxOnPathR0 インデックスをオフセットした蟻がいる */
    DWORD dwAntsIdxOnPathR0 = ((SQWORD)s_dwPathRFirstIdx + sqPathColNum) % sqInput_N;
    DWORD dwPath0LastPos = (s_asqPathR[0] + sqInput_T) % sqInput_L;

    /* T秒後のすべての蟻の位置を求める。(インデックスは後付け) */
    for (DWORD dwIdx = 0; dwIdx < s_dwPathNum_R; dwIdx++) {
        SQWORD sqLastPos = (s_asqPathR[dwIdx] + sqInput_T) % sqInput_L;
        s_asqAntLastPos[dwIdx] = sqLastPos;
    }
    for (DWORD dwIdx = 0; dwIdx < s_dwPathNum_L; dwIdx++) {
        SQWORD sqLastPos = (s_asqPathL[dwIdx] - sqInput_T) % sqInput_L;
        if (sqLastPos < 0) {
            sqLastPos += sqInput_L;
        }
        s_asqAntLastPos[dwIdx + s_dwPathNum_R] = sqLastPos;
    }
    vector<SQWORD> ants_pos(s_asqAntLastPos, s_asqAntLastPos + sqInput_N);
    sort(ants_pos.begin(), ants_pos.end());
    auto path0_pos_lower = lower_bound(ants_pos.begin(), ants_pos.end(), (SQWORD)dwPath0LastPos);
    DWORD dwPath0Idx = path0_pos_lower - ants_pos.begin();

    /* 各蟻の位置をインデックスから出力 */
    for (DWORD dwAntIdx=0; dwAntIdx < sqInput_N; dwAntIdx++) {
        SQWORD sqAntsPosIdx = (((SQWORD)dwPath0Idx - (SQWORD)dwAntsIdxOnPathR0) + dwAntIdx) % sqInput_N;
        if (sqAntsPosIdx < 0) {
            sqAntsPosIdx += sqInput_N;
        }
        SQWORD sqAntPos = ants_pos[sqAntsPosIdx];
        printf("%d\n", sqAntPos);
    }
}

int main()
{
    DWORD dwInput_N;
    DWORD dwInput_L;
    DWORD dwInput_T;

    dwInput_N = inputSDWORD();
    dwInput_L = inputSDWORD();
    dwInput_T = inputSDWORD();

    DWORD dwIdx_R = 0;
    DWORD dwIdx_L = 0;

    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        DWORD dwInput_X = inputSDWORD();
        DWORD dwInput_W = inputSDWORD();

        s_asqAntInitPos[dwIdx] = dwInput_X;
        if (1 == dwInput_W) {
            if (0 == dwIdx_R) {
                s_dwPathRFirstIdx = dwIdx;
            }
            s_asqPathR[dwIdx_R] = dwInput_X;
            dwIdx_R++;
        } else if (2 == dwInput_W) {
            s_asqPathL[dwIdx_L] = dwInput_X;
            dwIdx_L++;
        }
    }
    s_dwPathNum_R = dwIdx_R;
    s_dwPathNum_L = dwIdx_L;

    solve(dwInput_N, dwInput_L, dwInput_T);

    return 0;
}
