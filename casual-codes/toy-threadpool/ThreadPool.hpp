#pragma once

#include <pthread.h>
#ifndef THREADPOLL_HPP
#define THREADPOLL_HPP

// #include "TaskQueue.cpp"
#include "TaskQueue.hpp"

template <typename T>
class ThreadPool {
   public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    auto addTask(Task<T> task) -> void;
    auto getBusyNum() -> int;
    auto getLiveNum() -> int;

   private:
    static auto worker(void* arg) -> void*;
    static auto manager(void* arg) -> void*;
    static const int NUMBER = 2;

    auto threadExit() -> void;

    TaskQueue<T>* taskQ;

    pthread_t managerID;
    pthread_t* threadIDs;

    int minNum;
    int maxNum;
    int busyNum;
    int liveNum;
    int exitNum;

    pthread_mutex_t mutexPool;
    pthread_cond_t notEmpty;

    bool shutdown;
};

#endif
