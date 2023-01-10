#include <iostream>
#include <cmath>
#include <thread>
#include <future>

class Worker
{
public:
  Worker(int min, int max) : mMin(min), mMax(max) {}  // ①
  double work()
  {  //
    mResult = 0;
    for (int i = mMin; i <= mMax; i++) {
      mResult += std::sqrt(i);
    }
    return mResult;
  }
  double getResult()
  {
    return mResult;
  }

private:
  int mMin;
  int mMax;
  double mResult;
};

int main()
{
  Worker w(0, 10e8);
  std::cout << "Task in class triggered" << std::endl;
  auto f3 = std::async(std::launch::async, &Worker::work, &w);  //
  f3.wait();
  std::cout << "Task in class finish, result: " << w.getResult() << std::endl << std::endl;

  return 0;
}