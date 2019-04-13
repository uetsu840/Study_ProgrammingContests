using namespace std;

#include <iostream>
#include <stdlib.h>
#include <string.h>

using QWORD = unsigned long long;
using SDWORD = int;
using DWORD = unsigned int;
using KNAPSACK_OBJ_ST = struct
{
    QWORD dwWeight;
    QWORD dwValue;
};

int main()
{
    DWORD dwInput_N;
    DWORD dwInput_K;
    char str[30];

    cin.getline(str, sizeof(str));
    dwInput_N = atoi(strtok(str, " "));
    dwInput_K = atoi(strtok(NULL, " "));

    // printf("n:%d k:%d\n", dwInput_N, dwInput_K);
    QWORD qwAnswer = 0;
    if (0 == dwInput_K % 2) {
        QWORD qwAnswer1, qwAnswer2;
        DWORD dwUnit1 = (dwInput_N / dwInput_K);
        qwAnswer1 = (QWORD)dwUnit1 * (QWORD)dwUnit1 * (QWORD)dwUnit1;

        DWORD dwUnit2 = (dwInput_N + (dwInput_K / 2)) / dwInput_K;
        qwAnswer2 = (QWORD)dwUnit2 * (QWORD)dwUnit2 * (QWORD)dwUnit2;

        //printf("%d %d\n", dwAnswer1, dwAnswer2);

        qwAnswer = qwAnswer1 + qwAnswer2;
    } else {
        DWORD dwUnit = (dwInput_N / dwInput_K);
        qwAnswer = (QWORD)dwUnit * (QWORD)dwUnit * (QWORD)dwUnit;
    }

    printf("%llu\n", qwAnswer);

    return 0;
}