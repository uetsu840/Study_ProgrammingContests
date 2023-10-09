
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(void)
{
    int N;
    cin >> N;
    vector<int> A;

    for (int i = 0; i < N; i++) {
        int a;
        cin >> a;
        
        A.push_back(a);
    }
    sort(A.begin(), A.end());

    for (int i = 0; i<N; i++) {
        if (A[i] != i+1) {
            printf("No\n");
            return 0;
        }
    }
    printf("Yes\n");
    
    return 0;
}