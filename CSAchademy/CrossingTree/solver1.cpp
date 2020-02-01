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
/**
 *  mod による操作ライブラリ
 */
#define ANS_MOD (1000000007)

class MODINT {
    static SQWORD MOD;
    SQWORD m_x;

public:
    MODINT(SQWORD val) {
        m_x = (val % MOD + MOD) % MOD;
    };
    MODINT() {
        m_x = 0;
    }
    static void Init(SQWORD sqMod) {
        MOD = sqMod;
    }

	MODINT& operator+= (const MODINT a)
    {
        m_x = (m_x + a.m_x) % MOD; 
        return *this;
    };
	MODINT& operator-= (const MODINT a)
    { 
        m_x = (m_x - a.m_x + MOD) % MOD; 
        return *this;
    };
	MODINT& operator*= (const MODINT a)
    {
        m_x = (m_x * a.m_x) % MOD;
        return *this;
    };
    MODINT pow(SQWORD t) const {
        if (!t) return 1;
        MODINT a = pow(t>>1);
        a *= a;
        if (t&1) a *= *this;
        return a;
    }
	MODINT operator+ (const MODINT a) const {
		MODINT res(*this);
		return (res += a);
	}
	MODINT operator- (const MODINT a) const {
		MODINT res(*this);
		return (res -= a);
	}
	MODINT operator* (const MODINT a) const {
		MODINT res(*this);
		return (res *= a);
	}
	MODINT operator/ (const MODINT a) const {
		MODINT res(*this);
		return (res /= a);
	}

    /* 逆元 */
    MODINT inv() const {
        return pow(MOD-2);
    }

    /* 除算 */
    MODINT& operator/=(const MODINT a) {
        return (*this) *= a.inv();
    } 

    /* 整数版 */
	MODINT& operator+= (const SQWORD a) {*this += MODINT(a); return *this;};
	MODINT& operator-= (const SQWORD a) {*this -= MODINT(a); return *this;};
	MODINT& operator*= (const SQWORD a) {*this *= MODINT(a); return *this;};
	MODINT& operator/= (const SQWORD a) {*this /= MODINT(a); return *this;};

    SQWORD getVal() { return m_x; };
};
SQWORD MODINT::MOD = ANS_MOD;

/*----------------------------------------------*/
/*----------------------------------------------*/


/*----------------------------------------------*/

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;
    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D() : dX(0), dY(0) {};

	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    const VECTOR_2D operator- (const VECTOR_2D a) const {
		return VECTOR_2D(dX - a.dX, dY - a.dY);
    };

    DOUBLE norm(void)
    {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void)
    {
        return sqrt(norm());
    }

    DOUBLE angle(void)
    {
        return atan2(dY, dX);
    }
};


struct PROBLEM_ONE {
    SQWORD sqN;
    vector<VECTOR_2D> vecCenter;
};

const DOUBLE d_PI = 3.14159265358979;

static DOUBLE normalizeAngle(DOUBLE dAngle)
{
    DOUBLE dRet = dAngle;
    while (dRet < 0.0) {
        dRet += (2.0 * d_PI);
    }

    while ((2.0 * d_PI) < dRet) {
        dRet -= (2.0 * d_PI);
    }

    return dRet;
}


enum PNT_EVENT_TYPE {
    PNT_EVENT_ADD = 0,
    PNT_EVENT_SUB
};

struct INTERSECTION {
    DOUBLE dAngle;
    SQWORD sqCircleIdx;
    PNT_EVENT_TYPE  eEv;

    INTERSECTION(DOUBLE d, SQWORD idx, PNT_EVENT_TYPE e): dAngle(d), sqCircleIdx(idx), eEv(e) {};
};

bool operator< (const INTERSECTION &a, const INTERSECTION &b)
{
    return a.dAngle < b.dAngle;
}


static SQWORD getMaxIntersect(
    const PROBLEM_ONE *pstPro,
    DOUBLE dRadius)
{
    SQWORD sqMaxIntersect = 0;


    for (SQWORD sqBaseIdx = 0; sqBaseIdx < pstPro->sqN; sqBaseIdx++) {
        vector<INTERSECTION> vecstIntersection;
        SQWORD sqIncludeCnt = 0;
        VECTOR_2D stBase = pstPro->vecCenter[sqBaseIdx];
        for (SQWORD sqTgtIdx = 0; sqTgtIdx < pstPro->sqN; sqTgtIdx++) {
            VECTOR_2D stTgt = pstPro->vecCenter[sqTgtIdx];
            if (sqTgtIdx != sqBaseIdx) {
                VECTOR_2D stRel = stTgt - stBase;
                DOUBLE dDist = stRel.dist();

                if (dDist < dRadius * 2.0) {
                    DOUBLE dCenterAngle = stRel.angle();
                    DOUBLE dRelAngle = acos(dDist / (dRadius * 2.0));
                    DOUBLE dAngle1 = normalizeAngle(dCenterAngle - dRelAngle);
                    DOUBLE dAngle2 = normalizeAngle(dCenterAngle + dRelAngle);

                    vecstIntersection.push_back(INTERSECTION{dAngle1, sqTgtIdx, PNT_EVENT_ADD});
                    vecstIntersection.push_back(INTERSECTION{dAngle2, sqTgtIdx, PNT_EVENT_SUB});
                }
            }
        }
        sort(vecstIntersection.begin(), vecstIntersection.end());
        set<SQWORD> setsqCurCrossing;
        SQWORD sqCnt = 0;
        for (SDWORD lCircle = 0; lCircle < 2; lCircle++) {
            for (auto intsect: vecstIntersection) {
                SQWORD sqCircleIdx = intsect.sqCircleIdx;
                if (PNT_EVENT_ADD == intsect.eEv) {
                    /* 追加 */
                    setsqCurCrossing.insert(sqCircleIdx);
                } else {
                    /* 削除 */
                    setsqCurCrossing.erase(sqCircleIdx);
                }
                sqCnt = max((SQWORD)setsqCurCrossing.size(), sqCnt);
            }
        }

        sqMaxIntersect = max(sqCnt, sqMaxIntersect);
    }
    return sqMaxIntersect;
}


/*----------------------------------------------*/


void dfs_depth(
    const vector<vector<SQWORD>> &vvEdge,
    SQWORD &sqMaxDepth,
    SQWORD &sqMaxDepthNode,
    SQWORD sqFrom,
    SQWORD sqCur,
    SQWORD sqDepth)
{
    if (sqMaxDepth < sqDepth) {
        sqMaxDepthNode = sqCur;
        sqMaxDepth = sqDepth;
    }
    for (auto next_node: vvEdge[sqCur]) {
        if (next_node != sqFrom) {
            dfs_depth(vvEdge, sqMaxDepth, sqMaxDepthNode, sqCur, next_node, sqDepth + 1);
        }
    }
}

static SQWORD findMaxDistNode(
    const vector<vector<SQWORD>> &vvEdge,
    SQWORD sqStartNode)
{
    vector<SQWORD> vDepth(vvEdge.size() + 1, 0);
    SQWORD sqNodeMaxDepth;
    SQWORD sqMaxDepth = 0;
    dfs_depth(vvEdge, sqMaxDepth, sqNodeMaxDepth, -1, sqStartNode, 0);

    return sqNodeMaxDepth;
}

bool dfs_longest_path(
    const vector<vector<SQWORD>> &vvEdge,
    vector<SQWORD> &vPath,
    SQWORD sqTarget,
    SQWORD sqFrom,
    SQWORD sqCur)
{
    if (sqTarget == sqCur) {
        vPath.emplace_back(sqCur);
        return true;
    }
    for (auto next_node: vvEdge[sqCur]) {
        if (next_node != sqFrom) {
            if (dfs_longest_path(vvEdge, vPath, sqTarget, sqCur, next_node)) {
                vPath.emplace_back(sqCur);
                return true;
            }
        }
    }
    return false;
}

static void dfs_euler_tour(
    const vector<vector<SQWORD>> &vvEdge,
    vector<SQWORD> &vPath,
    SQWORD sqFrom,
    SQWORD sqCur)
{
    vPath.emplace_back(sqCur);
    for (auto next_node: vvEdge[sqCur]) {
        if (next_node != sqFrom) {
            dfs_euler_tour(vvEdge, vPath, sqCur, next_node);            
            vPath.emplace_back(sqCur);
        }
    }
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    vector<vector<SQWORD>> vvEdge(sqN + 1);

    for (SQWORD sqIdx = 0; sqIdx < sqN - 1; sqIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();

        vvEdge[sqU].emplace_back(sqV);
        vvEdge[sqV].emplace_back(sqU);
    }

    /* 最長パスを求める */
    SQWORD sqNodeStart = findMaxDistNode(vvEdge, 1);
    SQWORD sqNodeEnd   = findMaxDistNode(vvEdge, sqNodeStart);

//    printf("%lld -> %lld\n", sqNodeStart, sqNodeEnd);

    vector<SQWORD> vLongestPath;
    dfs_longest_path(vvEdge, vLongestPath, sqNodeEnd, -1, sqNodeStart);

    vector<vector<SQWORD>> vvEdgeSep(sqN + 1);
    vector<vector<SQWORD>> vvNeighbors(sqN + 1, vector<SQWORD>(2, -1));
    for (SQWORD sqIdx = 0; sqIdx < vLongestPath.size(); sqIdx++) {
        if (0 != sqIdx) {
            vvNeighbors[vLongestPath[sqIdx]][0] = vLongestPath[sqIdx - 1];
        }
        if (vLongestPath.size() - 1 != sqIdx) {
            vvNeighbors[vLongestPath[sqIdx]][1] = vLongestPath[sqIdx + 1];
        }
    }

    for (SQWORD sqCurNode = 0; sqCurNode < vvEdge.size(); sqCurNode++) {
        for (auto node: vvEdge[sqCurNode]) {
            if ((vvNeighbors[sqCurNode][0] != node) && (vvNeighbors[sqCurNode][1] != node)) {
                vvEdgeSep[sqCurNode].emplace_back(node);
            }
        }
    }

    /* パス上のエッジを除いてエッジを再構築する */
    vector<SQWORD> vAns;
    for (auto sn: vLongestPath) {
        vector<SQWORD> vPath;
        dfs_euler_tour(vvEdgeSep, vPath, -1, sn);

#if 0
        printf("start node : %lld\n", sn);
        for (auto n: vPath) {
            printf("%lld ", n);
        }
        printf("\n");
#endif
        for (auto n: vPath) {
            vAns.emplace_back(n);
        }
    }
    printf("%lld\n", vAns.size() - 1);
    for (SQWORD sqIdx = 0; sqIdx < vAns.size(); sqIdx++) {
        printf("%lld", vAns[sqIdx]);
        if (sqIdx == vAns.size() - 1) {
            printf("\n");
        } else {
            printf(" ");
        }
    }

    return 0;
}