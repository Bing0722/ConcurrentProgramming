#include <iostream>
#include <mutex>

std::mutex mtx;
int shared_data = 0;

//可判断是否占有锁
void owns_lock() {
    //lock可自动解锁，也可手动解锁
    std::unique_lock<std::mutex> lock(mtx);
    shared_data++;
    if (lock.owns_lock()) {
        std::cout << "owns lock" << std::endl;
    }
    else {
        std::cout << "doesn't own lock" << std::endl;
    }
    lock.unlock();
    if (lock.owns_lock()) {
        std::cout << "owns lock" << std::endl;
    }
    else {
        std::cout << "doesn't own lock" << std::endl;
    }
}

int main(){

    system("pause");
    return 0;
}