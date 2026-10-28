#include <cstdio>
#include "book.h"

int main(void) {
  cudaDeviceProp prop;
  printf("Quering devices info: \n");

  int count;
  HANDLE_ERROR(cudaGetDeviceCount(&count));
  for (int i = 0; i < count; i++) {
    HANDLE_ERROR(cudaGetDeviceProperties(&prop, i));
    printf("Name: %s\n", prop.name);
    printf("Compute capability: %d.%d\n", prop.major, prop.minor);

    printf("Total global mem: %ld\n", prop.totalGlobalMem);
    printf("Total constant mem: %ld\n", prop.totalConstMem);
    printf("Max mem pitch: %ld\n", prop.memPitch);
    printf("Texture Alignment: %ld\n", prop.textureAlignment);

    printf("Multiprocessor count: %d\n", prop.multiProcessorCount);
    printf("Shared mem per mp: %ld\n", prop.sharedMemPerMultiprocessor);
    printf("Registers per mp: %d\n", prop.regsPerMultiprocessor);
    printf("Threads in warp: %d\n", prop.warpSize);

    printf("Max thread dimensions: (%d, %d, %d)\n", prop.maxThreadsDim[0],
           prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
    printf("Max grid dimensions: (%d, %d, %d)\n", prop.maxGridSize[0],
           prop.maxGridSize[1], prop.maxGridSize[2]);
    printf("\n");
  }
}