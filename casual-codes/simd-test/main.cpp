#include <immintrin.h>
#include <iostream>
#include <omp.h>

auto sum_avx2(float *data, size_t n) -> float {
    __m256 vsum = _mm256_setzero_ps();
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        __m256 v = _mm256_loadu_ps(data + i); // load 8 floats
        vsum = _mm256_add_ps(vsum, v);
    }
    float sum[8];
    _mm256_storeu_ps(sum, vsum);
    float total =
        sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];
    // 处理尾部
    for (; i < n; i++)
        total += data[i];
    return total;
}

float sum_parallel(float *data, size_t n) {
    float total = 0.0f;
#pragma omp parallel
    {
        __m256 vsum = _mm256_setzero_ps();
        float local_sum = 0.0f;
#pragma omp for
        for (size_t i = 0; i + 8 <= n; i += 8) {
            __m256 v = _mm256_loadu_ps(data + i);
            vsum = _mm256_add_ps(vsum, v);
        }
        float temp[8];
        _mm256_storeu_ps(temp, vsum);
        for (float i : temp)
            local_sum += i;
#pragma omp atomic
        total += local_sum;
    }
    return total;
}

int main() {
    float arr1[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    float arr2[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    std::cout << sum_avx2(arr1, sizeof(arr1) / sizeof(float)) << '\n'
              << sum_parallel(arr2, 6) << '\n';

    return 0;
}