#include "curand_normal.h"
#include <cuda_runtime.h>

// __global__ void monte_carlo_kernel(float *d_results, float S0, float K, float
// r,
//                                    float sigma, float T, int N) {
//     int tid = threadIdx.x + blockIdx.x * blockDim.x;
//     if (tid < N) {
//         float ST = S0;
//         // 简化：一次随机步长
//         ST = S0 * expf((r - 0.5f * sigma * sigma) * T +
//                        sigma * sqrtf(T) * curand_normal(&tid));

//         d_results[tid] = fmaxf(ST - K, 0.0f) * expf(-r * T);
//     }
// }

__global__ void moving_average_kernel(float *prices, float *ma, int N,
                                      int window) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < N) {
        float sum = 0;
        for (int i = 0; i < window; i++) {
            sum += prices[idx - i]; // 假设边界检查在 host 做
        }
        ma[idx] = sum / window;
    }
}

__global__ void sum_kernel(float *data, float *result, int N) {
    __shared__ float buffer[256];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int lane = threadIdx.x;

    float val = (tid < N) ? data[tid] : 0;
    buffer[lane] = val;
    __syncthreads();

    // block 内归约
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (lane < s)
            buffer[lane] += buffer[lane + s];
        __syncthreads();
    }

    if (lane == 0)
        result[blockIdx.x] = buffer[0];
}