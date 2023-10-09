#include <iostream>
using namespace std;

int main(void) {
    int N;
    cin >> N; 
    if (N % 2 == 0) {
        cout << N;
    } else {
        cout <<N*2;
    }
    return 0;
}