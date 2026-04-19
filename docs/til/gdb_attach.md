---
# title: "[TIL] Attach GDB to running process"
description:
comments: true
tags:
  - debug
  - gdb
---

When it is built with Debug, we can attach GDB to the executable for debugging:

```shell
sudo gdb -ex continue <path_to_executable> $(pidof <process_name>)
```
