---
title: "[TIL] Never `using namespace` in Header Files"
description:
comments: true
tags:
  - C++
---

You should `NEVER` use `using namespace` in headers, it can unexpectedly change the meaning of code in any other files that include that header.
When you write a header file, you don’t know from which context it will be included. 
Therefore, if this header contains using directives, you cannot be sure that they will not create ambiguities in that context. Those ambiguities could lead to compilation failures or, worse, to a different function being selected by overload resolution depending on the order of inclusion of headers.
Another reason is that there's no way to undo a using namespace.

In header files, don't write namespace-level using directives or using declarations; instead, explicitly namespace-qualify all names.
You can use `using` statements within .cpp files without much concern because the scope will be limited to just that file, but never do it before an `#include` statement. 
