#pragma GCC optimize ("O3")

#include <iostream>
#include <cstddef>
#include <numeric>
#include <algorithm>
#include <vector>
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


#define MAX_CITY_NUM    (100000)

/* インデックス付きソート */
typedef std::pair<DWORD,DWORD> ipair;
bool lessPair(const ipair& l, const ipair& r){return l.second < r.second;}
bool greaterPair(const ipair& l, const ipair& r){return l.second > r.second;}

static void sort_with_idx(vector<ipair> &y)
{
    sort(y.begin(), y.end(), lessPair);
}

#define DIST_INF (MAX_DWORD)

typedef pair<DWORD, DWORD> dist_info;

typedef priority_queue<dist_info, vector<dist_info>,
                         std::greater<dist_info> > pqDistInfo;

static M_BOOL abIsUsed[MAX_CITY_NUM];
static pqDistInfo mincost;
static vector<ipair> xpos(MAX_CITY_NUM);
static vector<ipair> ypos(MAX_CITY_NUM);
static DWORD adwIdxListX[MAX_CITY_NUM];
static DWORD adwIdxListY[MAX_CITY_NUM];

static void add_neighbor_dist(
    vector<ipair> &sorted_pos, 
    DWORD *pdwIdxList, 
    DWORD dwIdx)
{
    DWORD dwVectorIdx = *(pdwIdxList + dwIdx);

    if (0 == dwVectorIdx) {
        DWORD dwNodeIdx = sorted_pos[dwVectorIdx + 1].first;
        if (!abIsUsed[dwNodeIdx]) {
            DWORD dwDist = sorted_pos[dwVectorIdx + 1].second
                             - sorted_pos[dwVectorIdx].second;
            mincost.push(dist_info(dwDist, dwNodeIdx));
        }
    } else if (sorted_pos.size() - 1 == dwVectorIdx) {
        DWORD dwNodeIdx = sorted_pos[dwVectorIdx - 1].first;
        if (!abIsUsed[dwNodeIdx]) {
            DWORD dwDist = sorted_pos[dwVectorIdx].second
                            - sorted_pos[dwVectorIdx - 1].second;
            mincost.push(dist_info(dwDist, dwNodeIdx));
        }
    } else {
        DWORD dwNodeIdx1 = sorted_pos[dwVectorIdx + 1].first;
        DWORD dwNodeIdx2 = sorted_pos[dwVectorIdx - 1].first;
        if (!abIsUsed[dwNodeIdx1]) {
            DWORD dwDist1 = sorted_pos[dwVectorIdx + 1].second
                             - sorted_pos[dwVectorIdx].second;
            mincost.push(dist_info(dwDist1, dwNodeIdx1));
        }
        if (!abIsUsed[dwNodeIdx2]) {
            DWORD dwDist2 = sorted_pos[dwVectorIdx].second
                             - sorted_pos[dwVectorIdx - 1].second;
            mincost.push(dist_info(dwDist2, dwNodeIdx2));
        }
    }
}

static void add_node_to_network(
    DWORD dwNodeIdx)
{
    abIsUsed[dwNodeIdx] = M_TRUE;
    add_neighbor_dist(xpos, adwIdxListX, dwNodeIdx);
    add_neighbor_dist(ypos, adwIdxListY, dwNodeIdx);
}

int main()
{
    DWORD dwInput_N;

    dwInput_N = inputSDWORD();


    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        DWORD dwInput_x = inputSDWORD();
        DWORD dwInput_y = inputSDWORD(); 
        xpos[dwIdx] = ipair(dwIdx, dwInput_x);
        ypos[dwIdx] = ipair(dwIdx, dwInput_y);
    }
    xpos.resize(dwInput_N);
    ypos.resize(dwInput_N);
    sort_with_idx(xpos);
    sort_with_idx(ypos);

    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        adwIdxListX[xpos[dwIdx].first] = dwIdx; 
        adwIdxListY[ypos[dwIdx].first] = dwIdx;
    }

    /* Initialize */
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(abIsUsed); dwIdx++) {
        abIsUsed[dwIdx] = M_FALSE;
    }

    /* 頂点0だけを登録 */
    add_node_to_network(0);

    DWORD dwTotalCost = 0;
    DWORD dwTotalNodeNum = 1;

    while (1) {     
        dist_info new_node = mincost.top();
        mincost.pop();

        if (!abIsUsed[new_node.second]) {
            add_node_to_network(new_node.second);
            dwTotalCost += new_node.first;
            dwTotalNodeNum++;
            if (dwTotalNodeNum == dwInput_N) {
                break;
            }
        }
    }
    printf("%d\n", dwTotalCost);
}
