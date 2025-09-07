use rayon::prelude::*;

#[cfg(target_arch = "x86_64")]
use std::arch::x86_64::*;

unsafe fn sum_avx2(data: &[f32]) -> f32 {
    let mut sum = unsafe { _mm256_setzero_ps() };
    let chunks = data.chunks_exact(8);
    let remainder = chunks.remainder();

    for chunk in chunks {
        let v = unsafe { _mm256_loadu_ps(chunk.as_ptr()) };
        sum = unsafe { _mm256_add_ps(sum, v) };
    }

    let mut result = [0f32; 8];
    unsafe { _mm256_storeu_ps(result.as_mut_ptr(), sum) };
    let mut total = result.iter().sum::<f32>();
    total += remainder.iter().sum::<f32>();
    total
}

fn sum_of_squares(input: &[i32]) -> i32 {
    input
        .par_iter() // <-- just change that!
        .map(|&i| i * i)
        .sum()
}

unsafe fn sum_simd_rayon(data: &[f32]) -> f32 {
    data.par_chunks(1024) // 并行分块
        .map(|chunk| unsafe { sum_avx2(chunk) }) // 每块用 SIMD 求和
        .sum()
}

fn main() {
    let input_data: [i32; 3] = [3, 2, 1];
    println!("{}", sum_of_squares(&input_data));

    let input_data: [f32; 3] = [3.33, 2.22, 1.11];
    unsafe {
        println!("{}", sum_simd_rayon(&input_data));
    };
}
