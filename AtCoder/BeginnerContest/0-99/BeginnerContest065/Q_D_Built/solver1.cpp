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
#define CITY_ARY_LEN    (MAX_CITY_NUM)
static DWORD s_adwCityPosX[CITY_ARY_LEN];
static DWORD s_adwCityPosY[CITY_ARY_LEN];

/* インデックス付きソート */
typedef std::pair<DWORD,DWORD> ipair;
bool lessPair(const ipair& l, const ipair& r){return l.second < r.second;}
bool greaterPair(const ipair& l, const ipair& r){return l.second > r.second;}

static void sort_with_idx(vector<ipair> &y)
{
    sort(y.begin(), y.end(), lessPair);
}

#define DIST_INF (MAX_DWORD)


// ">" のオーバーロード numを基準に大小比較を行う
class city_info {
public:
    DWORD dwDist;
    DWORD dwIdx;
    city_info(DWORD dwDist, DWORD dwIdx) {
        this->dwDist = dwDist;
        this->dwIdx = dwIdx;
    }
};

bool operator> (const city_info &city1, const city_info &city2){
    return city1.dwDist > city2.dwDist;
};

typedef priority_queue<city_info, vector<city_info>,
                         std::greater<city_info> > pqCityInfo;

/* priority queue に (dist, idx) を放り込む */
static void gen_mincost_queue(vector<ipair> &pos, pqCityInfo &dist)
{
    DWORD dwPrevPos = 0;
    DWORD dwDist;
    M_BOOL  bFirst = M_TRUE;
    for (vector<ipair>::iterator it = pos.begin(); it != pos.end(); ++it) {
        if (bFirst) {
            dwDist = DIST_INF;
            bFirst = M_FALSE;
        } else {
            dwDist = it->second - dwPrevPos;
        }
        dwPrevPos = it->second;
        printf("push priority queue : [%d %d]\n", dwDist, it->first);
        dist.push(city_info(dwDist, it->first));
    }
}

int main()
{
    DWORD dwInput_N;

    dwInput_N = inputSDWORD();

    using namespace std;

    //インデックスと共にペアに突っ込む
    std::vector<ipair> xpos;
    std::vector<ipair> ypos;

    for (DWORD dwIdx = 0; dwIdx < dwInput_N; dwIdx++) {
        DWORD dwInput_x = inputSDWORD();
        DWORD dwInput_y = inputSDWORD(); 
        xpos.push_back(ipair(dwIdx, dwInput_x));
        ypos.push_back(ipair(dwIdx, dwInput_y));
    }
    sort_with_idx(xpos);
    sort_with_idx(ypos);

    pqCityInfo x_dist_list;
    pqCityInfo y_dist_list;

    gen_mincost_queue(xpos, x_dist_list);
    gen_mincost_queue(ypos, y_dist_list);

    /* Initialize */
    M_BOOL abIsUsed[MAX_CITY_NUM];
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(abIsUsed); dwIdx++) {
        abIsUsed[dwIdx] = M_FALSE;
    }

    while(1) {
        /* search minimum cost node */
        city_info y_nearest(0, 0);
        y_nearest = y_dist_list.top();
        printf("y: %d %d\n", y_nearest.dwDist, y_nearest.dwIdx);
        y_dist_list.pop();

        if (y_dist_list.empty()) {
            break;
        }
    }

#if 1   
    printf("-------------\n");
    while (1) {
        city_info x_nearest(0, 0);
        x_nearest = x_dist_list.top();
        printf("x: %d %d\n", x_nearest.dwDist, x_nearest.dwIdx);
        x_dist_list.pop();
        if (x_dist_list.empty()) {
            break;
        }
    }
#endif


    printf("\n");
}
