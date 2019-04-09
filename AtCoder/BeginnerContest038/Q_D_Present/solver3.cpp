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


static DWORD cntDiffChar(char *pA, char *pB, DWORD dwLen)
{
    DWORD dwDiffCnt = 0;
    for (DWORD dwIdx = 0; dwIdx < dwLen; dwIdx++) {
        if (*(pA + dwIdx) != *(pB + dwIdx)) {
            dwDiffCnt++;
        }
    }
    return dwDiffCnt;
}

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SDWORD lMax;

public:
    void init() {
        lMax = 0;
    }
};

struct SegmentTree {
private:
    DWORD dwBaseSize;
    vector<SEGMENT_NODE_ST> vNode;

private:
    static inline void updateNode(
        const SEGMENT_NODE_ST &stL,
        const SEGMENT_NODE_ST &stR,
        SEGMENT_NODE_ST &stRet)
    {
        stRet.lMax = max(stR.lMax, stL.lMax);
    }

    DWORD initSegmentTree(vector<SEGMENT_NODE_ST> &v) {
        /**
         *  最下段のノード数は元配列のサイズ以上になる最小の 2 冪 -> これを n とおく
         * セグメント木全体で必要なノード数は 2n-1 個である
         */
        DWORD dwSize = v.size();
        dwBaseSize = 1; 
        while (dwBaseSize < dwSize) {
            dwBaseSize *= 2;
        }
        vNode.resize(2 * dwBaseSize - 1);

        /**
         *  最下段に値を入れたあとに、下の段から順番に値を入れる
         * 値を入れるには、自分の子の 2 値を参照すれば良い
         */
        for(DWORD dwIdx = 0; dwIdx < vNode.size(); dwIdx++) {
            vNode[dwIdx].init();
        }
    }

public:
    /* 元配列 v をセグメント木で表現する */
    SegmentTree(vector<SEGMENT_NODE_ST> v) {
        initSegmentTree(v);
    }

    void update(DWORD dwNodeIdx, const SEGMENT_NODE_ST &stNewVal) {

        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);

        // 最下段のノードを更新したら、あとは親に上って更新していく
        vNode[lTreeIdx] = stNewVal;
        while(lTreeIdx > 0) {
            lTreeIdx = (lTreeIdx - 1) / 2;
            updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2], vNode[lTreeIdx]);
        }
    }

    SDWORD GetMax(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return 0;
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx].lMax;
        }

        SDWORD lRes = 0;
        SDWORD lCenter = (lLeft + lRight) / 2;
        lRes = max(lRes, GetMax(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter));
        lRes = max(lRes, GetMax(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight));
        return lRes;        
    }
};


struct ST_BOX {
    SDWORD lIdx;
    SDWORD lWidth;
    SDWORD lHeight;
};

bool cmp(ST_BOX a, ST_BOX b)
{
    if (a.lWidth == b.lWidth) {
        return (a.lHeight > b.lHeight);
    }
    return a.lWidth < b.lWidth;
}


#define HEIGHT_INF  (100000000)
#define MAX_N       (100000)
int main()
{
    SDWORD lInput_N;

    static vector<ST_BOX> vBoxes;

    lInput_N = inputSDWORD();

    for (SDWORD lIdx = 0; lIdx < lInput_N; lIdx++) {
        SDWORD lWidth = inputSDWORD();
        SDWORD lHeight = inputSDWORD();

        ST_BOX box;
        box.lIdx = lIdx;
        box.lHeight = lHeight;
        box.lWidth = lWidth;

        vBoxes.emplace_back(box);
    }

    sort(vBoxes.begin(), vBoxes.end(), cmp);

    vector<SEGMENT_NODE_ST> vMaxBoxNum(MAX_N);
    SegmentTree seg(vMaxBoxNum);

    SDWORD lAns = 0;
    for (SDWORD lIdx = 0; lIdx < lInput_N; lIdx++) {
        SDWORD lHeight = vBoxes[lIdx].lHeight;
        SDWORD lNextCnt = seg.GetMax(0, lHeight) + 1;

        lAns = max(lAns, lNextCnt);

        SEGMENT_NODE_ST stUpdateSegment;
        stUpdateSegment.lMax = lNextCnt;
        seg.update(lHeight, stUpdateSegment);
    }

    printf("%d\n", lAns);
    return 0;
}