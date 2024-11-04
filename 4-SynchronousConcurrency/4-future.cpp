#include <iostream>
#include <future>
#include <thread>

int my_task() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "my task run 5 s" << std::endl;
    return 42;
}

// std::packaged_task 包装任务
void use_package() {
    // 创建一个包装了任务的 std::packaged_task 对象  
    std::packaged_task<int()> task(my_task);
    // 获取与任务关联的 std::future 对象  
    std::future<int> result = task.get_future();
    // 在另一个线程上执行任务  
    std::thread t(std::move(task));
    t.detach(); // 将线程与主线程分离，以便主线程可以等待任务完成  
    // 等待任务完成并获取结果  
    int value = result.get();
    std::cout << "The result is: " << value << std::endl;
}

/*
*@ std::future::get(): 是一个阻塞调用 获取对象表示的值或异常 只能调用一次 一旦 get() 被调用，std::future 对象就不能再被用来获取结果
*@ std::future::wait(): 是一个阻塞调用 不会返回任务的结果 只是等待异步任务完成 可以被多次调用
*/


int main(){
    use_package();
    system("pause");
    return 0;
}