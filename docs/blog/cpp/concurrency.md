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
    Multiple threads may read from the same memory location while all other accesses (r-w,w-r and w-w) are called conflicts Data races, deadlocks are undefined behavior.

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

|---|---|---|
|API|C++ Version|Description|
|---|---|---|
|`this_thread::yield`|C++11| Let the operating system schedule another thread.|
|`this_thread::get_id`|C++11|Return current thread ID (OS specific)|
|`this_thread::sleep_for`|C++11|Blocks the execution of the current thread for at least the specified sleep_duration.|
|`this_thread::sleep_until`|C++11|Blocks the execution of the current thread until specified sleep_time has been reached.|

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

## Mutex and Lock

### Mutual Exclusion

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
