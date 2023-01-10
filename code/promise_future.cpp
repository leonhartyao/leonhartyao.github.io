#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void doWork(std::promise<int> promiseObj)
{
  std::cout << "Inside new thread" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  promiseObj.set_value(7);  // set value that can be accessed by future1
}

int main()
{
  std::promise<int> promise1;
  std::future<int> future1 = promise1.get_future();
  std::thread th(doWork, std::move(promise1));
  std::cout << "Main thread call get() for result... " << std::endl;
  int result = future1.get();  // blocked here until promise1 is set in the new thread
  std::cout << "Result is returned: " << result << std::endl;
  th.join();
  return 0;
}