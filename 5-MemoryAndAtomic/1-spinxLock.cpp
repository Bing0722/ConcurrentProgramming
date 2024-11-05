#include <iostream>
#include <atomic>
#include <thread>

class SpinLock {
public:
    void lock() {
        // 只有一个线程进入 test_and_set 
        while (flag.test_and_set(std::memory_order_acquire)); // 自旋等待，直到成功获取到锁
    }
    void unlock() {
        // 将flag 设置为清空状态
        flag.clear(std::memory_order_release); // 释放锁
    }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

void TestSpinLock() {
    SpinLock spinlock;
    std::thread t1([&spinlock]() {
        spinlock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "*";
            }
        std::cout << std::endl;
        spinlock.unlock();
        });
    std::thread t2([&spinlock]() {
        spinlock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "?";
        }
        std::cout << std::endl;
        spinlock.unlock();
        });
    t1.join();
    t2.join();
}

int main(){
    TestSpinLock();
    system("pause");
    return 0;
}