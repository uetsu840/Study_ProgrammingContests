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
static void printVector(vector<SQWORD> v)
{
    for (auto m: v) {
        printf("%lld ", m);
    }
    printf("\n");
}

/*-----------------------------------------------------*/

struct stLine {
    SQWORD sqMoney;
    SQWORD sqTime;
    SQWORD sqDst;

    stLine(SQWORD a, SQWORD b, SQWORD d) : sqMoney(a), sqTime(b), sqDst(d) {}; 
};

const SQWORD MAX_NODE = 50;
const SQWORD MAX_MONEY = MAX_NODE * 50 + 5;

static SQWORD s_asqDp[MAX_NODE][MAX_MONEY];

struct stState {
    SQWORD sqNode;      /* 頂点 */
    SQWORD sqMoney;     /* 所持金 */  
    SQWORD sqTime;      /* 最小の所要時間 */

    stState(SQWORD n, SQWORD m, SQWORD t) : sqNode(n), sqMoney(m), sqTime(t) {};

    bool operator<(const stState &a) const {
        return sqTime > a.sqTime;
    }

};

struct stExchenge {
    SQWORD sqC;
    SQWORD sqD;

    stExchenge(SQWORD c, SQWORD d) : sqC(c), sqD(d) {};
};

void pushQue(
    priority_queue<stState> &que, 
    SQWORD sqNode, 
    SQWORD sqMoney, 
    SQWORD sqTime)
{
    if (sqMoney < 0) {
        return;
    }

    if (s_asqDp[sqNode][sqMoney] <= sqTime) {
        return;
    } 

    s_asqDp[sqNode][sqMoney] = sqTime;
    que.emplace(sqNode, sqMoney, sqTime);
}


int main(void)
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqS = inputSQWORD();

    vector<stLine> vstLines[MAX_NODE];

    for (SQWORD sqLineIdx = 0; sqLineIdx < sqM; sqLineIdx++) {
        SQWORD sqU = inputSQWORD();
        SQWORD sqV = inputSQWORD();
        SQWORD sqA = inputSQWORD();
        SQWORD sqB = inputSQWORD();

        sqU--;  /* 0-indexed */
        sqV--;  /* 0-indexed */

        vstLines[sqU].emplace_back(sqA, sqB, sqV);
        vstLines[sqV].emplace_back(sqA, sqB, sqU);
    }

    vector<stExchenge> vstExchanges;
    for (SQWORD sqNodeIdx = 0; sqNodeIdx < sqN; sqNodeIdx++) {
        SQWORD sqC = inputSQWORD();
        SQWORD sqD = inputSQWORD();

        vstExchanges.emplace_back(sqC, sqD);
    }

    priority_queue<stState> que;

    /* DPテーブルの初期化 */
    for (SQWORD sqNodeIdx = 0; sqNodeIdx < MAX_NODE; sqNodeIdx++) {
        for (SQWORD sqMoneyIdx = 0; sqMoneyIdx < MAX_MONEY; sqMoneyIdx++) {
            s_asqDp[sqNodeIdx][sqMoneyIdx] = MAX_SQWORD;
        }
    }

    sqS = min(sqS, MAX_MONEY - 1);
    pushQue(que, 0, sqS, 0);

    while (!que.empty()) {
        stState stCurState = que.top();
        que.pop();
        
        SQWORD sqNode  = stCurState.sqNode;
        SQWORD sqMoney = stCurState.sqMoney;
        SQWORD sqTime  = stCurState.sqTime;

        if (s_asqDp[sqNode][sqMoney] == sqTime) {
            SQWORD sqNextMoney = min (sqMoney + vstExchanges[sqNode].sqC, MAX_MONEY - 1);
         
            pushQue(que, sqNode, sqNextMoney, sqTime + vstExchanges[sqNode].sqD);
            for (stLine l: vstLines[sqNode]) {
                pushQue(que, l.sqDst, sqMoney - l.sqMoney, sqTime + l.sqTime);
            }
        }
    }

    for (SQWORD sqNodeIdx = 1; sqNodeIdx < sqN; sqNodeIdx++) {
        SQWORD sqAns = MAX_SQWORD;

        for (SQWORD sqMoney = 0; sqMoney < MAX_MONEY; sqMoney++) {
            sqAns = min(sqAns, s_asqDp[sqNodeIdx][sqMoney]);
        }
        printf("%lld\n", sqAns);
    }

    return 0;
}