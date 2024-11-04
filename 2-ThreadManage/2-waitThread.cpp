#include <iostream>
#include <thread>
#include <cassert>

struct func
{
    int& _i;
    func(int& i) : _i(i) { }
    void operator()() {
        for(unsigned j = 0; j < 10; ++j){
            std::cout << "do something " << _i << std::endl;
        }
    }
};

void oops(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();     // 不等待线程结束
}                           // 新线程可能还在运行 但是局部变量释放了 导致出现问题

void f(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func); 
    try{
        std::cout << "do something in current thread..." << std::endl;
    }catch(...){
        t.join();   // 确保在抛出异常前 先释放掉线程
        throw;
    }
    t.join();       // 如果没有异常直接释放
}

class thread_guard{
    std::thread& _t;
public:
    explicit thread_guard(std::thread& t)
    : _t(t){ }
    ~thread_guard(){
        if(_t.joinable()){  // 判断线程是否已加入
            _t.join();      // 没有释放的话 就进行释放
        }
    }

    // 删除构造和拷贝赋值操作
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

void f2(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func); 
    thread_guard g(t);
    std::cout << "do something in current thread..." << std::endl;    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// 分离线程 让线程在后台运行 在unix中指 daemon threads 守护线程
void detachThread(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func); 
    t.detach();
    assert(!t.joinable());
}

int main(){
    // oops();
    // f2();
    detachThread();
    system("pause");
    return 0;
}