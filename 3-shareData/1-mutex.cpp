#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>

std::list<int> some_list;   // 定义一个全局变量
std::mutex some_mutex;      // 定义一个全局互斥变量

void add_to_list(int new_value){
    std::lock_guard<std::mutex> guard(some_mutex);  // 使用互斥锁保护变量
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find()){
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

int main(){

    system("pause");
    return 0;
}