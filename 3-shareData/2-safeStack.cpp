#include <iostream>
#include <memory>
#include <mutex>
#include <exception>
#include <stack>

struct empty_stack : std::exception
{
    const char* what() const throw() {
        return "empty stack!";
    }
};

// 线程安全的堆栈
template <typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack() : data(std::stack<T>()) { }
    
    threadsafe_stack(const threadsafe_stack& other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }

    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()){
            throw empty_stack();
        }
        std::shared_ptr<T> const res(std::make_shared(<T>(data.top())));
        data.pop();
        return res;
    }

    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()){
            throw empty_stack();
        }
        value = data.top();
        data.pop();        
    }
};


int main(){

    system("pause");
    return 0;
}