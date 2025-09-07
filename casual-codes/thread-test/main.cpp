#include <pthread.h>
#include <cstdlib>
#include <iostream>
// #include <thread>

struct Args {
    int arg1;
    int arg2;
};

auto func1(void* arg) -> void* {
    Args* a = static_cast<Args*>(arg);
    std::cout << "Thread ID: " << pthread_self() << ", arg1: " << a->arg1
              << ", arg2: " << a->arg2 << '\n';

    // pthread_t* rt_tid = new pthread_t(pthread_self());
    // return rt_tid;

    pthread_exit(arg);
    return nullptr;
}

auto main() -> int {
    const pthread_t tid_main = pthread_self();
    std::cout << "Thread ID of main: " << tid_main << '\n';

    pthread_t tid1;
    Args a1;
    a1.arg1 = 3;
    a1.arg2 = 9;
    pthread_create(&tid1, nullptr, func1, &a1);

    // Args* rt1 = static_cast<Args*>(malloc(sizeof(Args)));
    // void* rt1;
    // pthread_join(tid1, &rt1);
    // std::cout << "rt1->arg1: " << static_cast<Args*>(rt1)->arg1
    //           << ", tr1->arg2: " << static_cast<Args*>(rt1)->arg2 << '\n';

    // std::cout << "return tid is: " << *rt1 << '\n';
    // delete rt1;
    // std::cout << tid1;

    pthread_detach(tid1);
    pthread_exit(nullptr);

    return 0;
}