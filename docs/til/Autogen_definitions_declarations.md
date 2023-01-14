---
# title: "[TIL] Generate definitions or declarations in vscode"
# description: VS Code C++ Extension supports auto-genetating missing function definition or declaration.
comments: true
tags:
  - C++
  - vscode
---

# Generate definitions or declarations in vscode

Did you spend a lot of time typing out lists of arguments or copying and pasting them while defining functions that have already been declared in a header file, or while declaring functions that have already been defined in a cpp file?
The cpp extension in Visual Studio Code could automatically generate a skeleton definition/declaration if it is missing for a function.

Functions will be marked with three dots if definition/declaration is missing.
Click `Quick Fix...` or press <kbd>Ctrl</kbd> + <kbd>.</kbd> to bring up fix context menu.
Then you can create a skeleton of the missing definition or declaration.

![definition missing](../assets/images/missing_def.png))

The generated cpp file looks like:

```cpp
#include "program_manager_server.hpp"

uint32_t com::bshg::domain::cleaningrobot::ProgramManager1::SetCleanAll()
{
  return 0;
}
```
