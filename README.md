# Bluetooth Mesh Node - Light

## Overview

This light sample demonstrates Bluetooth Mesh functionality. It has several standard Mesh models, and supports provisioning over both the Advertising and the GATT Provisioning Bearers (i.e. PB-ADV and PB-GATT).

It's powered by [Zephyr](https://zephyrproject.org) with version 2.0.0-rc1

## Requirements

* A [micro:bit](https://microbit.org/)

## Setup

### Setup a Zephyr development environment

> Take Ubuntu as a example, more please refer to: [Zephyr - Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)


1. Make sure ~/.local/bin is on your PATH environment variable

```bash
export PATH=~/.local/bin:$PATH
```
2. Install Linux Host Dependencies

```bash
# Ubuntu
$ sudo apt-get install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib
```
3. Install the Zephyr Software Development Kit (SDK)

```bash
$ wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.10.2/zephyr-sdk-0.10.2-setup.run
$ sudo chmod +x zephyr-sdk-0.10.2-setup.run
# Install to ~/zephyr-sdk-0.10.2 for example
$ ./zephyr-sdk-0.10.2-setup.run -- -d ~/zephyr-sdk-0.10.2
```

4. Set these environment variables

Add below lines to ~/.bashrc

```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=$HOME/zephyr-sdk-0.10.2
```

```bash
$ source ~/.bashrc
```

5. Install west

```bash
$ pip3 install --user -U west
```

### Get the zephyr source code

```bash
$ cd
# --mr MANIFEST_REV, Manifest revision to fetch (default: master), v2.0.0-rc1
$ west init --mr master zephyrproject
```

## Building

```bash
$ git clone https://github.com/ahnniu/mesh_node_light.git light
$ cd light
$ source ~/zephyrproject/zephyr/zephyr-env.sh
$ west build
```
The .elf / .hex / .bin will be generated in ~/path/to/light/build/zephyr/, copy the zephyr.hex into the micro:bit (it will be acted as a u-disk when attahced to the PC)

## Running

Then you can use meshctl to provision it.

