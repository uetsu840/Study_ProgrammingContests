#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
using namespace std;

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
using WORD = unsigned short;
using SWORD = short;
using BYTE = unsigned char;
using SBYTE = char;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0xFFFFFFFFFFFFFFFF)

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

#define MAX_FACTORY (60)
static DWORD adwFactoryNum[MAX_FACTORY];
static DWORD adwFactoryCumMul[MAX_FACTORY];
static DWORD s_dwFactoryNum = 0;

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

#define MAX_MACHINE_NUM    (100000)

struct SEGMENT_NODE_ST {
    DOUBLE dA;
    DOUBLE dB;

public:
    void init() {
        dA = 1.0;
        dB = 0.0;
    }
};


static SEGMENT_NODE_ST s_astMachines[MAX_MACHINE_NUM];

struct SegmentTree {
private:
    DWORD dwBaseSize;
    vector<SEGMENT_NODE_ST> vNode;

private:
    static inline void updateNode(
        const SEGMENT_NODE_ST &stL,
        const SEGMENT_NODE_ST &stR,
        SEGMENT_NODE_ST &stRet)
    {
        stRet.dA = stR.dA * stL.dA;
        stRet.dB = stR.dA * stL.dB + stR.dB;
    }

    DWORD initSegmentTree(vector<SEGMENT_NODE_ST> &v) {
        /**
         *  最下段のノード数は元配列のサイズ以上になる最小の 2 冪 -> これを n とおく
         * セグメント木全体で必要なノード数は 2n-1 個である
         */
        DWORD dwSize = v.size();
        dwBaseSize = 1; 
        while(dwBaseSize < dwSize) {
            dwBaseSize *= 2;
        }
        vNode.resize(2 * dwBaseSize - 1);

        /**
         *  最下段に値を入れたあとに、下の段から順番に値を入れる
         * 値を入れるには、自分の子の 2 値を参照すれば良い
         */
        for(DWORD dwIdx = 0; dwIdx < dwBaseSize; dwIdx++) {
            vNode[dwBaseSize + dwIdx - 1].init();
        }
        for(DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vNode[dwBaseSize + dwIdx - 1] = v[dwIdx];
        }
        for(SDWORD lIdx = dwBaseSize-2; lIdx >= 0; lIdx--) {
            updateNode(vNode[2 * lIdx + 1], vNode[2 * lIdx + 2], vNode[lIdx]);
        }
    }

public:
    /* 元配列 v をセグメント木で表現する */
    SegmentTree(vector<SEGMENT_NODE_ST> v) {
        initSegmentTree(v);
    }

    void update(DWORD dwNodeIdx, const SEGMENT_NODE_ST &stNewVal) {
        // 最下段のノードにアクセスする
        SDWORD lTreeIdx = dwNodeIdx + (dwBaseSize - 1);

        // 最下段のノードを更新したら、あとは親に上って更新していく
        vNode[lTreeIdx] = stNewVal;
        while(lTreeIdx > 0) {
            lTreeIdx = (lTreeIdx - 1) / 2;
            updateNode(vNode[2*lTreeIdx+1], vNode[2*lTreeIdx+2], vNode[lTreeIdx]);
        }
    }

    void PrintNode(DWORD dwIdx)
    {
        printf("[%d]->a:%f b:%f\n", dwIdx, vNode[dwIdx].dA, vNode[dwIdx].dB);
    }

    DOUBLE getTotalValue() {
        return vNode[0].dA * 1.0 + vNode[0].dB;
    }
};


int main()
{
    DWORD dwInput_N;
    DWORD dwInput_M;
    DWORD dwAnswer;

    dwInput_N = inputSDWORD();
    dwInput_M = inputSDWORD();

    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_astMachines); dwIdx++) {
        s_astMachines[dwIdx].init();
    }

    vector<SEGMENT_NODE_ST> vMachines(s_astMachines, s_astMachines + ArrayLength(s_astMachines));
    SegmentTree seg(vMachines);

    DOUBLE dMax = 1.0;
    DOUBLE dMin = 1.0;

    for (DWORD dwChgIdx = 0; dwChgIdx < dwInput_M; dwChgIdx++) {
        SEGMENT_NODE_ST stNode;
        DOUBLE dValue;

        DWORD dwNodeIdx = inputSDWORD();

        stNode.dA = inputFP();
        stNode.dB = inputFP();
//        printf("input %f %f\n", stNode.dA, stNode.dB);
        seg.update(dwNodeIdx, stNode);
        dValue = seg.getTotalValue();
        dMax = max(dValue, dMax);
        dMin = min(dValue, dMin);
//        printf("%f\n", seg.getTotalValue());
    }
    printf("%0.9f\n", dMin);
    printf("%0.9f\n", dMax);

    return 0;
}
