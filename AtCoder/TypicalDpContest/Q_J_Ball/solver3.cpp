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
    for (DWORD dwIdx = 0; dwIdx < OBJ_POS_NUM; dwIdx++) {
        s_adExVal[0x1 << dwIdx] = 3.0;
    }
}

static inline DWORD getNeighbors(DWORD dwObjMap, DWORD dwTargetIdx)
{
    return (((dwObjMap) >> dwTargetIdx-1) & 0x7);
}

static DOUBLE calcExVal(DWORD);

static inline DOUBLE dp_2bit(DWORD dwObjMap, DWORD dwNeighbor, DWORD dwTargetPosIdx)
{
    DOUBLE dExVal = 0.0;
    for (DWORD dwIdx = 0; dwIdx < 3; dwIdx++) {
        if (dwNeighbor & (0x1<< dwIdx)) {
            DWORD dwObjMapOnHit = dwObjMap ^ (0x1 << (dwTargetPosIdx + dwIdx - 1));
            dExVal += calcExVal(dwObjMapOnHit);
        }
    }
    DOUBLE dRet = (3 + dExVal) / 2.0;
    return dRet;
}

static inline DOUBLE dp_3bit(DWORD dwObjMap, DWORD dwNeighbor, DWORD dwTargetPosIdx)
{
    DOUBLE dExVal = 0.0;
    DWORD dwObjMapOnHit;
    for (DWORD dwIdx = 0; dwIdx < 3; dwIdx++) {
        dwObjMapOnHit = dwObjMap ^ (0x1 << (dwTargetPosIdx + dwIdx - 1));
        dExVal += calcExVal(dwObjMapOnHit);
    }
    return (3 + dExVal) / 3.0;
}

static DOUBLE calcExVal(DWORD dwObjMap)
{
    /* hit memo */
    DWORD dwDpMapIdx = dwObjMap >> 1;
    if (s_adExVal[dwDpMapIdx] != 0.0) {
        return s_adExVal[dwDpMapIdx];
    }

    /* not hit memo */
    DOUBLE dExValCur = INFINITY;
    for (DWORD dwTargetPosIdx = 1; dwTargetPosIdx < 1 + OBJ_POS_NUM; dwTargetPosIdx++) {
        DOUBLE dExValTmp;
        DWORD dwNeighbor = getNeighbors(dwObjMap, dwTargetPosIdx);
        switch(dwNeighbor) {
        case 0:
            break;
        case 1:
        case 2:
        case 4:
            {
                DWORD dwObjMapOnHit = dwObjMap ^ (dwNeighbor << (dwTargetPosIdx - 1));
                dExValTmp = 3 + calcExVal(dwObjMapOnHit);
                if (dExValTmp < dExValCur) {
                    dExValCur = dExValTmp;
                } 
            }
            break;
        case 3:
        case 5:
        case 6:
            {
                dExValTmp = dp_2bit(dwObjMap, dwNeighbor, dwTargetPosIdx);
                if (dExValTmp < dExValCur) {
                    dExValCur = dExValTmp;
                } 
            }
            break;
        case 7:
            {
                dExValTmp = dp_3bit(dwObjMap, dwNeighbor, dwTargetPosIdx);
                if (dExValTmp < dExValCur) {
                    dExValCur = dExValTmp;
                } 
            }
            break;
        default:
            printf("ERR\n");
            break;
        }
    }

    s_adExVal[dwDpMapIdx] = dExValCur;
    return dExValCur;
}

int main()
{
    DWORD dwInput_N;
    DWORD dwObjMap=0;
    DWORD dwObjPos;
    char str[300];

    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));

    cin.getline(str, sizeof(str));
    dwObjPos = atoi(strtok(str, " "));
    dwObjMap |= 0x1 << (dwObjPos + 1);
    for (DWORD dwIdx = 1; dwIdx < dwInput_N; dwIdx++) {
        dwObjPos = atoi(strtok(NULL, " "));
        dwObjMap |= 0x1 << (dwObjPos + 1);
    }
    initDp();
    DOUBLE dAnswer = calcExVal(dwObjMap);
    printf("%0.9f\n", dAnswer);
    return 0;
}
