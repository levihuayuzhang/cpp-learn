#include <cuda_runtime.h>
#include <iostream>
#include <vector>

__global__ void vector_add(const float *a, const float *b, float *c, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        c[idx] = a[idx] + b[idx];
    }
}

int main() {
    constexpr int N = 1 << 16; // 65536 elements
    constexpr size_t size = N * sizeof(float);

    // Host vectors (using C++20 std::vector with initialization)
    std::vector<float> h_a(N, 1.0f);
    std::vector<float> h_b(N, 2.0f);
    std::vector<float> h_c(N);

    // Device pointers
    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    // Copy data from host to device
    cudaMemcpy(d_a, h_a.data(), size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b.data(), size, cudaMemcpyHostToDevice);

    // Launch kernel (256 threads per block)
    int threadsPerBlock = 256;
    int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;
    vector_add<<<blocks, threadsPerBlock>>>(d_a, d_b, d_c, N);

    // Copy result back to host
    cudaMemcpy(h_c.data(), d_c, size, cudaMemcpyDeviceToHost);

    // Verify result (C++20 ranges)
    // bool all_correct =
    //     std::ranges::all_of(h_c, [](float x) { return x == 3.0f; });
    // std::cout << (all_correct ? "All results correct!" : "Mismatch in
    // results!")
    //           << '\n';

    // Cleanup
    std::cout << "Freeing" << '\n';
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
