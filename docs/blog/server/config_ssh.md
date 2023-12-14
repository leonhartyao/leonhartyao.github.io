---
title: SSH Configuration for Easy and Secure Remote Login
description: This article shows how to make ssh easy, fast and secure.
comments: true
tags:
  - SSH
---

The most common way to login to a remote server is via SSH in a terminal. Four main steps will be shown here.\

1. Client config for fast login
2. Public key for password-free login
3. Disable password for security
4. Change default port for security
5. Stay connected

## Fast SSH

In order to login to a remote server with the IP `192.168.0.8` as `chao`, we can use the following ssh command.

```bash
ssh chao@192.168.0.8
```

It is sometimes hard to remember the IP address of every remote server, and typing IP address is not efficient.\
You can add hosts to `~/.ssh/config` on the client machine.

```bash
# ~/.ssh/config
Host nas
    HostName 192.168.0.8
    User chao
```

Consequently, you can use the host alias to login to the remote server more conveniently avoiding username and IP address.

```bash
ssh nas
```

## Password-free SSH

By storing the public key of the client on the server `~/.ssh/authorized_keys`, you can get rid of entering password every time.

If there is no ssh-key pair available yet in `~/.ssh/`, you can generate one with

```bash
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```

When you're prompted to "Enter a file in which to save the key", you can press <kbd>Enter</kbd> to accept the default file location. Press <kbd>Enter</kbd> again if you want to skip password for the key.

To copy the client public key to the remote server, you can use `ssh-copy-id`

```bash
ssh-copy-id nas

# From now on, you can login to the server from this client without password.
ssh nas
```

## Disable Password Login

To protect the server from password leakage, a good practice is disabling password login and accept only connections from authorized hosts whose public keys are store on the server. 
You can disable the password login in `/etc/ssh/sshd_config` on the server.

```bash
# /etc/ssh/sshd_config

PasswordAuthentication no
```

## Change Default Port

By default, SSH servers run on port number 22. However, probing through default ports is usually the first step attackers take when seeking open SSH server ports. The main advantage of switching to a non-standard port is limiting brute force attacks and port scanner access to your server.
You can change the port in `/etc/ssh/sshd_config` on the server.

```bash
# /etc/ssh/sshd_config

Port 2222

# You have to specify port for ssh
ssh -p 2222 nas
```

You can specify the port in `~/.ssh/config` on the client to be faster:

```bash
# ~/.ssh/config
Host nas
    HostName 192.168.0.8
    User chao
    Port 2222
```

## Disable Root Login

users can SSH to the server as `root` by default. This allows full access to and control over the entire system. To disable root user login, update SSH config file `/etc/ssh/sshd_config` and set option `PermitRootLogin` to `no` as below:

```bash
# /etc/ssh/sshd_config
PermitRootLogin no
```

Sudo users can still do root tasks even when root login is disabled.

## Stay Connected

You can specify the connection timeout in `~/.ssh/config` on the client:

```bash
# ~/.ssh/config
Host nas
    HostName 192.168.0.8
    User chao
    Port 2222
    ServerAliveInterval 60
```
