#include <iostream>

// c++11 的安全的单例模式
class SingleInstance {
private:
    SingleInstance()
    {
    }
    SingleInstance(const SingleInstance&) = delete;
    SingleInstance& operator=(const SingleInstance&) = delete;
public:
    static SingleInstance& GetInstance()
    {
        static SingleInstance Instance;
        return Instance;
    }
};

int main(){

    system("pause");
    return 0;
}