#pragma once

#ifndef TASKQUEUE_HPP
#define TASKQUEUE_HPP

#include <pthread.h>
#include <queue>

using callback = void (*)(void* arg);

template <typename T>
struct Task {
   public:
    Task() : fuction(nullptr), arg(nullptr) {}
    Task(callback func, void* arg) {
        fuction = func;
        this->arg = (T*)arg;
    }

    callback fuction;
    T* arg;
};

template <typename T>
class TaskQueue {
   public:
    TaskQueue();
    ~TaskQueue();

    void addTask(Task<T> task);
    void addTask(callback func, void* arg);
    auto takeTask() -> Task<T>;
    inline auto taskNumber() -> int { return m_taskQ.size(); }

   private:
    pthread_mutex_t m_mtx;
    std::queue<Task<T>> m_taskQ;
};

#endif
