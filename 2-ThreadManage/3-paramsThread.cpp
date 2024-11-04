#include <iostream>
#include <thread>

void f(int i, std::string const& s){
    std::cout << "i: " << i << std::endl;
    std::cout << "s: " << s << std::endl;
}

void oops(int some_param){
    char buffer[1024];  // 指针变量指向本地变量
    sprintf(buffer, "%i", some_param);
    // std::thread t(f, 3, buffer);
    std::thread t(f, 3, std::string(buffer));    // 避免悬垂指针使用是 std::string
    t.detach();
}

class X
{
public:
    void do_lengthy_work() {
        std::cout << "do_lengthy_work " << std::endl;
    }
};
// 绑定类成员
void bind_class_oops() {
    X my_x;
    std::thread t(&X::do_lengthy_work, &my_x);
    t.join();
}

int main(){
    oops(2);
    bind_class_oops();
    system("pause");
    return 0;
}