#pragma GCC optimize ("O3")
//using namespace std;

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
#define SEGMENT_DEFAULT_VAL    (0)
/**
 *  セグメント木
 */
struct SEGMENT_NODE_ST {
    SQWORD sqX;

public:
    SEGMENT_NODE_ST(SQWORD x) : sqX(x) {};
    SEGMENT_NODE_ST() : sqX(SEGMENT_DEFAULT_VAL) {};
    static SEGMENT_NODE_ST getInvalidVal()
    {
        return SEGMENT_NODE_ST{SEGMENT_DEFAULT_VAL};
    }
    static SEGMENT_NODE_ST uniteNode(SEGMENT_NODE_ST nodeA,  SEGMENT_NODE_ST nodeB)
    {
        SEGMENT_NODE_ST stRet;

        stRet.sqX = ABS(nodeA.sqX) + ABS(nodeB.sqX);
        return stRet;
    }
    void init() {
        sqX = SEGMENT_DEFAULT_VAL;
    }
    void debugPrint() {
        printf("<%lld> ", sqX);
    }
};


template<typename T>
class SegmentTree {
private:
    DWORD dwBaseSize;
    std::vector<T> vNode;

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

    static inline T updateNode(
        const T &stL,
        const T &stR)
    {
        T stRet;
        
        stRet = T::uniteNode(stL, stR);
        return stRet;
    }

    void initSegmentTree(std::vector<T> &v) {
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
    SegmentTree(std::vector<T> v) {
        initSegmentTree(v);
    }

    T getNode(DWORD dwNodeIdx) {
        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);
        return vNode[lTreeIdx];
    }

    void update(DWORD dwNodeIdx, const T &stNewVal) {
        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);

        // 最下段のノードを更新したら、あとは親に上って更新していく
        vNode[lTreeIdx] = stNewVal;
        while(lTreeIdx > 0) {
            lTreeIdx = (lTreeIdx - 1) / 2;
            vNode[lTreeIdx] = updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2]);
        }
    }

    T GetValue(SDWORD lA, SDWORD lB, SDWORD lNodeIdx=0, SDWORD lLeft=0, SDWORD lRight=-1)
    {
        if (-1 == lRight) {
            lRight = dwBaseSize;
        }

        if ((lRight <= lA) || (lB <= lLeft)) {
            return T::getInvalidVal();
        }

        if ((lA <= lLeft) && (lRight <= lB)) {
            return vNode[lNodeIdx];
        }

        T sqResL, sqResR;
        SQWORD lCenter = (lLeft + lRight) / 2;
        sqResL = GetValue(lA, lB, (lNodeIdx * 2) + 1, lLeft, lCenter);
        sqResR = GetValue(lA, lB, (lNodeIdx * 2) + 2, lCenter, lRight);
        return T::uniteNode(sqResL, sqResR);
    }
};


/*----------------------------------------------*/

static SQWORD s_sqMappedNumCur = 0;
static std::map<SQWORD, SQWORD>  mapNums;

static void registMap(SQWORD sqRawNum) {
    if (mapNums.find(sqRawNum) == mapNums.end()) {
        mapNums[sqRawNum] = s_sqMappedNumCur;
        s_sqMappedNumCur++;
    } else {
        ;
    }
}

static void getChangeIdx(std::vector<SQWORD> vsqVals, SQWORD sqN, std::vector<SQWORD> &vsqChgIdx)
{
    SQWORD sqCur = 0;
    std::vector<bool> vbIsAppeared(mapNums.size());
    while(1) {
        SQWORD sqNum = MAX_SQWORD;
        while (1) {
            if (sqCur == sqN) {
                /* 終端 */
                break;
            }
            sqNum = vsqVals[sqCur];
            sqCur++;
            if (!vbIsAppeared[sqNum]) {
                vbIsAppeared[sqNum] = true;
                vsqChgIdx.emplace_back(sqCur - 1);
                break;
            } 
        }
        if (MAX_SQWORD == sqNum) {
            vsqChgIdx.emplace_back(sqN);
            break;
        }
    }
}

struct MATCH_RANGE {
    SQWORD sqAl;
    SQWORD sqAh;
    SQWORD sqBl;
    SQWORD sqBh;

    MATCH_RANGE(SQWORD al, SQWORD ah, SQWORD bl, SQWORD bh): sqAl(al), sqAh(ah), sqBl(bl), sqBh(bh) {};
};


int main(void)
{
    SQWORD sqN = inputSQWORD();
    std::vector<SQWORD> vsqA;
    std::vector<SQWORD> vsqB;
    
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        vsqA.emplace_back(sqA);
        registMap(sqA);
    }
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqB = inputSQWORD();
        vsqB.emplace_back(sqB);
        registMap(sqB);
    }
    for (auto &a: vsqA) {
        a = mapNums[a];
    }
    for (auto &b: vsqB) {
        b = mapNums[b];
    }

    std::vector<SQWORD> vsqChgIdxA;
    std::vector<SQWORD> vsqChgIdxB;
    getChangeIdx(vsqA, sqN, vsqChgIdxA);
    getChangeIdx(vsqB, sqN, vsqChgIdxB);

    std::vector<SEGMENT_NODE_ST> vstSegNode(s_sqMappedNumCur);
    SegmentTree<SEGMENT_NODE_ST> segCounts(vstSegNode);
    std::vector<MATCH_RANGE> vMatchRange;
    std::vector<SQWORD> vLabels;

    SQWORD sqCurIdx = 0;
    while (1) {
        SQWORD sqValA = vsqA[vsqChgIdxA[sqCurIdx]];
        SQWORD sqValB = vsqB[vsqChgIdxB[sqCurIdx]];

        SEGMENT_NODE_ST stVal;
        stVal = segCounts.getNode(sqValA);
        stVal.sqX += 1;
        segCounts.update(sqValA, stVal);

        stVal = segCounts.getNode(sqValB);
        stVal.sqX -= 1;
        segCounts.update(sqValB, stVal);

        SQWORD sqSum = (segCounts.GetValue(0, s_sqMappedNumCur)).sqX;
        
        if (0 == sqSum) {
            vMatchRange.emplace_back(vsqChgIdxA[sqCurIdx], vsqChgIdxA[sqCurIdx + 1], 
                                    vsqChgIdxB[sqCurIdx], vsqChgIdxB[sqCurIdx + 1]);
            vLabels.emplace_back(vsqChgIdxA[sqCurIdx]);
        }
        sqCurIdx++;
        if ((sqCurIdx == vsqChgIdxA.size() - 1) || (sqCurIdx == vsqChgIdxB.size() - 1)) {
            break;
        }
    }

    SQWORD sqQ = inputSQWORD();
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();

        sqX--;
        sqY--;

#if 0
        printf("%lld %lld\n", sqX, sqY);
        for (auto l: vLabels) {
            printf("%lld ", l);
        }
        printf("\n");
#endif
        auto it = lower_bound(vLabels.begin(), vLabels.end(), sqX);
        SQWORD sqQueryIdx;
        if (it == vLabels.end()) {
            sqQueryIdx = vLabels.size() - 1;
        } else {
            sqQueryIdx = it - vLabels.begin();
        }
        MATCH_RANGE m = vMatchRange[sqQueryIdx];
        if ((m.sqAl <= sqX) && (sqX < m.sqAh)
            && (m.sqBl <= sqY) && (sqY < m.sqBh)) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }


    return 0;
}
