---
comments: true
tags:
  - GCC
  - C++
---

# GCC Diagnostic Pragma

GCC allows the user to selectively enable or disable certain types of diagnostics, and change the kind of the diagnostic.
For example, diagnostics might be enabled selectively to treat them as errors depending on which preprocessor macros are defined.

Here is a example:

```cpp
// remember current state of the diagnostic
#pragma GCC diagnostic push

// no diagnostic for return
#pragma GCC diagnostic ignored "-Wreturn-type"

int func1()
{
  return;
}

// restore pushed state of the diagnostic
#pragma GCC diagnostic push

int func2()
{
  return;
}

int main(int argc, char* argv[])
{
  func1();  // no warning
  func2();  // warning return without value
}
```
