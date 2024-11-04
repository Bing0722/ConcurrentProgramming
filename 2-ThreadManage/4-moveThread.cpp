#include <iostream>
#include <thread>
#include <vector>

void some_function() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void some_other_function() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void f(){
    //t1 绑定some_function
    std::thread t1(some_function); 
    //2 转移t1管理的线程给t2，转移后t1无效
    std::thread t2 =  std::move(t1);
    //3 t1 可继续绑定其他线程,执行some_other_function
    t1 = std::thread(some_other_function);
    //4  创建一个线程变量t3
    std::thread t3;
    //5  转移t2管理的线程给t3
    t3 = std::move(t2);
    //6  转移t3管理的线程给t1
    t1 = std::move(t3);     // 发生崩溃 t1 此时正在管理运行线程
    std::this_thread::sleep_for(std::chrono::seconds(2000));    
}

void do_work(unsigned id){

}
// 可以创建多个线程
void f2(){
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < 20; ++i){
        threads.push_back(std::thread(do_work, i));
    }
    for (auto& it : threads){
        it.join();
    }
}


int main(){

    system("pause");
    return 0;
}