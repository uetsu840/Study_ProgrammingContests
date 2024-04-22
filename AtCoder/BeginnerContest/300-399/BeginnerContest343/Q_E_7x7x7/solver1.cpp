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

/*----------------------------------------------*/

/*----------------------------------------------*/
struct ST_BOX {
    SQWORD sqX;
    SQWORD sqY;
    SQWORD sqZ;
    SQWORD sqLx;
    SQWORD sqLy;
    SQWORD sqLz;
};

static void MakeDefaultBox(SQWORD sqX, SQWORD sqY, SQWORD sqZ, ST_BOX &b)
{
    b.sqX = sqX;
    b.sqY = sqY;
    b.sqZ = sqZ;

    b.sqLx = 7;
    b.sqLy = 7;
    b.sqLz = 7;
}

static void getIntersection1D(
    SQWORD sqA,
    SQWORD sqLa,
    SQWORD sqB,
    SQWORD sqLb, 
    SQWORD &sqX,
    SQWORD &sqLx)
{
    if (sqA <= sqB) {
        sqX = sqB;
        sqLx = MAX(sqA + sqLa - sqB, 0LL);
        sqLx = MIN(sqLx, sqLb);
    } else {
        sqX = sqA;
        sqLx = MAX(sqB + sqLb - sqA, 0LL);
        sqLx = MIN(sqLx, sqLb);
    }
}

static SQWORD sqV1;
static SQWORD sqV2;
static SQWORD sqV3;

static void getAndBox(const ST_BOX &a, const ST_BOX &b, ST_BOX &x)
{
    getIntersection1D(a.sqX, a.sqLx, b.sqX, b.sqLx, x.sqX, x.sqLx);
    getIntersection1D(a.sqY, a.sqLy, b.sqY, b.sqLy, x.sqY, x.sqLy);
    getIntersection1D(a.sqZ, a.sqLz, b.sqZ, b.sqLz, x.sqZ, x.sqLz);
}

static SQWORD getVolume(const ST_BOX &box)
{
    return box.sqLx * box.sqLy * box.sqLz;
}

static bool solveOne(SQWORD sqA2, SQWORD sqB2, SQWORD sqC2, SQWORD sqA3, SQWORD sqB3, SQWORD sqC3)
{
    ST_BOX box_1, box_2, box_3;    
    ST_BOX box_12, box_23, box_13;
    ST_BOX box_123;

    MakeDefaultBox(0, 0, 0, box_1);
    MakeDefaultBox(sqA2, sqB2, sqC2, box_2);
    MakeDefaultBox(sqA3, sqB3, sqC3, box_3);

    getAndBox(box_1, box_2, box_12);
    getAndBox(box_1, box_3, box_13);
    getAndBox(box_2, box_3, box_23);

    ST_BOX box_tmp;
    getAndBox(box_12, box_13, box_tmp);
    getAndBox(box_23, box_tmp, box_123);


    SQWORD sqBox1 = getVolume(box_1);
    SQWORD sqBox2 = getVolume(box_2);
    SQWORD sqBox3 = getVolume(box_3);

    SQWORD sqBox12 = getVolume(box_12);
    SQWORD sqBox23 = getVolume(box_23);
    SQWORD sqBox13 = getVolume(box_13);

    SQWORD sqBox123 = getVolume(box_123);

//    printf("%d %d %d %d %d %d %d\n", sqBox1, sqBox2, sqBox3, sqBox12, sqBox13, sqBox23, sqBox123);

    SQWORD sqV3Ref = sqBox123;
    SQWORD sqV2Ref = sqBox12 + sqBox13 + sqBox23 - 3 * sqBox123;
    SQWORD sqV1Ref = sqBox1 + sqBox2 + sqBox3 - 2 * sqV2Ref - 3 * sqV3Ref;

//    printf("%d %d %d\n", sqV3Ref, sqV2Ref, sqV1Ref);

    if ((sqV1Ref == sqV1) && (sqV2Ref == sqV2) && (sqV3Ref == sqV3)) {
        return true;
    }
    return false;
}

static bool solve(void)
{
#if 1
    for (SQWORD sqa2 = 0; sqa2 <= 7; sqa2++) {
       for (SQWORD sqb2 = 0; sqb2 <= 7; sqb2++) {
           for (SQWORD sqc2 = 0; sqc2 <= 7; sqc2++) {
                for (SQWORD sqa3 = -7; sqa3 <= 7; sqa3++) {
                    for (SQWORD sqb3 = -7; sqb3 <= 7; sqb3++) {
                        for (SQWORD sqc3 = -7; sqc3 <= 7; sqc3++) {
                            if (solveOne(sqa2, sqb2, sqc2, sqa3, sqb3, sqc3)) {
                                printf("Yes\n");
                                printf("%d %d %d %d %d %d %d %d %d\n", 0, 0, 0, sqa2, sqb2, sqc2, sqa3, sqb3, sqc3);
                                return true;
                            }
                        }
                    }
                }
           }
       }
    }
#endif
//    solveOne(0, 6, 0, 6, 0, 0);


    printf("No\n");
    return false;
}

int main(void)
{
    sqV1 = inputSQWORD();
    sqV2 = inputSQWORD();
    sqV3 = inputSQWORD();
    solve();

    return 0;
}
