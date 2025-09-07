// #include <chrono>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "TaskQueue.cpp"
#include "TaskQueue.hpp"
#include "ThreadPool.cpp"
#include "ThreadPool.hpp"

void taskFunc(void* arg) {
    int num = *(int*)arg;
    std::cout << "thread " << pthread_self() << " is working, number = " << num
              << '\n';
    // sleep(1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    ThreadPool<int> pool(3, 10);
    for (int i = 0; i < 100; ++i) {
        int* num = new int(i + 100);
        Task<int> task(taskFunc, num);
        pool.addTask(task);
    }

    // sleep(60);
    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}
