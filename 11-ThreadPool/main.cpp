#include "ThreadPool.h"

void hello(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Hello World!" << std::endl;
}

int main(){
    ThreadPool& pool =  ThreadPool::getInstance(10);
    
    for(int i = 0; i < 30; i++){
        pool.AddTask(hello);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}