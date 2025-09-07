#include <iostream>
#include <thread>

auto hello() -> void {
    std::cout << "shity thread id is: " << std::this_thread::get_id() << "\n";
}

auto main() -> int {
    std::thread t(hello);

    t.join();
}