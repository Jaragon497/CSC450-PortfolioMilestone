#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable thread_flag;
std::atomic<bool> ready(false);

void count_up();
void count_down();

int main(int argc, char *argv[]) {

    std::thread thread1(count_up);
    std::thread thread2(count_down);

    thread1.join();
    thread2.join();

    return 0;
}

void count_up() {
    for (int i = 1; i < 21; i++) {
        std::cout << i << std::endl;

        // Make counting visible
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::cout << "First thread complete! " << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
    thread_flag.notify_one();
}

void count_down() {

    std::unique_lock<std::mutex> lock(mtx);
    thread_flag.wait(lock, [] { return ready.load(); });

    for (int i = 20; i > 0; --i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << i << std::endl;
    }

    std::cout << "Second thread complete! " << std::endl;
}
