#include <iostream>
#include <thread>

void hello() {
  std::cout << "Hello Concurrent World" << std::endl;
}

int main() {
  std::thread t(hello);  // 创建线程
  t.join();              // 等待线程结束
  return 0;
}
