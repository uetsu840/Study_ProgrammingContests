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

/*=========================================================*/
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

/*=========================================================*/
#define COMB_MAX 1002

vector<SQWORD> vsqFactory;
vector<SQWORD> vsqFactoryRev(COMB_MAX);
vector<vector<SQWORD>> vvPower(COMB_MAX, vector<SQWORD>(COMB_MAX));
vector<vector<SQWORD>> vvCombination(COMB_MAX, vector<SQWORD>(COMB_MAX));

/*=========================================================*/

static void initFactory(void)
{
    /* 階乗の事前計算 */
    SQWORD sqFactory = 1;
    for (SQWORD sqNum = 1; sqNum < COMB_MAX; sqNum++) {
        vsqFactory.emplace_back(sqFactory);
        sqFactory *= sqNum;
        sqFactory %= ANS_MOD;
    }
}

static void initPower(void) 
{
    for (SQWORD sqBase = 0; sqBase < COMB_MAX; sqBase++) {
        SQWORD sqNum = 1;
        for (SQWORD sqPower = 0; sqPower < COMB_MAX; sqPower++) {
            vvPower[sqBase][sqPower] = sqNum;

            sqNum *= vsqFactory[sqBase];
            sqNum %= ANS_MOD;
        }
    }
}

static void initCombination(void)
{
    for (SQWORD sqIdxN = 0; sqIdxN < COMB_MAX; sqIdxN++) {
        for (SQWORD sqIdxK = 0; sqIdxK <= sqIdxN; sqIdxK++) {
            MODINT comb = MODINT(vsqFactory[sqIdxN]) / MODINT(vsqFactory[sqIdxK] * vsqFactory[sqIdxN - sqIdxK]);
            vvCombination[sqIdxN][sqIdxK] = comb.getVal();
        }
    }
}

static void initRevFactory(void)
{
    for (SQWORD sqNum = 0; sqNum < COMB_MAX; sqNum++) {
        vsqFactoryRev[sqNum] = (MODINT(vsqFactory[sqNum]).inv()).getVal();
    }
}

#define MAX_N  (1000)
#define DPTBL_SIZE  (MAX_N + 1)

int main()
{
    SQWORD sqN = inputSQWORD();
    SQWORD sqA = inputSQWORD();
    SQWORD sqB = inputSQWORD();
    SQWORD sqC = inputSQWORD();
    SQWORD sqD = inputSQWORD();

    initFactory();
    initPower();
    initCombination();
    initRevFactory();

    /* dp(i)[j] グループの人数 i までを考えたとき、人数jで実現される場合の数 */
    vector<SQWORD>  vsqDpTbl(DPTBL_SIZE);
    vsqDpTbl[0] = 1;
    for (SQWORD sqGroupMem = sqA; sqGroupMem <= sqB; sqGroupMem++) {
        vector<SQWORD>  vsqDpTblNext(DPTBL_SIZE);
        for (SQWORD sqDpIdx = 0; sqDpIdx <= sqN; sqDpIdx++) {
            /* ない */
            vsqDpTblNext[sqDpIdx] += vsqDpTbl[sqDpIdx];
            vsqDpTblNext[sqDpIdx] %= ANS_MOD;

            SQWORD sqRestNum = sqN - sqDpIdx;
#if 0
            printf("[%lld %lld] rest: %lld\n", sqGroupMem, sqDpIdx, sqRestNum);
#endif
            /* ある */
            SQWORD sqMul = 1;
            if (0 <= sqRestNum - (sqC - 1) * sqGroupMem) {
                for (SQWORD sqIdx = 0; sqIdx < sqC; sqIdx++) {
                    sqMul *= vvCombination[sqRestNum - sqIdx * sqGroupMem][sqGroupMem];
                    sqMul %= ANS_MOD;
                }
            }

            for (SQWORD sqGroupNum = sqC; sqGroupNum <= sqD; sqGroupNum++) {
                SQWORD sqMem = sqGroupNum * sqGroupMem;
                if (sqMem <= sqRestNum) {
#if 0
                    SQWORD sqDpAdd = (MODINT(vsqDpTbl[sqDpIdx] * vsqFactory[sqRestNum])
                                          / MODINT((((vvPower[sqGroupMem][sqGroupNum] 
                                                    * vsqFactory[sqRestNum - sqMem]) % ANS_MOD)
                                                    * vsqFactory[sqGroupNum]) % ANS_MOD)).getVal(); 
#endif
                    SQWORD sqAdd = 1;
                    sqAdd *= vsqDpTbl[sqDpIdx];
                    sqAdd %= ANS_MOD;

                    sqAdd *= sqMul;
                    sqAdd %= ANS_MOD;

                    sqMul *= vvCombination[sqRestNum - sqGroupNum * sqGroupMem][sqGroupMem];
                    sqMul %= ANS_MOD;
//                    printf("   :%lld\n", sqMul);

                    sqAdd *= vsqFactoryRev[sqGroupNum];
                    sqAdd %= ANS_MOD;
#if 0
                    printf("%lld [gr:%lld] [rest:%lld]\n", sqAdd, sqGroupNum, sqRestNum);
                    printf(">>> %lld %lld %lld %lld\n", vsqDpTbl[sqDpIdx],
                                                vvPower[sqGroupMem][sqGroupNum],
                                                vsqFactory[sqRestNum - sqMem],
                                                vsqFactory[sqGroupNum]);
#endif
                    vsqDpTblNext[sqDpIdx + sqMem] += sqAdd;
                    vsqDpTblNext[sqDpIdx + sqMem] %= ANS_MOD;
                }
            }
        }

        swap(vsqDpTblNext, vsqDpTbl);

#if 0
        printf("====\n");
        for (SQWORD sqIdx = 0; sqIdx <= sqN; sqIdx++) {
            printf("%lld ", vmiDpTbl[sqIdx]);
        }
        printf("\n");
#endif
    }

    MODINT miAns = vsqDpTbl[sqN];

    printf("%lld\n", miAns);
}