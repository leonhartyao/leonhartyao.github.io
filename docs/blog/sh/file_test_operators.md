---
title: Bash File Test Operators
description: Checking if a specific file or folder is very useful in shell scripts.
comments: true
tags:
  - Bash
---

We often need to check files or folders in a bash script. Following are file test operators in bash.\

### -a

Returns `true` if file exists.

!!! note
    It is identical in effect to `-e`, but it is deprecated and its use is discouraged.

### -b

Returns `true` if file is a block device.

!!! note
    A block device (e.g., hard drives, CDROM drives, flash drives) reads and/or writes data in chunks, or blocks, in contrast to a character device (e.g., keyboards, modems, sound cards), which acesses data in character units.

```bash
device="/dev/sda2"
if [ -b "$device" ]
then
  echo "$device is a block device."
fi

# /dev/sda2 is a block device.
```

### -c

Returns `true` if file is a character device.

```bash
device1="/dev/ttyS1"   # PCMCIA modem card.
if [ -c "$device1" ]
then
  echo "$device1 is a character device."
fi

# /dev/ttyS1 is a character device.
```

### -d

Returns `true` if file is a directory.

### -e

Returns `true` if file exists.

### -f

Returns `true` if file is a `regular` file, i.e., not a device file or a directory.

### -g

Returns `true` if set-group-id (sgid) flag set on file or directory.\
If a directory has the `sgid` flag set, then a file created within that directory belongs to the group that owns the directory, not necessarily to the group of the user who created the file. This may be useful for a directory shared by a workgroup.

### -G

Returns `true` if `group-id` of the file is same as you.

### -h

Returns `true` if file is a symbolic link.

### -L

Returns `true` if file is a symbolic link.

### -N

Returns `true` if file is modified since last read.

### -O

Returns `true` if you are the owner of the file.

### -p

Returns `true` if file is a pipe.

!!! note
    Passes the output (stdout) of a previous command to the input (stdin) of the next one, or to the shell. This is a method of chaining commands together.

```bash
function show_input_type()
{
   [ -p /dev/fd/0 ] && echo PIPE || echo STDIN
}

show_input_type "Input"                           # STDIN
echo "Input" | show_input_type                    # PIPE

# This example courtesy of Carl Anderson.
```

### -s

Returns `true` if file is not zero size.

### -S

Returns `true` if file is a socket.

!!! note
    A socket is a communications node associated with a specific I/O port. (This is analogous to a hardware socket, or receptacle, for a connecting cable.) It permits data transfer between hardware devices on the same machine, between machines on the same network, between machines across different networks, and, of course, between machines at different locations on the Internet.

### -t

Returns `true` if file is associated with a terminal device.\
This test option may be used to check whether the `stdin` [ -t 0 ] or stdout [ -t 1 ] in a given script is a terminal.

### -u

Returns `true` if set-user-id (suid) flag set on file

!!! note
    A binary owned by root with set-user-id flag set runs with root privileges, even when an ordinary user invokes it. This is useful for executables (such as pppd and cdrecord) that need to access system hardware. Lacking the suid flag, these binaries could not be invoked by a non-root user.

### -w

Returns `true` if file has write permission (for the user running the test).

### -x

Returns `true` if file has execute permission (for the user running the test).

### f1 -nt f2

Returns `true` if file `f1` is newer than `f2`.

### f1 -ot f2

Returns `true` if file `f1` is older than `f2`.

### f1 -ef f2

Returns `true` if files `f1` and `f2` are hard links to the same file.

### ! <!-- nolint -->

"not" -- reverses the sense of the tests above (returns true if condition absent).

### Example

```bash
#!/bin/bash
# fileinfo.sh

FILES="/usr/sbin/accept
/usr/sbin/pwck
/usr/sbin/chroot
/usr/bin/fakefile
/sbin/badblocks
/sbin/ypbind"     # List of files you are curious about.
                  # Threw in a dummy file, /usr/bin/fakefile.

echo

for file in $FILES
do

  if [ ! -e "$file" ]       # Check if file exists.
  then
    echo "$file does not exist."; echo
    continue                # On to next.
   fi
```
