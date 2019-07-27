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

#define MAX_WIDTH   (2000)
#define SDWORD_INF  (100100100)

struct PIECE_STATUS_ONE_ST {
    SDWORD lBaseScore_o;    
    SDWORD lBaseScore_x;
    SDWORD lCount_o;
    SDWORD lCount_x;
    SDWORD lGap;
};

bool operator< (const PIECE_STATUS_ONE_ST &a, const PIECE_STATUS_ONE_ST &b)
{
    return (a.lCount_o + a.lCount_x) > (b.lCount_o + b.lCount_x);
}

static void processSingleLine(
    SDWORD lInput_W,
    vector<pair<SDWORD, SDWORD>> vecpairObj,
    SDWORD &lMinDist_x,
    SDWORD &lMinDist_o,
    vector<PIECE_STATUS_ONE_ST> &vecPiece)
{
    lMinDist_x = SDWORD_INF;
    lMinDist_o = SDWORD_INF;
    auto it = vecpairObj.begin();

    while (it != vecpairObj.end()) {
        SDWORD lCount_x = 0;
        SDWORD lCount_o = 0;
        vector<SDWORD> vec_o;
        vector<SDWORD> vec_x;
        while (it->first == 0) {
            vec_o.emplace_back(it->second);
            ++it;
            if (it == vecpairObj.end()) {
                break;
            }
        }
        if (it != vecpairObj.end()) {
            while (it->first == 1) {
                vec_x.emplace_back(it->second);
                ++it;
                if (it == vecpairObj.end()) {
                    break;
                }
            }
        }

        if (vec_o.empty()) {
            /* xのみ */
            lMinDist_x = *(vec_x.begin());
        } else if (vec_x.empty()) {
            /* oのみ */
            lMinDist_o = lInput_W - *(vec_o.end() - 1) - 1;
        } else {
            /* o と x */
            SDWORD lBasePos_o = *(vec_o.rbegin());
            for (auto it_o = vec_o.rbegin(); it_o != vec_o.rend(); ++it_o) {
                lCount_o += (lBasePos_o - *(it_o));
                lBasePos_o--;
            }
            SDWORD lBasePos_x = *(vec_x.begin());
            for (auto it_x = vec_x.begin(); it_x != vec_x.end(); ++it_x) {
                lCount_x += ((*it_x) - lBasePos_x);
                lBasePos_x++;
            }

            PIECE_STATUS_ONE_ST stPieceOne;
            stPieceOne.lBaseScore_o = lCount_o;
            stPieceOne.lBaseScore_x = lCount_x;
            stPieceOne.lCount_o = vec_o.size();
            stPieceOne.lCount_x = vec_x.size();
            stPieceOne.lGap = *(vec_x.begin()) - *(vec_o.rbegin()) - 1;
            vecPiece.emplace_back(stPieceOne);
        }
    }
}


int main(void)
{
    SDWORD lInput_H = inputSDWORD();
    SDWORD lInput_W = inputSDWORD();
    char acInput[MAX_WIDTH + 1];

    SDWORD lMinDist_x = SDWORD_INF;
    SDWORD lMinDist_o = SDWORD_INF;
    vector<PIECE_STATUS_ONE_ST> vecstPiece;

    for (SDWORD lRowIdx = 0; lRowIdx < lInput_H; lRowIdx++) {
        inputString(acInput);
        SDWORD lCnt = 0;
        vector<pair<SDWORD, SDWORD>> vecpairObj;

        for (SDWORD lColIdx = 0; lColIdx < lInput_W; lColIdx++) {
            if (acInput[lColIdx] == 'o') {
                vecpairObj.emplace_back(make_pair(0, lColIdx));
            } else if (acInput[lColIdx] == 'x') {
                vecpairObj.emplace_back(make_pair(1, lColIdx));
            } else {
                lCnt++;
            }
        }

        SDWORD lCurMinDist_x;
        SDWORD lCurMinDist_o;

        processSingleLine(lInput_W, vecpairObj, lCurMinDist_x, lCurMinDist_o, vecstPiece);

        lMinDist_o = min(lMinDist_o, lCurMinDist_o);
        lMinDist_x = min(lMinDist_x, lCurMinDist_x);
    }

    sort(vecstPiece.begin(), vecstPiece.end());

    SDWORD lTurn = 0;
    SQWORD sqTtlCnt_o = 0;
    SQWORD sqTtlCnt_x = 0;
    for (auto pieces: vecstPiece) {
        SDWORD lMid;
        if (lTurn % 2 == 0) {
            /* turn o */
            lMid = (pieces.lGap) / 2;
        } else {
            /* turn x */
            lMid = (pieces.lGap - 1) / 2;
        }
        SDWORD lGain_o = lMid;
        SDWORD lGain_x = pieces.lGap - lMid - 1;
#if 0
        printf("gap:%d mid:%d base:%d co:%d\n", 
                pieces.lGap, lMid, pieces.lBaseScore_o, pieces.lCount_o);
#endif
        sqTtlCnt_o += (SQWORD)((SQWORD)pieces.lBaseScore_o + (SQWORD)lGain_o * (SQWORD)pieces.lCount_o);       
        sqTtlCnt_x += (SQWORD)((SQWORD)pieces.lBaseScore_x + (SQWORD)lGain_x * (SQWORD)pieces.lCount_x);
        lTurn += pieces.lGap - 1;       
    }

//    printf("%lld %lld %d %d\n", sqTtlCnt_o, sqTtlCnt_x, lMinDist_o, lMinDist_x);
    if ((lMinDist_o != SDWORD_INF) || (lMinDist_x != SDWORD_INF)) {
        if (lMinDist_o <= lMinDist_x) {
            printf("o\n");
        } else {
            printf("x\n");
        }
    } else {
        if (sqTtlCnt_x < sqTtlCnt_o) {
            printf("o\n");
        } else {
            printf("x\n");
        }
    }

    return 0;
}