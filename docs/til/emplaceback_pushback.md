---
# title: "[TIL] Attribute: nodiscard since C++17"
description:
comments: true
tags:
  - C++
---

# emplace_back VS push_back

Clang-tidy suggests often to use `emplace_back` instead of `push_back`.
Why is `emplace_back` recommended and what is pros and cons of it?

**push_back**

```cpp
void push_back (const value_type& val); void push_back (value_type&& val);
```

Adds a new element at the end of the vector, after its current last element. The content of val is copied (or moved) to the new element.

This effectively increases the container size by one, which causes an automatic reallocation of the allocated storage space if -and only if- the new vector size surpasses the current vector capacity.

If we call `push_back`,

- A _constructor_ will be called to create a temporary object.
- A copy of the temporary object will be constructed in the memory for the container. (Note that the _move constructor_ will be called if exist because the temporary object is an `rvalue`, otherwise the _copy constructor_ should be called.
- The _destructor_ will be called to destroy the temporary object after copy.

**emplace_back**

Appends a new element to the end of the container. The element is constructed through std::allocator_traits::construct, which typically uses placement-new to construct the element **in-place** at the location provided by the container. The arguments `args...` are forwarded to the constructor as `std::forward<Args>(args)...`.

If the new `size()` is greater than capacity() then all iterators and references (including the past-the-end iterator) are invalidated. Otherwise only the past-the-end iterator is invalidated. 

In contrast to `push_back`, `emplace_back` directly takes constructor arguments for objects to be inserted. In other words, the emplacement function avoids constructing and destructing temporary objects. It will be much more efficient to insert large amount of objects or for object that is time consuming to create/destroy.

Let's see an example:

```cpp
class Foo {
public:
    Foo(int x, int y) : x_(x), y_(y)
    {
        std::cout << "Create class" << std::endl;
    }

    ~Foo()
    {
        std::cout << "Destroy class" << std::endl;
    }

    // Copy Constructor
    Foo(const Foo& myClass)
    {
        std::cout << "Copy Constructor Called" << std::endl;
        x_ = myClass.x_;
    }

    // Move Constructor
    Foo (Foo&& myClass) noexcept
    {
        std::cout << "Move Constructor Called" << std::endl;
        x_ = std::move(myClass.x_);
    }

private:
    int x_ = 0;
    int y_ = 0;

};

int main()
{
    std::vector<Foo> vector;
    // Reserve space to avoid reallocation
    vector.reserve(2);
    
    std::cout << "\n--- push_back ---" << std::endl;
    vector.push_back(Foo(1, 2));
    
    std::cout << "\n--- emplace_back ---" << std::endl;
    vector.emplace_back(1, 2);
    
    std::cout << "\n--- Finish ---" << std::endl;
    return 0;
}
```

The output is

```bash
--- push_back ---
Create Class
Move Constructor Called
Destroy Class

--- emplace_back ---
Create Class

--- Finish ---
Destroy Class
Destroy Class
```

As described, `push_back` called the _move constructor_ to make a copy and the destructor to destroy the temporary object while `emplace_back` constructed the object directly.

Should I use `emplace_back` all the time and forget `push_back`?

**NO**. 

```cpp
vector1.push_back(100); // we can tell it tries to add the number 100 to the end of the vector.
vector2.emplace_back(100); // Without knowing the type of the vector, we don’t know what constructor is actually invoking.
```

If the vector is defined as
```cpp
std::vector<int> vec1;
std::vector<std::vector<int>> vec2;
```
the `emplace_back` method in the above will construct a vector of 100 elements. 
If this behavior is unexpected, it may take long time for the developers to catch this issues.

!!! quote
    Very often the performance difference just won’t matter. As always, the rule of thumb is that you should avoid “optimizations” that make the code less safe or less clear, unless the performance benefit is big enough to show up in your application benchmarks.

