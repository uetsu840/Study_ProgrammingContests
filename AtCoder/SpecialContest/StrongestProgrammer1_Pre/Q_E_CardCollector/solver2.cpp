#include <iostream>


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

using QWORD = uint64_t;
using SQWORD = int64_t;
using DWORD = uint32_t;
using SDWORD = int32_t;
using WORD = uint16_t;
using SWORD = int16_t;
using BYTE = uint8_t;
using SBYTE = int8_t;
using DOUBLE = double;
using FLOAT = float;

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
static inline DOUBLE ABS(DOUBLE a) { return 0 <= a ? a : -a; }

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
				return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0, (DOUBLE)dwFpCnt)));
			}
		}
	}
}



/*----------------------------------------------*/

#define MAX_H   (300000)
#define MAX_W   (300000)


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

int main(void)
{
	SQWORD sqN = inputSQWORD();
	SQWORD sqH = inputSQWORD();
	SQWORD sqW = inputSQWORD();

	vector<ST_CARD> vstCards;
    vector<vector<ST_CARD>> vsqCardsRow(sqH);
    vector<vector<ST_CARD>> vsqCardsCol(sqW);

	for (SQWORD sqIdx = 0; sqIdx < sqN; sqIdx++) {
		SQWORD sqR = inputSQWORD();
		SQWORD sqC = inputSQWORD();
		SQWORD sqA = inputSQWORD();

        sqR--;
        sqC--;

        vsqCardsRow[sqR].emplace_back(sqA, sqR, sqC);
        vsqCardsCol[sqC].emplace_back(sqA, sqR, sqC);
		vstCards.emplace_back(sqA, sqR, sqC);
	}

    vector<SQWORD> vsqPicksRow(sqH, 0);
    vector<SQWORD> vsqPicksCol(sqW, 0);

	sort(vstCards.begin(), vstCards.end(), greater<ST_CARD>());

    for (auto it = vsqCardsRow.begin(); it != vsqCardsRow.end(); ++it) {
        sort(it->begin(), it->end());
    }
    for (auto it = vsqCardsCol.begin(); it != vsqCardsCol.end(); ++it) {
        sort(it->begin(), it->end());
    }

    SQWORD sqAns = 0;
    for (auto card: vstCards) {
//        printf("c: %lld\n", card.sqA);
        vsqCardsCol[card.sqC].pop_back();
        vsqCardsRow[card.sqR].pop_back();

        if ((0 == vsqPicksRow[card.sqR]) && (0 == vsqPicksCol[card.sqC])) {
            SQWORD sqRowSecond = -1;
            SQWORD sqColSecond = -1;
            for (auto it = vsqCardsRow[card.sqR].rbegin(); it != vsqCardsRow[card.sqR].rend(); ++it) {
                if (1 == vsqPicksCol[it->sqC]) {
                    sqRowSecond = it->sqA;
                    break;
                }                
            }
            for (auto it = vsqCardsCol[card.sqC].rbegin(); it != vsqCardsCol[card.sqC].rend(); ++it) {
                if (1 == vsqPicksRow[it->sqR]) {
                    sqColSecond = it->sqA;
                    break;
                }                
            }
            if (sqRowSecond < sqColSecond) {
                vsqPicksRow[card.sqR]++;
            } else {
                vsqPicksCol[card.sqC]++;
            }
            sqAns += card.sqA;
        } else if (0 == vsqPicksRow[card.sqR]) {
            vsqPicksRow[card.sqR]++;
            sqAns += card.sqA;
        } else if (0 == vsqPicksCol[card.sqC]) {
            vsqPicksCol[card.sqC]++;
            sqAns += card.sqA;
        }
    }
//    printf("hogheoge\n");

	printf("%lld\n", sqAns);

	return 0;
}
