---
title: Guide for Jetpack 7.2 on A603 Carrierboard with Realsense driver fix 
description: guide for flashing Jetpack 6.2 to Jetson Orin NX on a A603 carrier board.
comments: true
tags:
  - Nvidia
  - Jetson
  - Realsense
---

## Verify Hardware

This guild is only for Jetson Orin NX on SeeedStudio A603 carrier board. Before following further steps in this guide, you have to confirm that you have the hardware setup this guide aims to.

### Jetson Orin NX

A603 board supports only Orin NX, it is necessary to confirm that you have the correct chip. You can read the model on the PCB board. P3767 is for Orin NX and P3668 is for Xavier.

## Prepare files for flashing

### Download Files to same folder

1. Peripheral Drivers: [603_jp72.tbz2](https://seeedstudio88-my.sharepoint.com/:u:/g/personal/youjiang_yu_seeedstudio88_onmicrosoft_com/IQDFKQLWsQBBTrenUxxvj-qJAU4s62oPXWg6RxcdSg-uJnY?e=y3buDr)

2. Jetson BSP:

```bash
wget https://developer.nvidia.com/downloads/embedded/L4T/r39_Release_v2.0/release/Jetson_Linux_R39.2.0_aarch64.tbz2
```

3. Sample Root Filesystem:

```bash
wget https://developer.nvidia.com/downloads/embedded/L4T/r39_Release_v2.0/release/Tegra_Linux_Sample-Root-Filesystem_R39.2.0_aarch64.tbz2
```

### Extract Files

1. Extract BSP:

```bash
tar xf Jetson_Linux_R39.2.0_aarch64.tbz2
```

2. Extract sample root file system to rootfs folder:

```bash
sudo tar xpf Tegra_Linux_Sample-Root-Filesystem_R39.2.0_aarch64.tbz2 -C Linux_for_Tegra/rootfs/
```

3. Extract board drivers:

```bash
mkdir -p 603_jp72/ && cp 603_jp72.tbz2 603_jp72/ && cd 603_jp72 && sudo tar xf 603_jp72.tbz2
```

4. Run preparation scripts:

```bash
cd ../Linux_for_Tegra/
sudo ./tools/l4t_flash_prerequisites.sh
sudo ./apply_binaries.sh
```

### Create User and Set Hostname

```bash
sudo ./tools/l4t_create_default_user.sh -u <user> -p <password> -n <hostname> --accept-license
```

### Mix-in the board drivers

```bash
cp -r ../603_jp72/bootloader/ ./
cp -r ../603_jp72/kernel/ ./
cp ../603_jp72/p3768-0000-p3767-0000-a0.conf ./
sudo cp -r ../603_jp72/rootfs/ ./
```
​
### Fix debugging USB port

Modify rootfs/opt/nvidia/l4t-usb-device-mode/nv-l4t-usb-device-mode-start.sh to set the debugging usb port to device mode.

Change `if [ "${current_role}" = "none" ]; to if [ "${current_role}" != "device" ];

## Flash Jetson Orin NX
### Enter Force Recovery Mode

<img width="640" height="360" alt="image" src="https://github.com/user-attachments/assets/0d23864c-0d62-4adc-aed9-5d18eb43946f" />

1. Connect pin 3 and pin 4 with a jumper.
2. Connect the host machine to the board's Micro-USB port.
3. Connect the power supply to the DC jack on the board.
4. Confirm that the board is in recovery mode on the host machine.

```bash
lsusb

# You should see an entry for the Jetson Orin NX board.
Bus 001 Device 023: ID 0955:7323 NVIDIA Corp. APX
```

The board is in recovery mode if the device ID is **7323**. If not, the board is either not in recovery mode or is a different Jetson module. For example, **7e19** indicates a Jetson Xavier NX.

### Run Flash Command

```bash
sudo ./l4t_initrd_flash.sh --erase-all jetson-orin-nano-devkit-super internal
```

### Reboot

When the flash script completes successfully, remove the jumper and reconnect the power supply to reboot the board into normal mode.

## Set Up Network Connection

### Log In from the Host Machine

After the initial boot, connect the Jetson board to the local network. There are two ways to log in to the Jetson Linux system remotely, both using the USB connection.

**USB Network**

```bash
ssh uc1@192.168.55.1
```

**Troubleshooting**

If the `ssh` command does not work, check the USB connection and network interfaces.

```bash
# Check that the board is connected. There should be an entry for Jetson Linux.
lsusb

# Check the USB network interface. There should be an interface with IP address 192.168.55.100.
ifconfig
```

If the IP address does not appear, identify the USB network interface by comparing the interface list before and after connecting the USB cable. Then manually assign the static IP address:

```bash
sudo ip addr add 192.168.55.100/24 dev <interface_name>
sudo ip link set <interface_name> up
```

**Serial Console**

When the Jetson is connected via the USB port, the serial device should appear as `/dev/ttyACM*`.

```bash
screen /dev/ttyACM0 115200
```
​
### Fix WLAN interface if missing

Please follow ​[Wireless Module Setup Guide](https://wiki.seeedstudio.com/jetpack72_ax210_ax200_wifi_setup_guide/#option-1-one-click-repair-script)

### Configure Network
```bash
sudo nmtui
# Configure new network connection via the terminal UI
```
​
### Assign static IP

To connect to it easily, it is necessary to assign a static IP address to the newly flashed Jetson board on your Router.

Add the robot IP to `/etc/hosts` on your dev machine.
```bash
sudo nano /etc/hosts
```
 
## Install full Jetpack SDK

After flashing, the base Jetson Linux system is installed on to the NVMe SSD on the board. The full JetPack components, including CUDA, cuDNN, TensorRT, etc, need to be added manually. To ensure the full JetPack SDK is installed, you should install nvidia-jetpack after flashing.

It is highly recommended to connect the Jetson board to a high-bandwidth network to accelerate downloading process (over 10GB) and avoid high cost using cellular network.
```bash
sudo apt update
sudo apt dist-upgrade
sudo apt install -y nvidia-jetpack
sudo apt autoremove -y
```

## Patch Kernel for Realsense [optional]

Kernel module for HID devices is dropped since Jetpack 6. Consequently, Realsense SDK can’t find the IMU device on the camera.
Following steps patch the kernel to fix the issue.

1. Connect to Jetson host OS via ssh
2.Copy and run the following command create the patch script
```bash
cat <<'EOF' > patch-realsense-ubuntu-L4T.sh
#!/bin/bash
# The script utilizes `sources_sync.sh` provided as
# part of NVIDIA(R) SDK installer
set -e

echo -e "\e[32mThe script patches and applies in-tree kernel modules required for Librealsense SDK\e[0m"

#Locally suppress stderr to avoid raising not relevant messages
exec 3>&2
exec 2> /dev/null
con_dev=$(ls /dev/video* | wc -l)
exec 2>&3

function version_lt {
    IFS='.' read -r -a v1 <<< "$1"
    IFS='.' read -r -a v2 <<< "$2"
    for i in 0 1 2; do
        [[ v1[i] -lt v2[i] ]] && return 0
        [[ v1[i] -gt v2[i] ]] && return 1
    done
    return 1
}

git -C /tmp/ clone --depth 1 --single-branch -b master https://github.com/IntelRealSense/librealsense.git
cd /tmp/librealsense

if [[ $con_dev -ne 0 ]];
then
    echo -e "\e[32m"
    read -p "Remove all RealSense cameras attached. Hit any key when ready"
    echo -e "\e[0m"
fi

#Include usability functions
source ./scripts/patch-utils-hwe.sh

#Activate fan to prevent overheat during KM compilation
if [[ -f /sys/devices/pwm-fan/target_pwm ]]; then
    echo 200 | sudo tee /sys/devices/pwm-fan/target_pwm || true
fi

#Tegra-specific
KERNEL_RELEASE="4.9"
#Identify the Jetson board
JETSON_BOARD=$(tr -d '\0' </proc/device-tree/model)
echo -e "\e[32mJetson Board (proc/device-tree/model): ${JETSON_BOARD}\e[0m"

JETSON_L4T=""
# With L4T 32.3.1, NVIDIA added back /etc/nv_tegra_release
if [[ -f /etc/nv_tegra_release ]]; then
    JETSON_L4T_STRING=$(head -n 1 /etc/nv_tegra_release)
    JETSON_L4T_RELEASE=$(echo $JETSON_L4T_STRING | cut -f 2 -d ' ' | grep -Po '(?<=R)[^;]+')
    # Extract revision + trim trailing zeros to convert 32.5.0 => 32.5 to match git tags
    JETSON_L4T_REVISION_LONG=$(echo $JETSON_L4T_STRING | cut -f 2 -d ',' | grep -Po '(?<=REVISION: )[^;]+')
    JETSON_L4T_REVISION=$(echo $JETSON_L4T_REVISION_LONG | sed 's/.0$//g')
    JETSON_L4T_VERSION=$JETSON_L4T_RELEASE.$JETSON_L4T_REVISION
    echo -e "\e[32mJetson L4T version: ${JETSON_L4T_VERSION}\e[0m"
else
    echo -e "\e[41m/etc/nv_tegra_release not present, aborting script\e[0m"
    exit;
fi

# setting UBUNTU_CODENAME
[[ -f /etc/os-release ]] && eval $(cat /etc/os-release|grep UBUNTU_CODENAME=)

#Select the kernel patches revision that matches the paltform configuration
RELEASE_STRING="release"
case ${JETSON_L4T_VERSION} in
    36.3 | 36.4 | 36.4.3 | 36.4.4 | 36.4.7 | 36.5)
        # 36.3 --> JP 6.0
        # 36.4 -> JP 6.1
        # 36.4.3 --> JP 6.2
        # 36.4.4, 36.4.7 --> JP 6.2.1
        # 36.5 --> JP 6.2.2
        PATCHES_REV=6.0
        KERNEL_RELEASE=5.15
        UBUNTU_CODENAME=jammy
        KBASE=./Tegra/kernel/kernel-${UBUNTU_CODENAME}-src
    ;;
    38.2 | 38.2.1 | 38.2.2)
        # for revisions 38 licence link is inconsistent
        JETSON_L4T_REVISION_LONG=2.0
        # 38.2 --> JP 7.0
        ;&
    38.4)
        # 38.4 --> JP 7.1
        PATCHES_REV=7.0
        KERNEL_RELEASE=6.8
        UBUNTU_CODENAME=noble
        KBASE=./Tegra/kernel/kernel-${UBUNTU_CODENAME}-src
    ;;
  39.2)
        # 39.2 --> JP 7.2
        JETSON_L4T_REVISION_LONG=2.0
        PATCHES_REV=7.0
        KERNEL_RELEASE=6.8
        UBUNTU_CODENAME=noble
        KBASE=./Tegra/kernel/kernel-${UBUNTU_CODENAME}-src
    ;;
    *)
    echo -e "\e[41mUnsupported JetPack revision ${JETSON_L4T_VERSION} aborting script\e[0m"
    exit 1;
    ;;
esac
echo -e "\e[32mL4T ${JETSON_L4T_VERSION} to use patches revision ${PATCHES_REV}\e[0m"

# Get the required tools to build the patched modules
sudo apt-get install build-essential git libssl-dev curl -y

#Retrieve tegra tag version for sync, required for get and sync kernel source with Jetson:
#https://forums.developer.nvidia.com/t/r32-1-tx2-how-can-i-build-extra-module-in-the-tegra-device/72942/9
#Download kernel and peripheral sources as the L4T github repo is not self-contained to build kernel modules
sdk_dir=$(pwd)
echo -e "\e[32mCreate the sandbox - NVIDIA L4T source tree(s)\e[0m"
mkdir -p ${sdk_dir}/Tegra
TEGRA_SOURCE_SYNC_SH="sync.sh"
if [[ "${JETSON_L4T_RELEASE}" -ge 39 ]]; then
  TEGRA_TAG="jetson_${JETSON_L4T_RELEASE}.${JETSON_L4T_REVISION_LONG}"
else
    TEGRA_TAG="jetson_${JETSON_L4T_RELEASE}.${JETSON_L4T_REVISION}"
fi

cp ./scripts/Tegra/$TEGRA_SOURCE_SYNC_SH ${sdk_dir}/Tegra
cp ./scripts/Tegra/${PATCHES_REV}.repos ${sdk_dir}/Tegra/repos


# Download NVIDIA source
./Tegra/$TEGRA_SOURCE_SYNC_SH -k ${TEGRA_TAG}

pushd ${KBASE} > /dev/null

L4T_Patches_Dir=${sdk_dir}/scripts/Tegra/LRS_Patches/
if [[ ! -d ${L4T_Patches_Dir} ]]; then
    echo -e "\e[41mThe L4T kernel patches directory  ${L4T_Patches_Dir} was not found, aborting\e[0m"
    exit 3
else
    echo -e "\e[32mCopy LibRealSense patches to the sandbox\e[0m"
    cp -r ${L4T_Patches_Dir} .
fi

#Clean the kernel WS
echo -e "\e[32mPrepare workspace for kernel build\e[0m"

export ARCH=arm64

RUNNING_KERNEL=/lib/modules/$(uname -r)
if [[ -L $RUNNING_KERNEL/build ]]; then
    export KBUILD_EXTRA_SYMBOLS=$RUNNING_KERNEL/build/Module.symvers
fi

make mrproper -j$(($(nproc)-1))
if version_lt "$PATCHES_REV" "6.0"; then
    make tegra_defconfig -j$(($(nproc)-1))
else
    echo -e "\e[32mUpdate the kernel tree to support HID IMU sensors\e[0m"
    # appending config to defconfig so later .config will be generated with all necessary dependencies
    echo 'CONFIG_HID_SENSOR_HUB=m' >> arch/arm64/configs/defconfig
    echo 'CONFIG_HID_SENSOR_ACCEL_3D=m' >> arch/arm64/configs/defconfig
    echo 'CONFIG_HID_SENSOR_GYRO_3D=m' >> arch/arm64/configs/defconfig
    echo 'CONFIG_HID_SENSOR_IIO_COMMON=m' >> arch/arm64/configs/defconfig
    echo 'CONFIG_HID_SENSOR_IIO_TRIGGER=m' >> arch/arm64/configs/defconfig
    make defconfig -j$(($(nproc)-1))
fi

#JetPack prior to 4.4.1 requires manual reconfiguration of kernel
if [[ "$PATCHES_REV" = "4.4" ]]; then
    echo -e "\e[32mUpdate the kernel tree to support HID IMU sensors\e[0m"
    sed -i '/CONFIG_HID_SENSOR_ACCEL_3D/c\CONFIG_HID_SENSOR_ACCEL_3D=m' .config
    sed -i '/CONFIG_HID_SENSOR_GYRO_3D/c\CONFIG_HID_SENSOR_GYRO_3D=m' .config
    sed -i '/CONFIG_HID_SENSOR_IIO_COMMON/c\CONFIG_HID_SENSOR_IIO_COMMON=m\nCONFIG_HID_SENSOR_IIO_TRIGGER=m' .config
fi

#Remove previously applied patches
git reset --hard
echo -e "\e[32mApply LibRealSense kernel patches\e[0m"
if version_lt "${PATCHES_REV}" "6.0"; then
    patch -p1 < ./LRS_Patches/01-realsense-camera-formats-L4T-${PATCHES_REV}.patch
    patch -p1 < ./LRS_Patches/02-realsense-metadata-L4T-${PATCHES_REV}.patch
    if [[ "$PATCHES_REV" = "4.4" ]]; then # for JetPack 4.4 only
        patch -p1 < ./LRS_Patches/03-realsense-hid-L4T-4.9.patch
    fi
    if [[ "$PATCHES_REV" != "5.0" ]]; then
        patch -p1 < ./LRS_Patches/04-media-uvcvideo-mark-buffer-error-where-overflow.patch
    fi
    patch -p1 < ./LRS_Patches/05-realsense-powerlinefrequency-control-fix.patch
else
    patch -p1 < ${sdk_dir}/scripts/realsense-camera-formats-"${UBUNTU_CODENAME}"-master.patch
    patch -p1 < ${sdk_dir}/scripts/realsense-metadata-"${UBUNTU_CODENAME}"-master.patch
    [[ -f ${sdk_dir}/scripts/realsense-powerlinefrequency-control-fix-"${UBUNTU_CODENAME}".patch ]] \
        && patch -p1 < ${sdk_dir}/scripts/realsense-powerlinefrequency-control-fix-"${UBUNTU_CODENAME}".patch
    [[ -f ${sdk_dir}/scripts/makefile-${UBUNTU_CODENAME}-${PATCHES_REV}.patch ]] \
        && patch -p1 < ${sdk_dir}/scripts/makefile-${UBUNTU_CODENAME}-${PATCHES_REV}.patch
    sed -i s'/1.1.1/1.1.1-realsense/'g ./drivers/media/usb/uvc/uvcvideo.h
fi

#Building modules_prepare, which:
#1. Prepares kernel headers for building external modules
#2. Generates Module.symvers if it doesn’t already exist.
# Extract the local version suffix from the running kernel (e.g., "-tegra" or "")
KERNEL_LOCALVERSION="$(uname -r | sed -E 's/^[0-9]+\.[0-9]+\.[0-9]+(-[0-9]+)?//')"
echo -e "\e[32mPrepare\e[0m"
make prepare modules_prepare LOCALVERSION="${KERNEL_LOCALVERSION}" -j$(($(nproc)-1))

echo -e "\e[32mCompiling uvcvideo kernel module\e[0m"
make -j$(($(nproc)-1)) M=drivers/media/usb/uvc/ modules
echo -e "\e[32mCompiling v4l2-core modules\e[0m"
make -j$(($(nproc)-1)) M=drivers/media/v4l2-core modules

if [[ "$PATCHES_REV" = "4.4" ]]; then # for JetPack 4.4 only
    echo -e "\e[32mCompiling accelerometer and gyro modules\e[0m"
    make -j$(($(nproc)-1)) M=drivers/iio modules
fi
if version_lt "$PATCHES_REV" "6.0"; then # for JetPack 4-5
    echo -e "\e[32mCopying the patched modules to (~/) \e[0m"
    sudo cp drivers/media/usb/uvc/uvcvideo.ko ~/${TEGRA_TAG}-uvcvideo.ko
    sudo cp drivers/media/v4l2-core/videobuf-vmalloc.ko ~/${TEGRA_TAG}-videobuf-vmalloc.ko
    sudo cp drivers/media/v4l2-core/videobuf-core.ko ~/${TEGRA_TAG}-videobuf-core.ko
else
    echo -e "\e[32mCompiling hid support, accelerometer and gyro modules\e[0m"
    make -j$(($(nproc)-1)) M=drivers/hid modules
    if [[ -n ${KBUILD_EXTRA_SYMBOLS} ]]; then
        grep -w drivers/hid/hid-sensor-hub ${KBUILD_EXTRA_SYMBOLS} || KBUILD_EXTRA_SYMBOLS+=" drivers/hid/Module.symvers"
    fi
    make -j$(($(nproc)-1)) M=drivers/iio modules
fi
if [[ "$PATCHES_REV" = "4.4" ]]; then # for JetPack 4.4 only
    sudo cp drivers/iio/common/hid-sensors/hid-sensor-iio-common.ko ~/${TEGRA_TAG}-hid-sensor-iio-common.ko
    sudo cp drivers/iio/common/hid-sensors/hid-sensor-trigger.ko ~/${TEGRA_TAG}-hid-sensor-trigger.ko
    sudo cp drivers/iio/accel/hid-sensor-accel-3d.ko ~/${TEGRA_TAG}-hid-sensor-accel-3d.ko
    sudo cp drivers/iio/gyro/hid-sensor-gyro-3d.ko ~/${TEGRA_TAG}-hid-sensor-gyro-3d.ko
fi

if ! version_lt "$PATCHES_REV" "6.0"; then # from JetPack 6 onward
    echo -e "\e[32mCopying the patched modules to $RUNNING_KERNEL/extra/\e[0m"
    sudo mkdir -p $RUNNING_KERNEL/extra/
    # uvc modules with formats/sku support
    sudo cp drivers/media/usb/uvc/uvcvideo.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/media/v4l2-core/videodev.ko $RUNNING_KERNEL/extra/
    # iio modules for iio-hid support
    sudo cp drivers/iio/buffer/kfifo_buf.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/iio/buffer/industrialio-triggered-buffer.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/iio/common/hid-sensors/hid-sensor-iio-common.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/hid/hid-sensor-hub.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/iio/accel/hid-sensor-accel-3d.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/iio/gyro/hid-sensor-gyro-3d.ko $RUNNING_KERNEL/extra/
    sudo cp drivers/iio/common/hid-sensors/hid-sensor-trigger.ko $RUNNING_KERNEL/extra/
    # set depmod search path to include "extra" modules
    sudo sed -i 's/search updates/search extra updates/g' /etc/depmod.d/ubuntu.conf
fi
popd > /dev/null
if [[ "$PATCHES_REV" = "4.4" ]]; then # for JetPack 4.4 only
    echo -e "\e[32mMove the modified modules into the modules tree\e[0m"
    #Optional - create kernel modules directories in kernel tree
    sudo mkdir -p $RUNNING_KERNEL/kernel/drivers/iio/accel
    sudo mkdir -p $RUNNING_KERNEL/drivers/iio/gyro
    sudo mkdir -p $RUNNING_KERNEL/drivers/iio/common/hid-sensors
    sudo cp  ~/${TEGRA_TAG}-hid-sensor-accel-3d.ko     $RUNNING_KERNEL/kernel/drivers/iio/accel/hid-sensor-accel-3d.ko
    sudo cp  ~/${TEGRA_TAG}-hid-sensor-gyro-3d.ko      $RUNNING_KERNEL/kernel/drivers/iio/gyro/hid-sensor-gyro-3d.ko
    sudo cp  ~/${TEGRA_TAG}-hid-sensor-iio-common.ko   $RUNNING_KERNEL/kernel/drivers/iio/common/hid-sensors/hid-sensor-iio-common.ko
    sudo cp  ~/${TEGRA_TAG}-hid-sensor-trigger.ko      $RUNNING_KERNEL/kernel/drivers/iio/common/hid-sensors/hid-sensor-trigger.ko
fi

# update kernel module dependencies
sudo depmod

# special attention to uvcvideo because it is one of the files that is set to /lib/modules/`uname -r`/updates/ folder
# when using our jetson drivers instructions
UVCVIDEO_PATH=$(modinfo -F filename uvcvideo)
if [[ -z "$UVCVIDEO_PATH" ]]; then
    UVCVIDEO_PATH="$RUNNING_KERNEL/updates/uvcvideo.ko"
    echo -e "\e[33mCould not find the uvcvideo kernel module.\nIt will be loaded into updates folder: $UVCVIDEO_PATH.\e[0m"
fi

echo -e "\e[32mInsert the modified kernel modules\e[0m"

if version_lt "$PATCHES_REV" "6.0"; then
    try_module_insert uvcvideo              ~/${TEGRA_TAG}-uvcvideo.ko                $UVCVIDEO_PATH
    try_module_insert hid_sensor_gyro_3d    ~/${TEGRA_TAG}-hid-sensor-gyro-3d.ko      $RUNNING_KERNEL/kernel/drivers/iio/gyro/hid-sensor-gyro-3d.ko
    try_module_insert hid_sensor_accel_3d   ~/${TEGRA_TAG}-hid-sensor-accel-3d.ko     $RUNNING_KERNEL/kernel/drivers/iio/accel/hid-sensor-accel-3d.ko
else
    # for JP6.0 we will try to remove old modules and then load updated ones
    echo -e "\e[32mUnload kernel modules\e[0m"
    try_unload_module uvcvideo
    try_unload_module hid_sensor_accel_3d
    try_unload_module hid_sensor_gyro_3d
    try_unload_module hid_sensor_trigger
    try_unload_module industrialio_triggered_buffer
    try_unload_module kfifo_buf
  try_unload_module videodev || true
    echo -e "\e[32mLoad modified kernel modules\e[0m"
    try_load_module videodev || true
    try_load_module kfifo_buf
    try_load_module industrialio_triggered_buffer
    try_load_module hid_sensor_trigger
    try_load_module hid_sensor_gyro_3d
    try_load_module hid_sensor_accel_3d
    try_load_module uvcvideo
fi
echo -e "\e[32mDone\e[0m"
echo -e "\e[92m\n\e[1mScript has completed. Please consult the installation guide for further instruction.\n\e[0m"
EOF

chmod +x patch-realsense-ubuntu-L4T.sh
```

3. Run script
```bash
bash patch-realsense-ubuntu-L4T.sh
```
