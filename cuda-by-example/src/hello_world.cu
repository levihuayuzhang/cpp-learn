#include <cstdio>
#include <iostream>
#include "book.h"

__global__ auto add(int a, int b, int* c) -> void {
  *c = a + b;
}

auto main(void) -> int {
  int c;
  int* dev_c;

  HANDLE_ERROR(cudaMalloc((void**)&dev_c, sizeof(int)));
  add<<<1, 1>>>(2, 7, dev_c);
  HANDLE_ERROR(cudaMemcpy(&c, dev_c, sizeof(int), cudaMemcpyDeviceToHost));
  printf("2 + 7 = %d\n", c);

  return 0;
}