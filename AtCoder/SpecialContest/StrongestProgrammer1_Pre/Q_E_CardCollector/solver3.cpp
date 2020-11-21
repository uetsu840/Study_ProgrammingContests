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
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

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

typedef struct {
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    DWORD   dwSize;

    void initUnionFind(
        DWORD dwSize)
    {
        dwSize = dwSize;
        vdwPar.resize(dwSize);
        vdwRank.resize(dwSize);
        vdwCnt.resize(dwSize);
    
        for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vdwPar[dwIdx]  = dwIdx;
            vdwRank[dwIdx] = 0;
            vdwCnt[dwIdx]  = 1;
        }
    }

    DWORD ufGetCnt(DWORD sqIdx) {
        return vdwCnt[ufGetParent(sqIdx)];
    }


    DWORD ufGetParent(DWORD dwIdx) const
    {
        return vdwPar[dwIdx];
    }

    DWORD ufGetRank(DWORD dwIdx) const
    {
        return vdwRank[dwIdx];
    }

    void ufSetParent(DWORD dwIdx, DWORD dwParent)
    {
        vdwPar[dwIdx] = dwParent; 
        if (ufGetRank(dwIdx) == ufGetRank(dwParent)) {
            (vdwRank[dwParent])++;
        }
    }

    DWORD ufGetRoot(DWORD dwIdx) const
    {
        if (ufGetParent(dwIdx) == dwIdx) {
            return dwIdx;
        } else {
            DWORD dwParent = ufGetParent(dwIdx);
            DWORD dwRoot = ufGetRoot(dwParent);
            return dwRoot;
        }
    }

    bool ufUnite(DWORD dwX, DWORD dwY)
    {
        DWORD dwRootX = ufGetRoot(dwX);
        DWORD dwRootY = ufGetRoot(dwY);

        if (dwRootX == dwRootY) {
            return false;
        }

        if (ufGetRank(dwRootX) < ufGetRank(dwRootY)) {
            ufSetParent(dwRootX, dwRootY);
            (vdwCnt[dwRootY]) += (vdwCnt[dwRootX]);
        } else {
            ufSetParent(dwRootY, dwRootX);
            (vdwCnt[dwRootX]) += (vdwCnt[dwRootY]);
        }

        return true;
    }

    bool ufIsSame(DWORD dwX, DWORD dwY) const
    {
        return (ufGetRoot(dwX)  == ufGetRoot(dwY));
    }
} ST_UNION_FIND;

/*-----------------------------------------------------*/

struct ST_CARD {
	SQWORD sqA;
	SQWORD sqR;
	SQWORD sqC;

	ST_CARD(SQWORD a, SQWORD r, SQWORD c) : sqA(a), sqR(r), sqC(c) {};
};


bool operator< (const ST_CARD &a, const ST_CARD &b)
{
	return a.sqA < b.sqA;
}


bool operator> (const ST_CARD &a, const ST_CARD &b)
{
	return a.sqA > b.sqA;
}

static SQWORD pickOneGroup(SQWORD sqH, SQWORD sqW, const vector<ST_CARD> &vstCards,
    vector<SQWORD> &vsqRowPickCnts,
    vector<SQWORD> &vsqColPickCnts,
    vector<set<SQWORD>> &vsqPickCols,
    vector<set<SQWORD>> &vsqPickRows,
    set<SQWORD> &setRowPicks,
    set<SQWORD> &setColPicks)
{
    SQWORD sqAns = 0;
    SQWORD sqPickCardCnt = 0;
    setRowPicks.clear();
    setColPicks.clear();
    for (auto card: vstCards) {
        
        SQWORD sqPickRowCnt = vsqPickCols[card.sqR].size();
        SQWORD sqPickColCnt = vsqPickRows[card.sqC].size();

        if (vsqPickCols[card.sqR].find(card.sqC) == vsqPickCols[card.sqR].end()) {
            sqPickRowCnt++;
        }
        if (vsqPickRows[card.sqC].find(card.sqR) == vsqPickRows[card.sqC].end()) {
            sqPickColCnt++;
        }

        SQWORD sqTtlPickRow = setRowPicks.size();
        if (setRowPicks.find(card.sqR) == setRowPicks.end()) {
            sqTtlPickRow++;
        }
        SQWORD sqTtlPickCol = setColPicks.size();
        if (setColPicks.find(card.sqC) == setColPicks.end()) {
            sqTtlPickCol++;
        }

#if 1
        printf("card [%lld] (%lld, %lld)\n", card.sqA, card.sqR, card.sqC);
        printf("   rowpick[%lld] colpick[%lld] row num[%lld] col num[%lld]\n",
                vsqRowPickCnts[card.sqR], vsqColPickCnts[card.sqC],
                sqPickRowCnt, sqPickColCnt);
#endif            

        if ((vsqRowPickCnts[card.sqR] < sqPickRowCnt + 1)
            || (vsqColPickCnts[card.sqC] < sqPickColCnt + 1)) {
            if (sqPickCardCnt < sqTtlPickRow + sqTtlPickCol) {
                /* カードを取れる */
                printf(">>>pick\n");
                sqPickCardCnt++;
                vsqPickCols[card.sqR].insert(card.sqC);
                vsqPickRows[card.sqC].insert(card.sqR);
                setRowPicks.insert(card.sqR);
                setColPicks.insert(card.sqC);
                    
                vsqRowPickCnts[card.sqR]++;
                vsqColPickCnts[card.sqC]++;
                sqAns += card.sqA;

                if (sqPickCardCnt == sqTtlPickRow + sqTtlPickCol) {
                    sqPickCardCnt -= (sqTtlPickRow + sqTtlPickCol);
                    vsqPickCols[card.sqR].clear();
                    vsqPickRows[card.sqC].clear();
                    setRowPicks.erase(card.sqR);
                    setColPicks.erase(card.sqC);                    
                }
            }
        }
    }

    return sqAns;
}



int main(void)
{
	SQWORD sqN = inputSQWORD();
	SQWORD sqH = inputSQWORD();
	SQWORD sqW = inputSQWORD();

	vector<ST_CARD> vstCards;

	for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
		SQWORD sqR = inputSQWORD();
		SQWORD sqC = inputSQWORD();
		SQWORD sqA = inputSQWORD();

        sqR--;
        sqC--;
		vstCards.emplace_back(sqA, sqR, sqC);
	}

    ST_UNION_FIND uf;

    uf.initUnionFind(sqH + sqW);
    for (auto card: vstCards) {
        uf.ufUnite(card.sqR, sqH + card.sqC);
    }

    vector<vector<ST_CARD>> vstCardsGroup(sqH + sqW);

    for (auto card: vstCards) {
        SQWORD sqParent = uf.ufGetParent(card.sqR);
        vstCardsGroup[sqParent].emplace_back(card);
    }

    SQWORD sqAns = 0;

    vector<SQWORD> vsqRowPickCnts(sqH, 0);
    vector<SQWORD> vsqColPickCnts(sqW, 0);
    vector<set<SQWORD>> vsqPickCols(sqH);
    vector<set<SQWORD>> vsqPickRows(sqW);
    set<SQWORD> setRowPicks;
    set<SQWORD> setColPicks;

    for (auto cardGroup: vstCardsGroup) {
        if (0 < cardGroup.size()) {
//            printf("GR: %lld\n", uf.ufGetParent(cardGroup.begin()->sqR));
            sort(cardGroup.begin(), cardGroup.end(), greater<ST_CARD>());

            SQWORD sqPickedCard = pickOneGroup(sqH, sqW, cardGroup,
                vsqRowPickCnts, vsqColPickCnts,
                vsqPickCols, vsqPickRows,
                setRowPicks, setColPicks);
            sqAns += sqPickedCard;
        }
    }

    printf("%lld\n", sqAns);

    return 0;
}

