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
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

#define MAX_POINT (500007)

/* ----------------------------- KD木 -----------------------------*/

struct Point {
  SQWORD    sqX;
  SQWORD    sqY;
  SQWORD    sqIdx;

  Point(SQWORD x, SQWORD y, SQWORD i): sqX(x), sqY(y), sqIdx(i) {};
};

bool PointCompareX(const Point &p1, const Point &p2) 
{
    return p1.sqX < p2.sqX; 
}

bool PointCompareY(const Point &p1, const Point &p2) 
{ 
    return p1.sqY < p2.sqY;
}

struct KD_NODE {
  KD_NODE *left;
  KD_NODE *right;
  Point p;

  KD_NODE(KD_NODE *left, KD_NODE *right, Point p) : left(left), right(right), p(p) {}
};


/**
*   KD木の構築 
*/
KD_NODE *make(
    SQWORD sqL, 
    SQWORD sqR, 
    SQWORD sqDepth,
    vector<Point> &points) 
{
    if(!(sqL < sqR)) {
        return nullptr;
    }
    
    SQWORD sqMid = (sqL + sqR) / 2;

    auto point_base = points.begin();
    if ((sqDepth % 2) == 0) {
        sort(point_base + sqL, point_base + sqR, PointCompareX);
    } else {
        sort(point_base + sqL, point_base + sqR, PointCompareY);
    }

    return new KD_NODE(make(sqL, sqMid, sqDepth + 1, points), 
                        make(sqMid + 1, sqR, sqDepth + 1, points), 
                        points[sqMid]);
}


/** 
*
*/
static KD_NODE *KdTreeRoot;
void InitKdTree(
    vector<Point> points)
{
    SQWORD sqN = points.size();
    KdTreeRoot = make(0, sqN, 0, points);
}

/**
*  最近傍点の探索
*/
SQWORD findNearest(
    const KD_NODE *nd, 
    SQWORD sqX,             /* 検索点のX座標 */
    SQWORD sqY,             /* 検索点のY座標 */
    SQWORD sqDepth, 
    SQWORD sqR)             /* 現在見つかっている最小点までの距離 */
{
    if (nd == nullptr) {
        return sqR;
    }
    
    const Point &p = nd->p;
    
    SQWORD sqD = (sqX - p.sqX) * (sqX - p.sqX) + (sqY - p.sqY) * (sqY - p.sqY);
    if ((sqR == -1) || sqD < sqR) {
        sqR = sqD;
    }

    if(0 == sqDepth % 2) {
        if ((nd->left != nullptr) && (sqX - sqR <= p.sqX)) {
           sqR = findNearest(nd->left, sqX, sqY, sqDepth + 1, sqR);
        }
        if ((nd->right != nullptr) && (p.sqX <= sqX + sqR)) {
            sqR = findNearest(nd->right, sqX, sqY, sqDepth + 1, sqR);
        }
    } else {
        if ((nd->left != nullptr) && (sqY - sqR) <= p.sqY) {
            sqR = findNearest(nd->left, sqX, sqY, sqDepth + 1, sqR);
        }
        if ((nd->right) != nullptr && (p.sqY <= sqY + sqR)) {
            sqR = findNearest(nd->right, sqX, sqY, sqDepth + 1, sqR);
        }
    }
    return sqR;
}

struct ST_RECTANGULAR {
    SQWORD sqSx;
    SQWORD sqSy;
    SQWORD sqTx; 
    SQWORD sqTy; 

    ST_RECTANGULAR(SQWORD sx, SQWORD sy, SQWORD tx, SQWORD ty): sqSx(sx), sqSy(sy), sqTx(tx), sqTy(ty) {};
};

static bool isInsideRectangular(
    const ST_RECTANGULAR &rec,
    const Point &p)
{
    if (p.sqX < rec.sqSx) {
        return false;
    }

    if (p.sqY < rec.sqSy) {
        return false;
    }

    if (rec.sqTx < p.sqX) {
        return false;
    }

    if (rec.sqTy < p.sqY) {
        return false;
    }

    return true;
}

/**
*   矩形範囲内の点の検索 
*/
void findInsideRectangular(
    KD_NODE *nd, 
    vector<SQWORD> &result, 
    const ST_RECTANGULAR &rec,
    SQWORD sqDepth) 
{
    Point &p = nd->p;
    if (isInsideRectangular(rec, p)) {
        result.push_back(p.sqIdx);
    }

    if(0 == sqDepth % 2) {
        if ((nd->left != nullptr) && (rec.sqSx <= p.sqX)) {
            findInsideRectangular(nd->left, result, rec, sqDepth + 1);
        }
        if ((nd->right != nullptr) && (p.sqX <= rec.sqTx)) {
            findInsideRectangular(nd->right, result, rec, sqDepth + 1);
        }
    } else {
        if ((nd->left != nullptr) && (rec.sqSy <= p.sqY)) {
            findInsideRectangular(nd->left, result, rec, sqDepth + 1);
        }
        if ((nd->right) != nullptr && (p.sqY <= rec.sqTy)) {
            findInsideRectangular(nd->right, result, rec, sqDepth + 1);
        }
    }
}

/* ---------------------------------------------------------------*/

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

/* ---------------------------------------------------------------*/

int main(void)
{
    SQWORD sqN = inputSQWORD();
    vector<Point> points;
    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        points.emplace_back(sqX, sqY, sqIdx);
    }

    InitKdTree(points);

    SQWORD sqQ = inputSQWORD();
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqSx = inputSQWORD();
        SQWORD sqTx = inputSQWORD();
        SQWORD sqSy = inputSQWORD();
        SQWORD sqTy = inputSQWORD();

        vector<SQWORD> vsqAns;
        ST_RECTANGULAR rect(sqSx, sqSy, sqTx, sqTy);

        findInsideRectangular(KdTreeRoot, vsqAns, rect, 0);
        sort(vsqAns.begin(), vsqAns.end());
        for (auto a: vsqAns) {
            printf("%lld\n", a);
        }
        printf("\n");
    }
    return 0;
}
