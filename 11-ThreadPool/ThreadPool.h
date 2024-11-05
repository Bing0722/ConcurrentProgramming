#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <vector>
#include <functional>
#include "NoneCopy.h"


using Task = std::function<void()>;

class ThreadPool : public NoneCopy
{
public:
    // 继承了基类NoneCopy 所以不用写删除拷贝构造和赋值了
    static ThreadPool& getInstance(size_t threadNum);
    ~ThreadPool();
    // 添加任务
    void AddTask(Task&&);
    // 停止线程池
    void shutdown();
    // 查看线程数和空闲数量
    void checkThread();
private:
    ThreadPool(size_t threadNum);
    // 开启线程
    void Start();
    // 停止线程
    void Stop();
    // 线程循环函数
    void loop();
    std::atomic_int             _freeNum;   // 空闲的线程数
    int                         _threadNum; // 线程数
    std::queue<Task>            _taskQue;   // 任务队列
    std::vector<std::thread>    _pool;      // 存放线程的线程池
    std::atomic_bool            _stop;      // 线程停止标志位
    std::mutex                  _mtx;       // 互斥锁
    std::condition_variable     _cv;        // 条件变量
};

// 单例模式
ThreadPool &ThreadPool::getInstance(size_t threadNum)
{
    static ThreadPool SingleInstance(threadNum);
    return SingleInstance;
}

ThreadPool::ThreadPool(size_t threadNum)
: _stop(false)
{   // 线程数和硬件核心数比较
    if(threadNum > std::thread::hardware_concurrency()){
        _threadNum = std::thread::hardware_concurrency();
    }else if(threadNum <= 1){
        _threadNum = 2;
    }else{
        _threadNum = threadNum;
    }
    _freeNum = _threadNum;
    Start();
}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::AddTask(Task&& task)       // 传递右值引用 避免不必要的拷贝
{
    std::unique_lock<std::mutex> lock(_mtx);
    _taskQue.push(std::move(task));     // 移动任务进任务队列 
    _cv.notify_one();   // 通知线程池执行任务
}

void ThreadPool::shutdown()
{
    Stop();
}

void ThreadPool::checkThread()
{
    std::unique_lock<std::mutex> lock(_mtx);
    std::cout << "Thread Number: " << _pool.size() << ", "
              << "Free Thread Number: " << _freeNum.load() << std::endl;
}

void ThreadPool::Start()
{
    for(size_t i = 0; i < _threadNum; i++){     // 启动线程
        _pool.push_back(std::thread([this](){ this->loop(); }));
    }
}

void ThreadPool::Stop()
{
    _stop.store(true);  // 设置停止标志
    _cv.notify_all();   // 唤醒所有线程
    for(auto& td : _pool){
        if(td.joinable()){
            std::unique_lock<std::mutex> lock(_mtx);
            std::cout << "join thread " << td.get_id() << std::endl;
            lock.unlock();
            td.join();      // 释放线程
        }
    }
}

void ThreadPool::loop()
{
    while (!_stop.load())  
    {   // 1.取任务
        Task task;
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait(lock, [this](){    // 返回为 true 就继续等待
                return this->_stop.load() || !this->_taskQue.empty();
            });

            if(!_taskQue.empty()){
                task = std::move(_taskQue.front());
                _taskQue.pop();
                _freeNum--;                          
            }
        }
        // 任务不为空 执行任务
        if(task){
            checkThread();
            task();
            _freeNum++;
            checkThread();
        }
    }
}

#endif  // ThreadPool.h