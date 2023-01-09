---
title: Multithreading and Concurrency in C++
description:
comments: true
tags:
  - c++
---

Multithreading is a feature that allows concurrent execution of two or more parts of a program for maximum utilization of the CPU.
Multithreading support was introduced in C+11.
![cpp roadmap](https://www.modernescpp.com/images/blog/MultithreadingCpp17Cpp20/MultithreadingInCpp17Cpp20/timelineCpp17andCpp20.png)

!!! note
    Multiple threads may read from the same memory location while all other accesses (r-w,w-r and w-w) are called conflicts. Data races, deadlocks are undefined behavior.

## Thread

### Launching Thread

The constructor of `std::thread` can be used to start a new thread. We simply need to pass the executing code to be called (i.e, a callable object).
Once the object is created a new thread is launched which will execute the code specified in callable.
A callable can be either of the three

- Function pointer
- Function object
- Lambda expression

```cpp
#include <iostream>
#include <thread>
void foo(int a, int b)
{
  std::cout << "Thread has ID: " << std::this_thread::get_id() << std::endl;
}
class bar
{
  // Overload () operator
  void operator()(int c)
  {
      std::cout << "Thread has ID: " << std::this_thread::get_id() << std::endl;
  }
};
int main()
{
  int a = 1;
  int b = 2;
  // Pass a function and args
  std::thread t1(foo, a, b);
  // Pass a lambda
  std::thread t2([a, b](){ foo(a, b); });
  // Pass a function object
  std::thread t3(bar(), a);
  // Execute in the main thread
  foo(6, 7);
  t3.join();
  t2.join();
  t1.join();
}
```

Thread is movable but not copyable. `std::move` tansfers all resources associated with the running thread. The _move-from_ thread becomes empty and not joinable, and only the _move-to_ thread can be joined.

```cpp
#include <iostream>
#include <string_view>
#include <thread>
void foo(std::string_view s);
int main()
{
  std::thread t1([]() { foo("Hi\n"); });
  std::thread t2 = std::move(t1); // t1 is now empty
  t2.join(); // OK, thread originally started in t1 is joined
}
```

Thread can be used in standard library containers, e.g.

```cpp
#include <thread>
#include <vector>
void foo(int i);
int main()
{
  std::vector<std::thread> threadPool;
  for (int i = 1; i <= 9; ++i)
  {
    threadPool.emplace_back([i]() { foo(i); });
  }
  // Digits 1 to 9 are printed (unordered)
  for (auto& t : threadPool)
  {
    t.join();
  }
}
```

### Join and Detach

The thread will terminate once the function returns.
The member function `join()` can be used to wait for a thread to finish. This function makes the current thread wait until the thread identified by `*this` has finished executing. It must be called exactly once for each thread.

Alternatively, a thread can be detached for the thread object with `detach()`. The thread object can be destroyed and the OS thread of execution can continue on. If the program needs to know when that thread of execution has completed, some other mechanism needs to be used. `join()` cannot be called on that thread object any more, since it is no longer associated with a thread of execution.

`join()` or `detach()` must be called **before** a thread object is destroyed. It is considered an error to destroy a C++ thread object while it is still "joinable". If a C++ thread object is still joinable when it's destroyed, an exception will be thrown.

```cpp
#include <iostream>
#include <string_view>
#include <thread>
void foo(std::string_view s);
int main() {
{
  std::thread t1([]() { foo("Hi\n"); });
  t1.join();
}
// Everything is fine, we called t1.join()
{
  std::thread t2([]() {});
}
// Program terminated because t2 is joinable when destroyed
}
```

Both API can be called only on a joinable thread. It can be checked with `joinable()` before calling `joint()` or `detach()`.

```cpp
std::thread th(foo, args);
if (th.joinable())
{
  th.detach();
}
```

### Manage the Current Thread

| API                        | C++ Version | Description                                                                             |
|----------------------------|-------------|-----------------------------------------------------------------------------------------|
| `this_thread::yield`       | C++11       | Let the operating system schedule another thread.                                       |
| `this_thread::get_id`      | C++11       | Return current thread ID (OS specific)                                                  |
| `this_thread::sleep_for`   | C++11       | Blocks the execution of the current thread for at least the specified sleep_duration.   |
| `this_thread::sleep_until` | C++11       | Blocks the execution of the current thread until specified sleep_time has been reached. |

Each thread object has an associated ID `std::thread::id` which can be get by `std::thread::get_id()`. `std::this_thread::get_id()` returns the ID of the current thread. If a thread object has no associated thread, `get_id()` will return "not any thread"

!!! note
    In the example, the arguments are passed by value. If you pass by reference, the life time of the variable has to be considered. The thread kann exceed its lifetime, which leads to access a destroyed object.

### Call_once

In some use cases, we want to executes the Callable object exactly once, even if called concurrently, from several threads.
For instance, you have to do a _not-thread-safe_ one-time global initialization before you're able to do some _thread-safe_ stuff. You can use `call_once` that calls the initialization only once, no matter in which thread and whether it's called concurrently.

```cpp
void init()
{
  std::cout << "Initializing..." << std::endl;
  // Do something
}

void worker(once_flag* flag)
{
  call_once(*flag, init);
}

int main()
{
  once_flag flag;
  std::vector<std::thread> threadPool;
  for (int i = 1; i <= 9; ++i)
  {
    threads.push_back(std::thread(worker, &flag));
  }
  for (auto& t : threadPool)
  {
    t.join();
  }
}
```

In this example, all 9 threads will call `init` once. We don't know which thread has called the init function but it doesn't matter.

## Critical Section and Race Condition

A **critical section** is a section of code that is executed by multiple threads and where the sequence of execution for the threads makes a difference in the result of the concurrent execution of the critical section.

A **race condition** is a situation that may occur inside a critical section. This happens when the result of multiple thread execution in critical section differs according to the order in which the threads execute.
The term race condition stems from the metaphor that the threads are racing through the critical section, and that the result of that race impacts the result of executing the critical section.
Race conditions can be extremely difficult to debug simply because the bug itself depends on the timing of nondeterministic events. It is quite common that the bug cannot be recreated by testers, particularly if the problematic timing results from a rare circumstance.

!!! danger Story
    The Therac-25 radiation therapy machine is a classic and well-cited example of a race condition with deadly effects. The software for this machine included a one-byte counter. If the machine’s operator entered terminal input to the machine at the exact moment that the counter overflowed (quite common for a counter with only 256 possible values), a critical safety lock failed. This flaw made it possible for patients to receive approximately 100 times the intended radiation dose, which directly caused the deaths of three patients. [^1]

Let's try the following example. Five threads are launched to add 1000 respectively to a shared account object. The initial amount is 0 and the expected amount is 5000 when all the threads are finished.

```cpp
#include <iostream>
#include <vector>
#include <thread>

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
    for (auto i = 0; i < money; i++) {
      mMoney++;
    }
  }

private:
  int mMoney;
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
```

The result is sometimes less than the expectation due to race condition.

```plain
Error at run = 302, Money in accout = 4651
Error at run = 540, Money in accout = 4985
Error at run = 625, Money in accout = 4000
Error at run = 662, Money in accout = 4000
Error at run = 809, Money in accout = 4379
Error at run = 816, Money in accout = 4993
Error at run = 821, Money in accout = 4879
Error at run = 837, Money in accout = 2000
Error at run = 875, Money in accout = 4823
Error at run = 881, Money in accout = 4701
Error at run = 911, Money in accout = 4118
Error at run = 949, Money in accout = 4000
```

As we know, for modern processors, in order to speed up processing, each processor has multi-level Cache as shown in the following figure.
The cache is involved when the processor is performing calculations, such as reading and writing data. It is possible that there is an inconsistency between the cache and the main memory of the system. That is, a result is computed and saved in the processor's cache, but not yet synchronized to the main memory, and this value is not visible to other processors.

![cpu cache](assets/../../../assets/images/cpu_cache.svg)
<!-- <p align="center" width="100%">
    <img width="50%" src="../../assets/images/cpu_cache.svg">
</p> -->

Actually, the statement `mMoney++` is not atomic. It is actually a combination of many instructions to accomplish. Let's say that on a particular device, this statement is accomplished by 3 machine commands and their timing may be as follows:

![race timing](assets/../../../assets/images/race_timing.svg)
<!-- <p align="center" width="100%">
    <img width="50%" src="../../assets/images/race_timing.svg">
</p> -->

In this case, an increment will be ignored, because instead of increasing the mMoney variable twice, a different register is added and the value of the "mMoney" variable is overwritten.

Naturally, we can now understand that the race condition occurs because these threads are accessing the shared data at the same time, and the changes made by some of them are not made known to the other threads, causing the other threads to process on the wrong basis, and the result is naturally wrong.
Avoiding race conditions requires data protection for critical sections.
If we let only one thread access the shared data at a time, and let other threads access it afterwards, the problem can be solved.

## Mutex and Lock

### Mutual Exclusion

Mutual exclusion is a straightforward way to synchronize multiple threads, thus, avoid race conditions.

- Threads acquire a lock on a mutex object before entering a critical section.
- Threads release their lock on the mutex when leaving a critical section.

In the c++, mutexes are in the `<mutex>` header file, and there are mainly 6 classes:

| API                   | C++ version| Description                                          |
| --------------------- | ---------- | ---------------------------------------------------- |
| mutex                 | C++11      | basic mutual exclusion                               |
| timed_mutex           | C++11      | with timeout                                         |
| recursive_mutex       | C++11      | can be recursively locked by the same thread         |
| recursive_timed_mutex | C++11      | recursive mutex with timeout                         |
| shared_timed_mutex    | C++14      | shared mutex with timeout                            |
| shared_mutex          | C++17      | several threads can share ownership of the same mutex|

All mutex classes have three basic member functions:

|lock|locks the mutex, blocks if the mutex is not available|
|try_lock|tries to lock the mutex, returns if the mutex is not available|
|unlock|unlocks the mutex|

Other classes are extended with following features:

- **timeout** provides `try_lock_for` and `try_lock_until` methods. If the lock is not acquired within the time limit, it will return directly and will not wait any longer.
- **recursive** The same lock can be locked multiple times in the same thread. This avoids some unnecessary deadlocks.
- **share** has two levels of access. `shared`: several threads can share ownership of the same mutex. `exclusive`: only one thread can own the mutex.
  - If one thread has acquired the exclusive lock (through `lock`, `try_lock`), no other threads can acquire the lock (including the shared).
  - If one thread has acquired the shared lock (through `lock_shared`, `try_lock_shared`), no other thread can acquire the exclusive lock, but can acquire the shared lock.


In practice, high-level programming model is designed like this:

- The resource (usually a class) that requires protection from data races owns a mutex object of the appropriate type.
- Threads that intend to access the resource acquire a suitable lock on the mutex **before** performing the actual access.
- Threads release their lock on the mutex **after** completing the access.
- Usually locks are simply acquired and released in the member functions of the class.

Next, let's fix the example code with mutex. We have only to modify the `Account` class and lock in the shared function `addMoney`:

```cpp
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
    exclusive.lock();
    for (auto i = 0; i < money; i++) {
      mMoney++;
    }
    exclusive.unlock();
  }

private:
  int mMoney;
  std::mutex exclusive;
};
```

In the example, we have manually locked and unlocked the mutex. This is not an easy task in a complicated nested structure considering exception.
What happens if we forget to release the lock at the end of the function? In this case, one thread will exit without releasing the lock and the other threads will remain waiting. To avoid this, we should use `std::lock_guard`.

The class lock_guard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block.
When a lock_guard object is created, it attempts to take ownership of the mutex it is given. When control leaves the scope in which the lock_guard object was created, the lock_guard is destructed and the mutex is released.

```cpp
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
    std::lock_gard<std::mutex> lockGuard(exclusive)
    for (auto i = 0; i < money; i++) {
      // In case of exception, destructor of lock_guard will be called
      mMoney++;
    }
    // destructor of lock_guard will be called to unlock mutex
  }

private:
  int mMoney;
  std::mutex exclusive;
};
```

If you compared the example using 5 threads with a serial version, it performs much worse than the single thread program.
WHY?

### Unique_lock

### Recursive Mutexes

### Shared_lock

### Deadlocks

### Condition Variables

## Atomic Operations

## Future

## Parallel STL

With C++17, most of the algorithms of the Standard Template Library will be available in a parallel version. Therefore, you can invoke an algorithm with a so-called execution policy. This execution policy specifies if the algorithm runs sequential (`std::seq`), parallel (`std::par`), or parallel and vectorised (`std::par_unseq`).

```cpp
std::vector<int> vec ={3, 2, 1, 4, 5, 6, 10, 8, 9, 4};

std::sort(vec.begin(), vec.end());                            // sequential as ever
std::sort(std::execution::seq, vec.begin(), vec.end());       // sequential
std::sort(std::execution::par, vec.begin(), vec.end());       // parallel
std::sort(std::execution::par_unseq, vec.begin(), vec.end()); // parallel and vectorized
```

Therefore, the first and second variations of the sort algorithm run sequential, the third parallel, and the fourth parallel and vectorised.

C++20 offers totally new multithreading concepts. The key idea is that multithreading becomes a lot simpler and less error-prone.

[^1]: https://en.wikipedia.org/wiki/Therac-25
