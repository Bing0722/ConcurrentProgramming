#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>

template<typename Iterator,typename	T>
 struct	accumulate_block
 {
 void operator()(Iterator first,Iterator last,T&	result)
 {
    result=std::accumulate(first,last,result);
 }
 };

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);    // 第一个到最高一个的距离 不包括最后一个
    if (!length){
        return init;    // 发现长度为 0 返回
    }
        
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;    // 计算最大开辟数
    unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);    // 适合开辟线程的最小值
    unsigned long const block_size = length / num_threads;    // 计算步长 
        std::vector<T> results(num_threads);
    std::vector<std::thread>  threads(num_threads - 1);   // 初始化线程数 去掉主线程
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);    // 移动步长
        threads[i] = std::thread(// 开辟线程
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i]));
        block_start = block_end;    // 更新起始位置
    }
    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);    // 主线程计算
        for (auto& entry : threads){
            entry.join();    // 释放所有线程
        }
    return std::accumulate(results.begin(), results.end(), init);    // 将所有计算结果调用
}

void use_parallel_acc() {
    std::vector <int> vec(10000);
    for (int i = 0; i < 10000; i++) {
        vec.push_back(i);
    }
    int sum = 0;
    sum = parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), 
        vec.end(), sum);
    std::cout << "sum is " << sum << std::endl;
}

int main(){
    use_parallel_acc();
    system("pause");
    return 0;
}