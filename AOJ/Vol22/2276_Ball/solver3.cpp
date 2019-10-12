#define _USE_MATH_DEFINES
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <tuple>
#include <iomanip>
#include <assert.h>

#define sqr(x) std::pow(x, 2.0)
#define INF 1e10
#define EPS 1e-10
#define REP(i, n) for (int i = 0; i < n; i++)
#define POS_OR_INF(x) (x > 0 ? x : INF)

using namespace std;

using ld = long double;

struct Point {
	Point() = default;
	Point(ld x, ld y) : x(x), y(y) {}
	Point(const Point& s, const Point& e) : x(e.x - s.x), y(e.y - s.y) {}

	Point normalized(void) const {
		return Point(x / norm(), y / norm());
	}

	ld norm(void) const {
		return sqrt(x * x + y * y);
	}

	Point flipped(void) const {
		return Point(-x, -y);
	}

	Point normal_vector(bool normalize = false) const {
		auto vec = Point(-y, x);
		return normalize ? vec.normalized() : vec;
	}

	ld x, y;
};

using Vector = Point;

inline Vector points2vec(const Point& s, const Point& e) {
	return Vector(e.x - s.x, e.y - s.y);
}

inline ld distance(const Point& s, const Point& e) {
	return points2vec(s, e).norm();
}

inline ld distance_square(const Point& s, const Point& e) {
	return pow(points2vec(s, e).norm(), 2.0);
}

inline Vector operator * (const Vector& l, ld r) {
	return Vector(l.x * r, l.y * r);
}

inline Vector operator * (ld l, const Vector& r) {
	return r * l;
}

inline Vector operator / (const Vector& l, ld r) {
	return Vector(l.x / r, l.y / r);
}

inline Point operator + (const Point& l, const Point& r) {
	return Point(l.x + r.x, l.y + r.y);
}

inline Point operator - (const Point& l, const Point& r) {
	return Point(l.x - r.x, l.y - r.y);
}

inline istream& operator >> (istream& is, Point& p) {
	return is >> p.x >> p.y;
}

int main(int argc, char** argv) {
	int N, K;

	cin >> N >> K;
	cout << fixed << setprecision(20);

	vector<pair<double, double>> a(N), b;

	for (int i = 0; i < N; i++) {
		Point p;
		double r;
		cin >> p >> r;

		double theta = asin(r/p.norm());
		double rad = atan2(p.y, p.x);

		if (rad < -M_PI / 2) rad += 2 * M_PI;

		a[i].first = min(M_PI, max(0.0, rad - theta));
		a[i].second = min(M_PI, max(0.0, rad + theta));
	}

	for (int i = 0; i < N; i++) {
		bool included = false;
		for (int j = 0; j < N; j++) {
			if (a[i].first > a[j].first && a[i].second < a[j].second)
				included = true;
		}
		if (!included) {
			b.push_back(a[i]);
		}
	}

	sort(b.begin(), b.end());

	int M = b.size();

	vector<vector<double>> dp(K + 1, vector<double>(M + 1, 0));
	double ans = 0;

	for (int i = 1; i <= K; i++) {
		for (int j = 0; j < M; j++) {
			double best = 0;
			int k = 0;

			while (b[k].second < b[j].first) {
				best = max(best, dp[i-1][k]);
				k++;
			}
			
			dp[i][j] = max(dp[i][j], best + b[j].second - b[j].first);
			dp[i][j] = max(dp[i][j], dp[i-1][k] + b[j].second - b[k].second);
			ans = max(ans, dp[i][j]);
		}
	}

	cout << ans / M_PI;
}
