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

/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqVal;

public:
    static SQWORD getInvalidVal()
    {
        return 0;
    }
    static SQWORD uniteVal(SQWORD sqValA, SQWORD sqValB)
    {
        return max(sqValA, sqValB);
    }
    void init() {
        sqVal = getInvalidVal();
    }
    void debugPrint() {
        printf("%lld ", sqVal);
    }
};

struct SegmentTree {
private:
    DWORD dwBaseSize;
    vector<SEGMENT_NODE_ST> vNode;

private:
    void debugPrint(void)
    {
        printf("====\n");
        for (SDWORD lIdx = 0; lIdx < vNode.size(); lIdx++) {
            printf(" [%d] ", lIdx);
            vNode[lIdx].debugPrint();
            printf("\n");
        }
    }

    static inline SEGMENT_NODE_ST updateNode(
        const SEGMENT_NODE_ST &stL,
        const SEGMENT_NODE_ST &stR)
    {
        SEGMENT_NODE_ST stRet;
        
        stRet.sqVal = SEGMENT_NODE_ST::uniteVal(stR.sqVal, stL.sqVal);
        return stRet;
    }

    void initSegmentTree(vector<SEGMENT_NODE_ST> &v) {
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
        /* 最下段 */
        for (SDWORD lIdx = 0; lIdx < dwSize; lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)] = v[lIdx];
        }
        for(SDWORD lIdx = dwSize; lIdx < dwBaseSize; lIdx++) {
            vNode[lIdx + (dwBaseSize - 1)].init();
        }
        /* 2段目以降 */
        for (SDWORD lIdx = dwBaseSize - 2; 0 <= lIdx; lIdx--) {
            vNode[lIdx] = updateNode(vNode[lIdx*2+1], vNode[lIdx*2+2]);
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
            vNode[lTreeIdx] = updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2]);
        }
    }

    SQWORD GetMax(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return SEGMENT_NODE_ST::getInvalidVal();
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx].sqVal;
        }

        SQWORD sqResL, sqResR;
        SQWORD lCenter = (lLeft + lRight) / 2;
        sqResL = GetMax(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter);
        sqResR = GetMax(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight);
        return SEGMENT_NODE_ST::uniteVal(sqResL, sqResR);
    }
};


#define MAX_VAL_X   (200000)

struct BOOK {
    SQWORD sqNo;
    SQWORD sqPos;
    SQWORD sqWeight;
};

bool operator< (const BOOK &a, const BOOK &b)
{
    return a.sqPos < b.sqPos;
}

int main()
{
    SQWORD sqInput_N = inputSQWORD();

    vector<BOOK> vecBook(sqInput_N);
    SQWORD sqWeightSum = 0;

    for (SQWORD sqBookIdx = 0; sqBookIdx < sqInput_N; sqBookIdx++) {
        SQWORD sqWeight = inputSQWORD();
        vecBook[sqBookIdx].sqNo = sqBookIdx + 1;
        vecBook[sqBookIdx].sqWeight = sqWeight;
        sqWeightSum += sqWeight;
    }
    for (SQWORD sqPosIdx = 0; sqPosIdx < sqInput_N; sqPosIdx++) {
        SQWORD sqNo = inputSQWORD();
        vecBook[sqNo - 1].sqPos = sqPosIdx;
    }
    sort(vecBook.begin(), vecBook.end());

    vector<SEGMENT_NODE_ST> vec(sqInput_N);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        it->sqVal = 0;
    }
    SegmentTree seg(vec);

    for (SQWORD sqIdx = 0; sqIdx < vecBook.size(); sqIdx++) {
        auto book = vecBook.at(sqIdx);
        SQWORD sqBookIdx = book.sqNo - 1;

        SQWORD sqMaxWeight = seg.GetMax(0, sqBookIdx);
        SEGMENT_NODE_ST stNewVal;
        stNewVal.sqVal = sqMaxWeight + book.sqWeight;
        seg.update(sqBookIdx, stNewVal);
    }
    SQWORD sqMaxWeight = seg.GetMax(0, sqInput_N);
    printf("%lld\n", (sqWeightSum - sqMaxWeight) * 2);
    return 0;
}