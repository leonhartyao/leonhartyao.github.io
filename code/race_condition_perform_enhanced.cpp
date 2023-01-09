#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Account
{
public:
  Account() : mMoney(0) {}
  int getMoney()
  {
    return mMoney;
  }
  void addMoney(int money)
  {
    int tmp = 0;
    for (auto i = 0; i < money; i++) {
      tmp++;
    }
    std::lock_guard<std::mutex> lockGuard(exclusive);
    mMoney += tmp;
  }

private:
  int mMoney;
  std::mutex exclusive;
};

int testMultiThreadAccount()
{
  Account myAccountObj;
  std::vector<std::thread> threadpool;
  // 5 thread to add 1000 to myAccountobj in each.
  for (auto i = 0; i < 5; i++) {
    threadpool.push_back(std::thread(&Account::addMoney, &myAccountObj, 1000));
  }
  for (auto& t : threadpool) {
    t.join();
  }
  return myAccountObj.getMoney();
}

int main()
{
  int value;
  for (int i = 0; i < 1000; i++) {
    value = testMultiThreadAccount();
    if (value != 5000) {
      std::cout << "Error at run = " << i << ", Money in accout = " << value << std::endl;
    }
  }
  return 0;
}