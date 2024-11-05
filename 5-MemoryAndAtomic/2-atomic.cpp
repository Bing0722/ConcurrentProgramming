#include <iostream>
#include <atomic>

// store() 存储操作 load() 加载操作 exchange() 读-改-写 操作
void base_operation(){
    std::atomic<bool> b(true);
    b = false;
    bool x = b.load(std::memory_order_acquire);
    b.store(true);
    x = b.exchange(false, std::memory_order_acq_rel);
}

int main(){

    system("pause");
    return 0;
}