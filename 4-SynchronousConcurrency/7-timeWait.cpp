#include <iostream>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

int do_work(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "wait over!" << std::endl;
    return 42;
}

// 等待时延 
void time1(){
    std::future<int> f = std::async(do_work);
    int val = 0;
    if(f.wait_for(std::chrono::seconds(2)) == std::future_status::ready){   // 等待2s 如果线程函数发生变化那么就返回一个状态值
        val = f.get();
    }else{
        std::cout << "log" << std::endl;
    }
    std::cout << "val: " << val << std::endl;
}

// 计算代码耗时
void code_time(){
    auto start = std::chrono::high_resolution_clock::now();
    do_work();
    auto end = std::chrono::high_resolution_clock::now();
    auto t = std::chrono::duration<double>(end - start).count();
    std::cout << "do work took " << t << " s" << std::endl;
}


std::condition_variable cv;
bool done;
std::mutex m;

// 等待超时
bool wait_loop(){
    auto const timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lk(m);
    while (!done)
    {
        if(cv.wait_until(lk, timeout) == std::cv_status::timeout){
            break;
        }
    }
    return done;
}

void timeout(){
    bool flag = wait_loop();
    std::cout << "flag: " << flag << std::endl;
}

int main(){
    // time1();
    // code_time();
    timeout();
    system("pause");
    return 0;
}