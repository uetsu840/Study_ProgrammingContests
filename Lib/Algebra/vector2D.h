#include "common.h"

#define EPS 1e-10
DOUBLE double_add(DOUBLE a, DOUBLE b) {
    if (ABS(a + b) < EPS * (ABS(a) + ABS(b))) {
        return 0;
    }
    return a+b;
}

struct VECTOR_2D {
    DOUBLE dX;
    DOUBLE dY;
    VECTOR_2D(DOUBLE x, DOUBLE y) : dX(x), dY(y) {};
    VECTOR_2D() : dX(0), dY(0) {};

    /* 加算 */
	VECTOR_2D& operator+= (const VECTOR_2D a)
    { 
        dX = dX + a.dX;
        dY = dY + a.dY;
        return *this;
    };
    const VECTOR_2D operator+ (const VECTOR_2D a) const {
		return VECTOR_2D(dX + a.dX, dY + a.dY); 
    };

    /* 減算 */
	VECTOR_2D& operator-= (const VECTOR_2D a)
    { 
        dX = dX - a.dX;
        dY = dY - a.dY;
        return *this;
    };
    const VECTOR_2D operator- (const VECTOR_2D a) const {
		return VECTOR_2D(dX - a.dX, dY - a.dY);
    };

    /* 定数倍 */
	VECTOR_2D& operator*= (const DOUBLE a)
    { 
        dX = dX * a;
        dY = dY * a;
        return *this;
    };
    const VECTOR_2D operator* (const DOUBLE a) const {
		return VECTOR_2D(dX * a, dY * a); 
    };

    /* 比較 */
    bool operator< (const VECTOR_2D &a) {
        if (dX == a.dX) {
            return (dY < a.dY);
        }
        return dX < a.dX;
    }

    DOUBLE norm(void) const {
        return (dX * dX + dY * dY);
    }

    DOUBLE dist(void) const {
        return sqrt(norm());
    }

    DOUBLE angle(void) const {
        return atan2(dY, dX);
    }

    DOUBLE dotproduct(const VECTOR_2D a) const {
        return double_add(dX * a.dX, dY * a.dY);
    }

    DOUBLE crossproduct(const VECTOR_2D a) const {
        return double_add(dX * a.dY, -dY * a.dX);
    }

    VECTOR_2D rotate(DOUBLE angle) {
        VECTOR_2D ret;
        ret.dX = dX * cos(angle) - dY * sin(angle);
        ret.dY = dX * sin(angle) + dY * cos(angle);

        return ret;
    }

    void print() {
        printf("%f %f\n", dX, dY);
    }
};

