using namespace std;
#include <iostream>
#include <algorithm> // sort, for_each, random_shuffle
#include <stdlib.h>
#include <string.h>
#include <math.h>

using QWORD = unsigned long long;
using SQWORD = long long;
using DWORD = unsigned int;
using SDWORD = int;
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

#define OBJ_NUM_MAX (16)
#define OBJ_POS_NUM (16)
#define DP_ARY_SIZE (1 << OBJ_NUM_MAX)
static DOUBLE s_adExVal[DP_ARY_SIZE];

static void initDp(void)
{
    /*
    *   initialize dp 
    *     single object : Expected Value = 3
    *     otherwise     : Expected Value = -1 (unknown)
    */
    printf("initDp Entry");
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(s_adExVal); dwIdx++) {
        s_adExVal[dwIdx] = INFINITY;
    }
    printf("****");
    for (DWORD dwIdx = 0; dwIdx < OBJ_NUM_MAX; dwIdx++) {
        s_adExVal[0x1 << dwIdx] = 3.0;
    }
    printf("---\n");
}

static inline DWORD getNeighbors(DWORD dwObjMap, DWORD dwTargetIdx)
{
    return (((dwObjMap) >> dwTargetIdx) & 0x7);
}

static DOUBLE dp(DWORD dwObjMap)
{
    printf("-- dp[0x%x]\n", dwObjMap);

    /* hit memo */
    DWORD dwDpMapIdx = dwObjMap >> 1;
    if (s_adExVal[dwDpMapIdx] != INFINITY) {
        printf("hit >> %f\n", s_adExVal[dwDpMapIdx]);
        return s_adExVal[dwDpMapIdx];
    }

    /* not hit memo */
    DOUBLE adExValTmp[OBJ_POS_NUM];
    for (DWORD dwTargetPosIdx = 1; dwTargetPosIdx < 1 + OBJ_POS_NUM; dwTargetPosIdx++) {
        DWORD dwNeighbor = getNeighbors(dwObjMap, dwTargetPosIdx);
        switch(dwNeighbor) {
        case 0:
            adExValTmp[dwTargetPosIdx - 1] = 0;
            break;
        case 1:
        case 2:
        case 4:
            {
                DWORD dwObjMapOnHit = dwObjMap ^ (dwNeighbor << (dwTargetPosIdx));
                printf("   0x%x, %d, next: 0x%x\n", dwNeighbor, dwTargetPosIdx, dwObjMapOnHit);
                adExValTmp[dwTargetPosIdx - 1] = (3 + dp(dwObjMapOnHit)) / 3.0; 
            }
            break;
        case 3:
        case 5:
        case 6:
            {
                DOUBLE dExVal = 0.0;
                for (DWORD dwIdx = 0; dwIdx < 3; dwIdx++) {
                    if (dwNeighbor & (0x1 << dwIdx)) {
                        DWORD dwObjMapOnHit = dwObjMap ^ (0x1 << (dwTargetPosIdx + dwIdx));
                        printf("   3, 5, 6: 0x%x, next: 0x%x\n", dwNeighbor, dwObjMapOnHit);
//                        dExVal += dp(dwObjMapOnHit);
                    }
                }
                adExValTmp[dwTargetPosIdx - 1] = (3 + dExVal) / 2.0;                
            }
            break;
        case 7:
            {
                DOUBLE dExVal = 0.0;
                for (DWORD dwIdx = 0; dwIdx < 3; dwIdx++) {
                    DWORD dwObjMapOnHit = dwObjMap ^ (0x1 << (dwTargetPosIdx + dwIdx));
                    printf("   7:0x%x, next: 0x%x\n", dwNeighbor, dwObjMapOnHit);
//                    dExVal += dp(dwObjMapOnHit);
                }
                adExValTmp[dwTargetPosIdx - 1] = (3 + dExVal) / 3.0;
            }
            break;
        default:
            printf("ERR\n");
            break;
        }
    }

    DOUBLE dExValCur = INFINITY;
    for (DWORD dwIdx = 0; dwIdx < ArrayLength(adExValTmp); dwIdx++) {
        if (adExValTmp[dwIdx] < dExValCur) {
            dExValCur = adExValTmp[dwIdx];
        }
    }
    printf("not hit >> %f\n", dExValCur);
    s_adExVal[dwDpMapIdx] = dExValCur;
    return dExValCur;
}

int main()
{
    printf("hoge\n");
    return 0;
#if 0 
    DWORD dwInput_N;
    DWORD dwObjMap=0;
    DWORD dwObjPos;
    char str[6000];

    printf("main entry\n");
    
    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));

    cin.getline(str, sizeof(str));
    dwObjPos = atoi(strtok(str, " "));
    dwObjMap |= 0x1 << (dwObjPos + 1);
    for (DWORD dwIdx = 1; dwIdx < dwInput_N; dwIdx++) {
        dwObjPos = atoi(strtok(NULL, " "));
        dwObjMap |= 0x1 << (dwObjPos + 1);
    }
#endif
//    initDp();
//    printf(" init : %x\n", dwObjMap);
//    DOUBLE dAnswer = dp(dwObjMap);
//    printf("%0.9F", dAnswer);
    return 0;
}
