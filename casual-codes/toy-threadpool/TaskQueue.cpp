#include "TaskQueue.hpp"
#include <pthread.h>

template <typename T>
TaskQueue<T>::TaskQueue() {
    pthread_mutex_init(&m_mtx, nullptr);
}

template <typename T>
TaskQueue<T>::~TaskQueue() {
    pthread_mutex_destroy(&m_mtx);
}

template <typename T>
auto TaskQueue<T>::addTask(Task<T> task) -> void {
    pthread_mutex_lock(&m_mtx);
    m_taskQ.push(task);
    pthread_mutex_unlock(&m_mtx);
}

template <typename T>
auto TaskQueue<T>::addTask(callback func, void* arg) -> void {
    pthread_mutex_lock(&m_mtx);
    m_taskQ.push(Task<T>(func, arg));
    pthread_mutex_unlock(&m_mtx);
}

template <typename T>
auto TaskQueue<T>::takeTask() -> Task<T> {
    Task<T> task;

    pthread_mutex_lock(&m_mtx);
    if (!m_taskQ.empty()) {
        task = m_taskQ.front();
        m_taskQ.pop();
    }
    pthread_mutex_unlock(&m_mtx);

    return task;
}
