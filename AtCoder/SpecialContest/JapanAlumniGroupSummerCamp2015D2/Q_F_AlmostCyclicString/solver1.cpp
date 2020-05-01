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

static SQWORD powSqword(SQWORD val, SQWORD t)
{
    if (!t) {
        return 1;
    }
    SQWORD a = powSqword(val, t>>1);
    a *= a;
    if (t&1) {
        a *= val;
    } 
    return a;
}

/*----------------------------------------------*/

/**
 *  rolling hash 
 */
#define MS  (2)
const long long base[] = {999999937LL, 1000000007LL};

struct hash_val {
    SQWORD asqHash[MS];

    const bool operator== (const hash_val &v) {
        return 0 == memcmp(this, &v, sizeof(*this));
    };

    const bool operator!= (const hash_val &v) {
        return 0 != memcmp(this, &v, sizeof(*this));
    };
};

struct rolling_hash {
    int n;
    vector<long long> hs[MS], pw[MS];
    rolling_hash(){}
    rolling_hash(const string &s) {
        n = s.size();
        for (int i = 0; i < MS; i++) {
            hs[i].assign(n+1,0);
            pw[i].assign(n+1,0);
            hs[i][0] = 0;
            pw[i][0] = 1;
            for (int j = 0; j < n; j++) {
                pw[i][j+1] = pw[i][j]*base[i];
                hs[i][j+1] = (hs[i][j]*base[i]+s[j]);
            }
        }
    }
    hash_val hash(int l, int r) const { 
//        printf("<%d %d>\n", l, r);
        hash_val ret;
        for (int i = 0; i < MS; i++) {
            ret.asqHash[i] = ((hs[i][r]-hs[i][l]*pw[i][r-l])); 
        }
        return ret;
    }

    bool match(int l1, int r1, int l2, int r2) {
        bool ret = false;
        hash_val h1 = hash(l1,r1);
        hash_val h2 = hash(l2,r2);

        return (h1 == h2);
    }

    bool match(int l, int r, hash_val h) {
        bool ret = 1;
        return (h == hash(l,r));
    }

    /**
     * LCP (Longest Common Prefix)
     * O( log N )
     */
    SDWORD getLCP(int a, int b) const {
        int len = min((int) hs[0].size() - a, (int) hs[0].size() - b);
        int low = 0, high = len;
        while (high - low > 1) {
            int mid = (low + high) >> 1;
            if (hash(a, a + mid) != hash(b, b + mid)) {
                high = mid;
            } else {
                low = mid;
            }
        }
        return low;
    }
  
};

static bool judge(SQWORD sqA, SQWORD sqB)
{
    for (SQWORD sqD = -27; sqD <= 27; sqD++) {
        if (sqA == sqB * sqD) {
            return true;
        }
    }
    return false;
}

int main(void)
{
    string strS;

    cin >> strS;

    rolling_hash h(strS);

    SQWORD sqQ = inputSQWORD();
    for (SQWORD sqIdx = 0; sqIdx < sqQ; sqIdx++) {
        SQWORD sqL = inputSQWORD();
        SQWORD sqR = inputSQWORD();
        SQWORD sqT = inputSQWORD();

        hash_val h1 = h.hash(sqL - 1,       sqR - sqT);
        hash_val h2 = h.hash(sqL + sqT - 1, sqR);

        SQWORD sqStrLen = (sqR - sqL + 1) - sqT;

        if (0 == sqStrLen) {
            printf("Yes\n");
        } else {
            SDWORD lPos  = h.getLCP(sqL - 1, sqL + sqT - 1);

            if ((sqR - sqL - sqT) <= lPos) {
                printf("Yes\n");
            } else {
                SQWORD sqModDiff1 = powSqword(base[0], (sqR - sqL - sqT) - lPos);
                SQWORD sqModDiff2 = powSqword(base[1], (sqR - sqL - sqT) - lPos);
                
                SQWORD sqHashDiff1 = (h1.asqHash[0] - h2.asqHash[0]);
                SQWORD sqHashDiff2 = (h1.asqHash[1] - h2.asqHash[1]);

                printf("h1 [%lld %lld], h2[%lld %lld]\n", h1.asqHash[0], h1.asqHash[1], h2.asqHash[0], h2.asqHash[1]);
                printf("pos [%d], %lld %lld\n", lPos, sqHashDiff1, sqHashDiff2);

                if (judge(sqHashDiff1, sqModDiff1)
                    && judge(sqHashDiff2, sqModDiff2)) {
                    printf("Yes\n");
                } else {
                    printf("No\n");
                }
            }
        }
    }

    return 0;
}