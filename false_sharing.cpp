// false_sharing_atomic.cpp
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>

constexpr long long ITERS = 200'000'000;

std::atomic<int> arr[2];

void t0() {
    for (long long i = 0; i < ITERS; ++i)
        arr[0].fetch_add(1, std::memory_order_relaxed);
}

void t1() {
    for (long long i = 0; i < ITERS; ++i)
        arr[1].fetch_add(1, std::memory_order_relaxed);
}

int main() {
    auto start = std::chrono::steady_clock::now();

    std::thread a(t0), b(t1);
    a.join(); b.join();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time: "
              << std::chrono::duration<double>(end - start).count()
              << " s\n";
}
