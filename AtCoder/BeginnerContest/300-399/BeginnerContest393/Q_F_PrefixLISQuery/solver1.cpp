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

        stRet.sqX = max(nodeA.sqX, nodeB.sqX);
        return stRet;
    }
    bool operator==(const SEGMENT_NODE_ST &a) {
        return this->sqX == a.sqX;
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
    vector<T> vNode;

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

    void initSegmentTree(vector<T> &v) {
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
    SegmentTree(vector<T> v) {
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
            SEGMENT_NODE_ST vNodePrev = vNode[lTreeIdx];
            vNode[lTreeIdx] = updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2]);
            if (vNode[lTreeIdx] == vNodePrev) {
                /* no update */
                break;
            }
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

/*----------------------------------------------*/
typedef struct ST_QUERY {
    QWORD qIdx;
    QWORD qR;
    QWORD qX;
    QWORD qAns;

    ST_QUERY (QWORD idx, QWORD r, QWORD x) {
        qIdx = idx;        
        qR = r;
        qX = x;
        qAns = 0;
    }
    static bool CompByR(const ST_QUERY &a, const ST_QUERY &b) {
        return a.qR < b.qR;
    }
    static bool CompByIdx(const ST_QUERY &a, const ST_QUERY &b) {
        return a.qIdx < b.qIdx;
    }

} ST_QUERY;


typedef struct ST_VALUE_IDX {
    SQWORD sqVal;
    SQWORD sqIdx;
    ST_VALUE_IDX (QWORD a) {
        sqVal = a;
        sqIdx = 0;
    }

    static bool CompByVal(const ST_VALUE_IDX &a, const ST_VALUE_IDX &b) {
        return a.sqVal < b.sqVal;
    }
    static bool CompByIdx(const ST_VALUE_IDX &a, const ST_VALUE_IDX &b) {
        return a.sqIdx < b.sqIdx;
    }
} ST_VALUE_IDX;


int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    vector<SQWORD> vsqA;
    vector<ST_QUERY> vstQuery;
    set<SQWORD> setValuesA;
    map<SQWORD, SQWORD> mapValuesToIdxA;
    vector<SQWORD> mapIdxToValuesA(sqN);

    /* read values */
    /*
     * compress values to idx; 
     */
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqA = inputSQWORD();
        vsqA.emplace_back(sqA);
        setValuesA.insert(sqA);
    }

    SQWORD sqIdxA = 0;
    for (auto a: setValuesA) {
        mapValuesToIdxA[a] = sqIdxA;
        mapIdxToValuesA[sqIdxA] = a;
        sqIdxA++;
    }

    /* read queries */
    vector<vector<ST_QUERY>> vvstQuery(sqN);
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqR = inputSQWORD() - 1;
        SQWORD sqX = inputSQWORD();
        auto it = setValuesA.upper_bound(sqX);
        it--;
        SQWORD sqRefValA = *it;
        SQWORD sqIdxA = mapValuesToIdxA[sqRefValA];
        vvstQuery[sqR].emplace_back(sqIdx, sqR, sqIdxA);
    }

    vector<SEGMENT_NODE_ST> vstSegVals(setValuesA.size());
    SegmentTree<SEGMENT_NODE_ST> seg(vstSegVals);    
    bool bFirstSet = false;

    for (SQWORD sqIdx = 0; sqIdx < vsqA.size(); sqIdx++) {
        auto a= vsqA[sqIdx];
        SQWORD sqIdxA = mapValuesToIdxA[a];

        if (0 == sqIdxA) {
            if (!bFirstSet) {
                seg.update(0, 1);
                bFirstSet = true;
            }
        } else {
            SQWORD sqMaxLength = seg.GetValue(0, sqIdxA).sqX;
            seg.update(sqIdxA, sqMaxLength + 1);
        }
        for (auto &q: vvstQuery[sqIdx]) {
            SQWORD sqAns = seg.GetValue(0, q.qX + 1).sqX;
            q.qAns = sqAns;
        }
    }

    vector<SQWORD> vsqAns(sqQ);
    for (SQWORD sqIdx = 0; sqIdx < vvstQuery.size(); sqIdx++) {
        for (const auto &q: vvstQuery[sqIdx]) {
            vsqAns[q.qIdx] = q.qAns;
        }
    }

    for (auto a: vsqAns) {
        printf("%lld\n", a);
    }

    return 0;
}
