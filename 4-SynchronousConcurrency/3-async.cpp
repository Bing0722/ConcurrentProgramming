#include <iostream>
#include <future>
#include <chrono>

// 定义一个异步任务
std::string fetchDataFromDB(std::string query) {
    // 模拟一个异步任务，比如从数据库中获取数据
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return "Data: " + query;
}

/*
*@ std::async 创建了一个新的线程（或从内部线程池中挑选一个线程）
*/
void async_task(){
    // 使用 std::async 异步调用 fetchDataFromDB
    std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
    // 在主线程中做其他事情
    std::cout << "Doing something else..." << std::endl;
    // 从 future 对象中获取数据
    std::string dbData = resultFromDB.get();
    std::cout << dbData << std::endl;    
}

/*
*@ async的启动策略
*@ std::launch::async: 可以确保在独立线程中执行任务，适用于需要立即执行的情况
*@ std::launch::deferred: 这种策略意味着任务将在调用std::future::get()或std::future::wait()函数时延迟执行 换句话说，任务将在需要结果时同步执行
*@ std::launch::async | std::launch::deferred: 这种策略是上面两个策略的组合, 任务可以在一个单独的线程上异步执行，也可以延迟执行，具体取决于实现
*/

int main(){
    async_task();
    system("pause");
    return 0;
}