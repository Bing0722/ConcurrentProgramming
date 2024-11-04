#include <iostream>
#include <thread>

void f(){
    std::cout << "id: " << std::this_thread::get_id() << std::endl;
}

int main(){
    std::thread::id mainId = std::this_thread::get_id();
    std::thread t([](){
        std::thread::id threadId = std::this_thread::get_id();
        std::cout << "thread id: " << threadId << std::endl;
        std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
    });
    std::cout << "main id: " << mainId << std::endl;
    std::cout << "main id: " << std::this_thread::get_id() << std::endl;
    t.join();

    std::thread t2(f);
    t2.join();
    system("pause");
    return 0;
}