#include <iostream>
#include <thread>

void do_some_work(){
    std::cout << "do some work..." << std::endl;
}

class background_task{
public:
    void operator()() const {
        do_some_work();
        std::cout << "do something else..." << std::endl;
    }
};

int main(){
    // 1.直接使用无参函数
    std::thread t1(do_some_work);
    t1.join();

    // 2.使用函数对象
    background_task f;
    std::thread t2(f);
    t2.join();
  
    // 3.使用临时变量 要使用 {}
    std::thread t3{background_task()};
    t3.join();

    // 4.使用 lambda 表达式
    std::thread t4([](){
        do_some_work();
        std::cout << "do something else..." << std::endl;
    });
    t4.join();
    
    system("pause");
    return 0;
}
