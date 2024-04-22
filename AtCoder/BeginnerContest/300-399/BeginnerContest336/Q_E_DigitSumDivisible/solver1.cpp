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
#define MAX_DIGIT_SUM   (9 * 14)

int main(void)
{
    string strN;
    cin >> strN;

    SQWORD sqDigitNum = strN.length();

    SQWORD sqTen = 1;
    for (SQWORD sqDigit = 1; sqDigit < sqDigitNum; sqDigit++) {
        sqTen *= 10;
    }


    /**
     * dp(m)[i][j][k]   上からm桁目までを考えたとき、
     *            mod p で考えたときに桁和がjで剰余がkになるような数の個数の和
     *      i = 0   それまでの桁が一致している。
     *      i = 1   それまでの桁で一致してない数がある。
     */
    SQWORD sqDpTblSize = 9 * sqDigitNum + 1;
    SQWORD sqAns = 0;
    for (SQWORD sqMod = 1; sqMod < sqDpTblSize; sqMod++) {
        vector<vector<vector<SQWORD>>>
                vvsqDp(2, vector<vector<SQWORD>>(sqDpTblSize, vector<SQWORD>(sqDpTblSize, 0)));

        vvsqDp[0][0][0] = 1;
        SQWORD sqCurTen = sqTen;

        for (SQWORD sqDigit = 0; sqDigit < sqDigitNum; sqDigit++) {
            vector<vector<vector<SQWORD>>>
                    vvsqDpNext(2, vector<vector<SQWORD>>(sqDpTblSize, vector<SQWORD>(sqDpTblSize, 0)));
            SQWORD sqCurNumber = strN[sqDigit] - '0';

            for (SQWORD sqRemain = 0; sqRemain < sqMod; sqRemain++) {
                for (SQWORD sqCursqDigitSum = 0; sqCursqDigitSum < sqDpTblSize - 9; sqCursqDigitSum++) {
                    /* i = 0 */
                    vvsqDpNext[0]
                                [sqCursqDigitSum + sqCurNumber]
                                [(sqRemain + sqCurNumber * sqCurTen) % sqMod]
                                                            += vvsqDp[0][sqCursqDigitSum][sqRemain];
                    for (SQWORD sqDpNumber = 0; sqDpNumber < sqCurNumber; sqDpNumber++) {
                        vvsqDpNext[1]
                                    [sqCursqDigitSum + sqDpNumber]
                                    [(sqRemain + sqDpNumber * sqCurTen) % sqMod]
                                                            += vvsqDp[0][sqCursqDigitSum][sqRemain];
                    }
                    /* i = 1 */
                    for (SQWORD sqDpNumber = 0; sqDpNumber <= 9; sqDpNumber++) {
                        vvsqDpNext[1]
                                    [sqCursqDigitSum + sqDpNumber]
                                    [(sqRemain + sqDpNumber * sqCurTen) % sqMod]
                                                            += vvsqDp[1][sqCursqDigitSum][sqRemain];
                    }
                }
            }
            sqCurTen /= 10;
            swap(vvsqDp, vvsqDpNext);
#if 0
            for (SQWORD sqCurDigitSum = 0; sqCurDigitSum < sqDpTblSize; sqCurDigitSum++) {
                printf("  > %d %d\n", sqCurDigitSum, sqTen);
                for (auto d : vvsqDp[0][sqCurDigitSum]) {
                    printf("%lld ", d);
                }
                printf("\n");
                for (auto d : vvsqDp[1][sqCurDigitSum]) {
                    printf("%lld ", d);
                }
                printf("\n");
                printf("---\n");

            }
#endif
        }
        sqAns += vvsqDp[0][sqMod][0];
        sqAns += vvsqDp[1][sqMod][0];
//        printf("num : %lld %lld\n", vvsqDp[0][sqMod][0], vvsqDp[1][sqMod][0]);

    }
    printf("%lld\n", sqAns);

    return 0;
}
