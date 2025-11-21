#include <iostream>
#include <thread>
#include <vector>
#include <syncstream>
#include <latch>
#include <chrono>

const int A_LEN = 9;
const int B_LEN = 5;
const int C_LEN = 5;
const int D_LEN = 9;
const int E_LEN = 8;
const int F_LEN = 5;
const int G_LEN = 5;
const int H_LEN = 4;
const int I_LEN = 9;
const int J_LEN = 7;


std::latch done_a(1);
std::latch done_b(1);
std::latch done_c(1);
std::latch done_d(1);
std::latch done_e(1);
std::latch done_f(1);
std::latch done_g(1);

void f(char name, int count) {
    for (int i = 1; i <= count; ++i) {
        std::osyncstream(std::cout) << "From set " << name << " action " << i << " performed.\n";

    }
}

void thread_worker_1() {
    // 1. Perform A
    f('a', A_LEN);
    done_a.count_down();

    f('d', D_LEN);
    done_d.count_down();

    done_f.wait();
    f('i', I_LEN);
}

void thread_worker_2() {
    f('b', B_LEN);
    done_b.count_down();

    done_c.wait();
    f('f', F_LEN);
    done_f.count_down();

    done_e.wait();
    done_g.wait();
    f('j', J_LEN);
}

void thread_worker_3() {
    f('c', C_LEN);
    done_c.count_down();

    done_b.wait();
    f('g', G_LEN);
    done_g.count_down();

    done_d.wait();
    done_f.wait();
    f('h', H_LEN);
}

void thread_worker_4() {
    done_a.wait();
    f('e', E_LEN);
    done_e.count_down();
}

int main() {
    std::cout << "Calculation started.\n";

    std::vector<std::thread> threads;
    threads.emplace_back(thread_worker_1);
    threads.emplace_back(thread_worker_2);
    threads.emplace_back(thread_worker_3);
    threads.emplace_back(thread_worker_4);

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Calculation completed.\n";
    return 0;
}