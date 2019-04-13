using namespace std;

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using QWORD = unsigned long long;
using SDWORD = int;
using DWORD = unsigned int;
using DOUBLE = double;

const int MAX_FACTORY(60);

inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
inline QWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)

typedef struct {
    DWORD elem_2;
    DWORD elem_3;
    DWORD elem_5;
} DICE_PRIME_ELEM;

/**
 *  2, 3, 5 に限定して素因数分解する 
 */
M_BOOL calc_PrimeFactorization(QWORD qwNum, DICE_PRIME_ELEM *pElem)
{
    int elem_2 = 0;
    int elem_3 = 0;
    int elem_5 = 0;
    while(1) {
        if (0 == qwNum % 2) {
            qwNum /= 2;
            elem_2++;
        } else {
            break;
        }
    }

    while(1) {
        if (0 == qwNum % 3) {
            qwNum /= 3;
            elem_3++;
        } else {
            break;
        }
    }

    while (1) {
        if (0 == qwNum % 5) {
            qwNum /= 5;
            elem_5++;
        } else {
            break;
        }
    }
    if (1 != qwNum) {
        return M_FALSE;
    }

    pElem->elem_2 = elem_2;
    pElem->elem_3 = elem_3;
    pElem->elem_5 = elem_5;

    //printf("2:%d 3:%d 5:%d\n", pElem->elem_2, pElem->elem_3, pElem->elem_5);
    return M_TRUE;
}

#define MAX_N (100)
static DOUBLE s_aaadProba_A[(MAX_N+1) * 2 + 3][MAX_N+4][MAX_N+4];
static DOUBLE s_aaadProba_B[(MAX_N+1) * 2 + 3][MAX_N+4][MAX_N+4];

DWORD get_Proba(DICE_PRIME_ELEM *pElem, DWORD dwN)
{
    DWORD dwTryIdx;
    DWORD dwIdx_2, dwIdx_3, dwIdx_5;

    /* Initialize */
    memset(s_aaadProba_A, 0, sizeof(s_aaadProba_A));
    memset(s_aaadProba_B, 0, sizeof(s_aaadProba_B));
    s_aaadProba_A[2][2][2] = 1;

    for (dwTryIdx = 0; dwTryIdx < dwN; dwTryIdx++) {
        //printf("-");
        for (dwIdx_2 = 2; dwIdx_2 <= (dwTryIdx + 1) * 2 + 2; dwIdx_2++) {
            for (dwIdx_3 = 2; dwIdx_3 <= (dwTryIdx + 1) + 2; dwIdx_3++) {
                for (dwIdx_5 = 2; dwIdx_5 <= (dwTryIdx + 1) + 2; dwIdx_5++) {
                    if (dwTryIdx % 2 == 0) {
                        s_aaadProba_B[dwIdx_2][dwIdx_3][dwIdx_5]
                         = (s_aaadProba_A[dwIdx_2][dwIdx_3][dwIdx_5]
                            + s_aaadProba_A[dwIdx_2-1][dwIdx_3][dwIdx_5]
                            + s_aaadProba_A[dwIdx_2][dwIdx_3-1][dwIdx_5]
                            + s_aaadProba_A[dwIdx_2-2][dwIdx_3][dwIdx_5]
                            + s_aaadProba_A[dwIdx_2][dwIdx_3][dwIdx_5-1]
                            + s_aaadProba_A[dwIdx_2-1][dwIdx_3-1][dwIdx_5]) / 6;
                        //printf("B: [%d][%d][%d]<- [%f]\n", dwIdx_2-2, dwIdx_3-2, dwIdx_5-2, s_aaadProba_B[dwIdx_2][dwIdx_3][dwIdx_5]);
                    } else {
                        s_aaadProba_A[dwIdx_2][dwIdx_3][dwIdx_5]
                         = (s_aaadProba_B[dwIdx_2][dwIdx_3][dwIdx_5]
                            + s_aaadProba_B[dwIdx_2-1][dwIdx_3][dwIdx_5]
                            + s_aaadProba_B[dwIdx_2][dwIdx_3-1][dwIdx_5]
                            + s_aaadProba_B[dwIdx_2-2][dwIdx_3][dwIdx_5]
                            + s_aaadProba_B[dwIdx_2][dwIdx_3][dwIdx_5-1]
                            + s_aaadProba_B[dwIdx_2-1][dwIdx_3-1][dwIdx_5]) / 6;
                        //printf("A: [%d][%d][%d]<- [%f]\n", dwIdx_2-2, dwIdx_3-2, dwIdx_5-2, s_aaadProba_A[dwIdx_2][dwIdx_3][dwIdx_5]);
                    }
                }
            }
        }
    }


    #define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))
    DOUBLE dSum = 0;
    if (dwN % 2 == 0) {
        for (dwIdx_2 = pElem->elem_2 + 2; dwIdx_2 <= (dwN) * 2 + 2; dwIdx_2++) {
            for (dwIdx_3 = pElem->elem_3 + 2; dwIdx_3 <= (dwN) + 2; dwIdx_3++) {
                for (dwIdx_5 = pElem->elem_5 + 2; dwIdx_5 <= (dwN) + 2; dwIdx_5++) {
                    dSum += s_aaadProba_A[dwIdx_2][dwIdx_3][dwIdx_5];       
                }
            } 
        }
    } else {
        for (dwIdx_2 = pElem->elem_2 + 2; dwIdx_2 <= (dwN) * 2 + 2; dwIdx_2++) {
            for (dwIdx_3 = pElem->elem_3 + 2; dwIdx_3 <= (dwN) + 2; dwIdx_3++) {
                for (dwIdx_5 = pElem->elem_5 + 2; dwIdx_5 <= (dwN) + 2; dwIdx_5++) {
                    dSum += s_aaadProba_B[dwIdx_2][dwIdx_3][dwIdx_5];       
                }
            } 
        }
    }
    printf("%0.12f\n", dSum);
}

int main()
{
    DWORD dwInput_N;
    QWORD qwInput_D;
    DICE_PRIME_ELEM stElem_D;
    char str[30];

    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));
    qwInput_D = atoll(strtok(NULL, " "));

    //printf("N:%d D:%lld\n", dwInput_N, qwInput_D);

    if (calc_PrimeFactorization(qwInput_D, &stElem_D)) {
        get_Proba(&stElem_D, dwInput_N);
    } else {
        printf("0.0");
    }
    return 0;
}
