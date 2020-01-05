#include <cstdlib>
#include <cstdint>
#include <cstdio>

const int MAX_C = 1000000000;

uint64_t x;

uint64_t xorshift() {
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  return x * 2685821657736338717LL;
}

void xorshift_srand(uint64_t s) {
  x = s;
  for (int i = 0; i < 1000; ++i) xorshift();
}

int main(int argc, char **argv) {
  int N = atoi(argv[1]);
  int M = atoi(argv[2]);
  int K = atoi(argv[3]);
  xorshift_srand(atoi(argv[4]));

  printf("%d %d %d\n", N, M, K);
  for (int i = 0; i < M; ++i) {
    int a = xorshift() % N;
    int b = xorshift() % N;
    int w = 1 + xorshift() % MAX_C;
    printf("%d %d %d\n", a, b, w);
  }

  return 0;
}
