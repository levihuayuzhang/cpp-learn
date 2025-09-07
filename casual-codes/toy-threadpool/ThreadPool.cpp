#include "ThreadPool.hpp"
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max) {
    do {
        taskQ = new TaskQueue<T>;
        if (taskQ == nullptr) {
            std::cout << "create taskQ failed...\n";
            break;
        }

        threadIDs = new pthread_t[max];
        if (threadIDs == nullptr) {
            std::cout << "create threadIDs failed...\n";
            break;
        }

        memset(threadIDs, 0, sizeof(pthread_t) * max);
        minNum = min;
        maxNum = max;
        busyNum = 0;
        liveNum = min;
        exitNum = 0;

        if (pthread_mutex_init(&mutexPool, nullptr) != 0 ||
            pthread_cond_init(&notEmpty, nullptr) != 0) {
            std::cout << "mutex or condition init failed...\n";
            break;
        }

        shutdown = false;

        pthread_create(&managerID, nullptr, manager, this);
        for (int i = 0; i < min; ++i) {
            pthread_create(&threadIDs[i], nullptr, worker, this);
        }

        return;
    } while (0);

    if (threadIDs) {
        delete[] threadIDs;
    }
    if (taskQ) {
        delete taskQ;
    }
}

template <typename T>
auto ThreadPool<T>::worker(void* arg) -> void* {
    auto* pool = static_cast<ThreadPool*>(arg);

    while (true) {
        pthread_mutex_lock(&pool->mutexPool);
        while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            if (pool->exitNum > 0) {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum) {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
            }
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }

        Task<T> task = pool->taskQ->takeTask();

        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexPool);

        std::cout << "thread " << std::to_string(pthread_self())
                  << " start working...\n";

        task.fuction(task.arg);
        delete task.arg;
        task.arg = nullptr;

        std::cout << "thread " << std::to_string(pthread_self())
                  << " end working...\n";
        pthread_mutex_lock(&pool->mutexPool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);
    }
}

template <typename T>
auto ThreadPool<T>::threadExit() -> void {
    pthread_t tid = pthread_self();
    for (int i = 0; i < maxNum; ++i) {
        if (threadIDs[i] == tid) {
            threadIDs[i] = 0;
            std::cout << "threadExit() called, " << tid << " exiting...\n";
            break;
        }
    }
    pthread_exit(NULL);
}

template <typename T>
auto ThreadPool<T>::manager(void* arg) -> void* {
    auto* pool = static_cast<ThreadPool*>(arg);
    while (!pool->shutdown) {
        // sleep(3);
        std::this_thread::sleep_for(std::chrono::seconds(3));

        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->taskQ->taskNumber();
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);

        if (queueSize > liveNum && liveNum < pool->maxNum) {
            pthread_mutex_lock(&pool->mutexPool);
            int count = 0;
            for (int i = 0; i < pool->maxNum && count < NUMBER &&
                            pool->liveNum < pool->maxNum;
                 ++i) {
                if (pool->threadIDs[i] == 0) {
                    pthread_create(&pool->threadIDs[i], nullptr, worker, pool);
                    count++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);

            for (int i = 0; i < NUMBER; ++i) {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }

    return NULL;
}

template <typename T>
auto ThreadPool<T>::addTask(Task<T> task) -> void {
    if (shutdown) {
        pthread_mutex_unlock(&mutexPool);
        return;
    }
    taskQ->addTask(task);
    pthread_cond_signal(&notEmpty);
}

template <typename T>
auto ThreadPool<T>::getBusyNum() -> int {
    pthread_mutex_lock(&mutexPool);
    const int busyNum = this->busyNum;
    pthread_mutex_unlock(&mutexPool);

    return busyNum;
}

template <typename T>
auto ThreadPool<T>::getLiveNum() -> int {
    pthread_mutex_lock(&mutexPool);
    const int liveNum = this->liveNum;
    pthread_mutex_unlock(&mutexPool);

    return liveNum;
}

template <typename T>
ThreadPool<T>::~ThreadPool() {
    shutdown = true;
    pthread_join(managerID, nullptr);
    for (int i = 0; i < liveNum; ++i) {
        pthread_cond_signal(&notEmpty);
    }

    delete taskQ;
    delete[] threadIDs;

    pthread_mutex_destroy(&mutexPool);
    pthread_cond_destroy(&notEmpty);
}
