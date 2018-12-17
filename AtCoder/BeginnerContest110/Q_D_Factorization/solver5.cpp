/**
 * 解説見た。
 */
#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
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

#define MAX_FACTORY (100)
static DWORD s_adwFactoryCnt[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

M_BOOL calc_PrimeFactorization(DWORD dwNum)
{
    DWORD dwCurNum = dwNum;
    DWORD dwCeiling = 32000;

    for (DWORD dwDiv = 2; dwDiv <= dwCeiling; dwDiv++) {
        if (0 == (dwCurNum % dwDiv)) {
            DWORD dwFactoryCnt = 0;
            while(1) {
                dwCurNum /= dwDiv;
                dwFactoryCnt++;
                if (0 != dwCurNum % dwDiv) {
                    break;
                }   
            }
            s_adwFactoryCnt[s_dwFactoryNum] = dwFactoryCnt;
            s_dwFactoryNum++;
        }
    }

    if (1 != dwCurNum) {
        s_adwFactoryCnt[s_dwFactoryNum] = 1;
        s_dwFactoryNum++;
    }

    return M_TRUE;
}


static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    M_BOOL bRead = M_FALSE;
    for (;;) {
        char c = getchar();
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        } else {
            if (bRead) {
                return lNumber;
            }
        }
    }
    return 0;
}

using ll = long long;

const ll MOD = 1000000007LL;
 
ll ADD(ll x, ll y) { return (x + y) % MOD; }
ll SUB(ll x, ll y) { return (x - y + MOD) % MOD; }
ll MUL(ll x, ll y) { return x * y % MOD; }
ll POW(ll x, ll e) {
  ll v = 1;
  for (; e; x = MUL(x, x), e >>= 1)
    if (e & 1) v = MUL(v, x);
  return v;
}
ll DIV(ll x, ll y) { /*assert(y%MOD!=0);*/
  return MUL(x, POW(y, MOD - 2));
}
ll comb(ll n, ll k) { ll v=1; for(ll i=1; i<=k; i++) v = DIV(MUL(v, n-i+1),i); return v; }

int main()
{
    DWORD dwInput_N, dwInput_M;
 
    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    memset(s_adwFactoryCnt, 0, sizeof(s_adwFactoryCnt));

    calc_PrimeFactorization(dwInput_M);

    DWORD dwAns = 1;
    for (DWORD dwIdx = 0; dwIdx < s_dwFactoryNum; dwIdx++) {
        DWORD dwFactory = s_adwFactoryCnt[dwIdx];
        DWORD dwComb = comb(dwFactory + dwInput_N - 1, dwInput_N - 1);
        dwAns = MUL(dwAns, dwComb);
    }
    printf("%d", dwAns);

    return 0;
}
