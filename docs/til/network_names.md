---
comments: true
tags:
  - Ubuntu
---

# Network Interface Name on Ubuntu

Ubuntu changed to **predictable network interface names** (like wlp0s20f3 or enp3s0) starting with Ubuntu 15.10 (Wily Werewolf). 
`systemd` is adopted to replace old, non-persistent kernel enumeration (eth0, wlan0) with names based on hardware topology and physical location.

## Why the Naming Scheme Changed

Old names like eth0 or wlan0 depended on the order the kernel discovered devices.Discovery order could swap randomly on reboot if multiple devices were present.
New names use predictable identifiers tied to firmware, slots, or PCI bus paths.

## Breakdown of wlp0s20f3

- wl: Wireless local area network (en is used for ethernet).
- p0: PCI bus 0.
- s20: Slot 20.
- f3: Function 3 on that device.

## How to Revert to Old Names (eth0/wlan0)

1. Edit GRUB configuration file (/etc/default/grub).
2. Add `net.ifnames=0` to the `GRUB_CMDLINE_LINUX_DEFAULT` line.
3. Run sudo update-grub and reboot the system.
