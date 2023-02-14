---
# title: "[TIL] Circular Dependency in C++"
description:
comments: true
tags:
  - C++
---

# Circular Dependency in C++

Circular dependencies are normally design flaws. But if you have to live along with it, there is a way to go around.

Suppose we have class `Fool` and `Bar`. If `Fool` uses `Bar` and `Bar` also use `Fool`, then there is a circular dependency. 
In a complex project, it might be not so obvious due to a long chain.
In C++, if `Fool.h` includes `Bar.h` then `Bar.h` is not allowed to include `Fool.h`. 
In case of circular dependency, the compiler is not able to determine the size of both `Fool` and `Bar` since they contain size-unknown class.

The only way for `Bar` to use `Fool` is forward declaration.
We can forward declare `Fool` and use pointers or references on `Fool` without include `fool.h` in `bar.h`.
Consequently, the compiler doesn't have to know the size of the class but the size of a class pointer. The size of pointer of any class is same, and the compiler won't complain. 
Since we still want access to `Fool`'s public members from the members of `Bar`, we include `fool.h` in `bar.cpp`.


```cpp title="fool.h"
# include "bar.h"

class Foo
{
private:
  Bar myBar_
};
```

```cpp title="bar.h"
class Foo; // Forward declaration

Class Bar
{
private:
  Fool* myFool_;
};
```

```cpp title="bar.cpp"
#include "bar.h"
#include "fool.h" // include fool.h in .cpp to get access to Fool
```

In the end drawbacks of circular dependencies are listed:

- Long compilation time. When a file in the cycle is changed, all the other files have to be recompiled.
- Prone to errors: since several pieces of code are tightly coupled, a change in one will probably break another.
- Harder to reuse: as many files are dependent on each other, if you want to reuse a file in another project, you must also take the other ones.
- Harder to debug: as many pieces of codes are coupled, you will have to look at a lot of files if you want to trace back a bug.
