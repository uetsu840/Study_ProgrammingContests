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
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }

static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }

static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }

static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }

static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline DOUBLE ABS(DOUBLE a) { return 0 < a ? a : -a; }
static inline bool DoubleIsZero(const DOUBLE &a)
{
    return ABS(a) < DOUBLE_EPS;
}

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


/**
 *  mod による操作ライブラリ
 */

#define ANS_MOD (1000000007LL)
 
static SQWORD addMod(SQWORD x, SQWORD y)
{ 
    return (x + y) % ANS_MOD;
}
 
static SQWORD subMod(SQWORD x, SQWORD y)
{
    return (x - y + ANS_MOD) % ANS_MOD;
}
 
static SQWORD mulMod(SQWORD x, SQWORD y) 
{
    return (x * y) % ANS_MOD;
}
 
static SQWORD powMod(SQWORD x, SQWORD e) {
    SQWORD v = 1;
    for (; e; x = mulMod(x, x), e >>= 1) {
        if (e & 1) {
            v = mulMod(v, x);
        }
    }
    return v;
}
 
static SQWORD divMod(SQWORD x, SQWORD y)
{
    return mulMod(x, powMod(y, ANS_MOD - 2));
}
 
 
static SQWORD combMod(SQWORD n, SQWORD k)
{
    SQWORD v=1;
    for(SQWORD i=1; i<=k; i++) {
        v = divMod(mulMod(v, n-i+1),i);
    } 
    return v;
}

/*----------------------------------------------*/

#define MAX_N_STR   (512)    

static SQWORD binStrToInt(string &str)
{
    SQWORD sqAns = 0;
    SQWORD sqBin = 1 << (str.length() - 1);
    for (auto c: str) {
        if (c == '1') {
            sqAns += sqBin; 
        }
        sqBin >>= 1;
    }
    return sqAns;
}

#define L_MAX   (100)
#define W_MAX   (8)
#define DPTBL_W_SIZE    (0x1 << (W_MAX - 1))
#define DPTBL_SEP_SIZE  (0x1 << W_MAX)

struct STRING_INFO {
    SQWORD sqVal;
    SQWORD sqLen;

    STRING_INFO (SQWORD v, SQWORD l) : sqVal(v), sqLen(l) {};
};

bool operator< (const STRING_INFO &a, const STRING_INFO &b) {
    return a.sqLen < b.sqLen;
}

static bool matchStr(
    SQWORD sqTail, 
    SQWORD sqSep, 
    const vector<STRING_INFO> &vecS)
{
    for (auto s: vecS) {
        SQWORD sqMask = (0x1 << s.sqLen) - 1;
        if (((0x1 << s.sqLen) & (sqSep << 1)) 
            && ((sqMask & sqTail) == s.sqVal)) {
            return true; 
        }
    }
    return false;
}

int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqL = inputSQWORD();

    vector<STRING_INFO> vecstW;

    for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
        string w;
        cin >> w;

        SQWORD sqVal = binStrToInt(w);
        vecstW.emplace_back(sqVal, w.size());
    }
    sort(vecstW.begin(), vecstW.end());

    /**
     *  dp[i][j][k]  長さが i で、末尾7文字の値が j, kの位置に区切りがある組み合わせの数
     * 
     *  dp[i+1][(j<<1|0)][k<<1|1] += dp[i][j][k] if match (j<<1|0, str)
     *  dp[i+1][(j<<1|0)][k<<1|0] += dp[i][j][k] if not match
     * 
     *  dp[i+1][(j<<1|1)][k<<1|1] += dp[i][j][k] if match (j<<1|1, str)
     *  dp[i+1][(j<<1|1)][k<<1|0] += dp[i][j][k] if not match
     */
    static SQWORD s_asqDpTbl[L_MAX + 1 + W_MAX][DPTBL_W_SIZE][DPTBL_SEP_SIZE];
    s_asqDpTbl[0][0][1] = 1;

    SQWORD sqTailValMax = DPTBL_W_SIZE - 1;
    SQWORD sqSepPosMax  = DPTBL_SEP_SIZE - 1;
    SQWORD sqTailMask   = sqTailValMax;
    SQWORD sqSepPosMask = sqSepPosMax;
    for (SQWORD sqIdxL = 0; sqIdxL < sqL; sqIdxL++) {
        for (SQWORD sqTailVal = 0; sqTailVal <= sqTailValMax; sqTailVal++) {
            for (SQWORD sqSepPos = 0; sqSepPos <= sqSepPosMax; sqSepPos++) {
                for (SQWORD sqAddVal = 0; sqAddVal < 2; sqAddVal++) {
                    SQWORD sqNextTail = (sqTailVal<<1) | sqAddVal;
                    if (matchStr(sqNextTail, sqSepPos, vecstW)) {
                        SQWORD *psqDpTblPtr = &(s_asqDpTbl[sqIdxL+1][sqNextTail & sqTailMask][((sqSepPos<<1) | 1) & sqSepPosMask]);
                        SQWORD sqDpNext = addMod(
                            *psqDpTblPtr,
                            s_asqDpTbl[sqIdxL][sqTailVal][sqSepPos]
                        );

                        *psqDpTblPtr = sqDpNext;
                    } else {
                        SQWORD *psqDpTblPtr = &(s_asqDpTbl[sqIdxL+1][sqNextTail & sqTailMask][((sqSepPos<<1) | 0) & sqSepPosMask]);
                        SQWORD sqDpNext = addMod(
                            *psqDpTblPtr,
                            s_asqDpTbl[sqIdxL][sqTailVal][sqSepPos]
                        );
                       
                        *psqDpTblPtr = sqDpNext;
                    }
                }
            }
        }
    }

    /* sum up L */
    SQWORD sqAns = 0;
    for (SQWORD sqTailVal = 0; sqTailVal <= sqTailValMax; sqTailVal++) {
        for (SQWORD sqSepPos = 0; sqSepPos <= sqSepPosMax; sqSepPos++) {
            if (sqSepPos & 0x1) {
                sqAns = addMod(sqAns, s_asqDpTbl[sqL][sqTailVal][sqSepPos]);
            }
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}