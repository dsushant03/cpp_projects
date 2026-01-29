// no_false_sharing_atomic.cpp
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>

constexpr long long ITERS = 200'000'000;

struct alignas(64) PaddedAtomic {
    std::atomic<int> v;
};

PaddedAtomic a, b;

void t0() {
    for (long long i = 0; i < ITERS; ++i)
        a.v.fetch_add(1, std::memory_order_relaxed);
}

void t1() {
    for (long long i = 0; i < ITERS; ++i)
        b.v.fetch_add(1, std::memory_order_relaxed);
}

int main() {
    auto start = std::chrono::steady_clock::now();

    std::thread tA(t0), tB(t1);
    tA.join(); tB.join();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time: "
              << std::chrono::duration<double>(end - start).count()
              << " s\n";
}
