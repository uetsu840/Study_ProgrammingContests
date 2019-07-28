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

using ll = long long;

class BouncingBalls
{
public:
	double expectedBounces(vector <int> x, int T);
};

#define INPUT_MAX   (100100100)

double BouncingBalls::expectedBounces(vector <int> x, int T)
{
    sort(x.begin(), x.end());

    double ans= 0.0;
    for (auto it = x.begin(); it != x.end(); ++it) {
        ll border_ll = (ll)*it + (ll)T * (ll)2;
        int border = min((ll)INPUT_MAX, border_ll);
        
        int collision_count = upper_bound(it, x.end(), border) - it - 1;
//        printf("%d\n", collision_count);

        ans += (double)collision_count * 0.25;
    }

    return ans;
}


int main(void)
{
    BouncingBalls solver;

    int x_1[] = {5, 8};
    int T_1 = 2;
    vector<int> vec1(x_1, end(x_1));

    double ans1 = solver.expectedBounces(vec1, T_1);
    printf("--%f\n", ans1);

    int x_2[] = {91, 857, 692, 54, 8679, 83, 792, 86, 9537, 913, 64, 592};
    int T_2 = 458;
    vector<int> vec2(x_2, end(x_2));
    double ans2 = solver.expectedBounces(vec2, T_2);
    printf("--%f\n", ans2);

    int x_3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int T_3 = 3;
    vector<int> vec3(x_3, end(x_3));
    double ans3 = solver.expectedBounces(vec3, T_3);
    printf("--%f\n", ans3);

    int x_4[] = {1, 4};
    int T_4 = 1;
    vector<int> vec4(x_4, end(x_4));
    double ans4 = solver.expectedBounces(vec4, T_4);
    printf("--%f\n", ans4);



    return 0;
}