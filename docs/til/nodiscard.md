---
title: "[TIL] Attibute: nodiscard since C++17"
description:
comments: true
tags:
  - C++
---

`nodiscard` is a attribute introduced by c++17.
If a function

 - declared nodiscard **or**
 - returning an enumeration or class declared nodiscard by value 
 
 the compiler will issue a warning when the function is called from a _discarded-value_ expression other than a cast to void.
 
Here is a example:

```cpp
[[nodiscard]] int foo()
{
  return 7;
}

foo(); // warning: ignoring return value of function declared with 'nodiscard' attribute
int bar = foo(); // no warning
static_cast<void>(foo()); // no warning
```
