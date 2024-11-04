#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::queue<int> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    while(true)
    {
        int data;
        std::lock_guard<std::mutex>	lk(mtx);
        data_queue.push(data);		//	准备压入数据
        data_cond.notify_one();		//	进行通知
    }
}
void data_processing_thread()
{
    while(true)
    {
        std::unique_lock<std::mutex> lk(mtx);		//	先上锁
        data_cond.wait(lk, [](){return !data_queue.empty(); });		// 等待收到通知条件成立
        int data = data_queue.front();
        data_queue.pop();
        lk.unlock();		// 解锁
        std::cout << "process data..." << std::endl;
    }
}

int main(){

    system("pause");
    return 0;
}