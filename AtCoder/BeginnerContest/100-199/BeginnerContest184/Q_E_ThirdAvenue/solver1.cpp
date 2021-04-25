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
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

static inline SQWORD ABS(SQWORD a) { return 0 < a ? a : -a; }

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

#define MAX_H   (2000)
#define MAX_W   (2000)

int main(void)
{
    SQWORD sqH = inputSQWORD();
    SQWORD sqW = inputSQWORD();

    static SDWORD s_aalMap[MAX_H + 2][MAX_W + 2];

    vector<vector<pair<SDWORD, SDWORD>>> vvpTeleportMap(27);
    pair<SQWORD, SQWORD> pairPosStart;
    pair<SQWORD, SQWORD> pairPosGoal;

    for (SQWORD sqRow = 1; sqRow <= sqH; sqRow++) {
        string strS;
        cin >> strS;
        for (SQWORD sqCol = 1; sqCol <= sqW; sqCol++) {
            char c = strS[sqCol - 1];
            if ('#' == c) {
                ;
            } else {
                s_aalMap[sqRow][sqCol] = 1000;
                if ('.' == c) {
                    ;
                } else if ('S' == c) {
                    pairPosStart = make_pair(sqRow, sqCol);
                } else if ('G' == c) {
                    pairPosGoal = make_pair(sqRow, sqCol);
                } else {
                    SDWORD lTeleportIdx = c - 'a' + 1;
                    s_aalMap[sqRow][sqCol] = lTeleportIdx;
                    vvpTeleportMap[lTeleportIdx].emplace_back(make_pair(sqRow, sqCol));
                }
            }
        }
    }

    vector<pair<SQWORD, SQWORD>> vDir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    vector<vector<SQWORD>> vvDist(sqH + 2, vector<SQWORD>(sqW + 2, -1));

    queue<pair<SQWORD, SQWORD>> queSearch;
    queSearch.push(pairPosStart);
    vvDist[pairPosStart.first][pairPosStart.second] = 0;
    while(!queSearch.empty()) {
        auto cur_pos = queSearch.front();

        queSearch.pop();

        SQWORD sqCurDist = vvDist[cur_pos.first][cur_pos.second];
        for (auto dir: vDir) {
            SQWORD sqNextR = cur_pos.first + dir.first;
            SQWORD sqNextC = cur_pos.second + dir.second;

            if (s_aalMap[sqNextR][sqNextC] != 0) {
                if (vvDist[sqNextR][sqNextC] == -1) {
                    vvDist[sqNextR][sqNextC] = sqCurDist + 1;
                    queSearch.push(make_pair(sqNextR, sqNextC));
                }
            }
        }

        if (s_aalMap[cur_pos.first][cur_pos.second] != 1000) {
            SDWORD lTeleportIdx = s_aalMap[cur_pos.first][cur_pos.second];
            for (auto dst: vvpTeleportMap[lTeleportIdx]) {
                SQWORD sqNextR = dst.first;
                SQWORD sqNextC = dst.second;
//                printf("--->>>>W %lld %lld\n", sqNextR, sqNextC);
                if (s_aalMap[sqNextR][sqNextC] != 0) {
                    if (vvDist[sqNextR][sqNextC] == -1) {
                        vvDist[sqNextR][sqNextC] = sqCurDist + 1;
                        queSearch.push(make_pair(sqNextR, sqNextC));
                    }
                }
            }
            vvpTeleportMap[lTeleportIdx].clear();
        }
    }

    printf("%lld\n", vvDist[pairPosGoal.first][pairPosGoal.second]);


 
    return 0;
}
