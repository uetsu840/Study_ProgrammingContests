#include <iostream>
#include <vector>
using namespace std;

int main() {
	try {
        vector<int> vvTest = vector<int>(5,0);
        vvTest[0]= 3;
        printf("%d", vvTest[0]);
	}
	catch (...) {
	    cout << "!\n";
	}

	return 0;
}