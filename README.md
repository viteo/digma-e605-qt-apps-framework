# Digma E605 E-Ink reader third-party applications framework

## Contents:

* [Subject](#subject)
* [Obtaining the toolchain](#obtaining-the-toolchain)
* [Example application](#example-application)
* [Deploying](#deploying)
* [Research](#research)
  * [Official Rockchip kernel](#official-rockchip-kernel)
    * [Generating the headers](#generating-the-headers)
  * [Keyboard input](#keyboard-input)
* [Related works](#related-works)
* [References](#references)
* [TODO](#todo)

## Subject
This is a low-cost E-Ink pearl reader, based on Boeye Sibrary C60 platform and SDK.

The application framework has been switched from GTK to QT 4.7 over the course of the years.
Currently this product is discontinued and unsupported by the vendor.

<details>
  <summary>Some technical specs:</summary>
  
#### /proc/cpuinfo
- Processor: ARM926EJ-S rev 5 (v5l)
- CPU architecture: 5TEJ
- Features: swp half thumb fastmult edsp java
- Hardware: RK28board
- Target chip: RK2818
  
#### uname -a
- Linux boeye 2.6.25-dirty #63 Tue Oct 27 15:54:04 HKT 2015 armv5tejl GNU/Linux

#### /proc/version
- Linux version 2.6.25-dirty (drs@drs-desktop) (gcc version 4.4.0 (GCC) ) #63 Tue Oct 27 15:54:04 HKT 2015 rockchip 281x version release v1.09(Eink) used in Eink, without camera video and touch

#### ./lib/libc-2.9.so
- GNU C Library (crosstool-NG 1.12.4) stable release version 2.9
- Compiled by GNU CC version 4.4.3

#### Other
- libQtCore.so.4.7.4 (qt-everywhere-opensource-src-4.7.4)
- libdbus-1.so.3.4.0 (DBUS 1.2.24)

</details>

## Obtaining the toolchain

<details>
	<summary>Prebuild</summary>

Get the [toolchain](https://github.com/viteo/digma-e605-qt-apps-framework/releases/latest) from release section. Unpack both `arm-926ejs-linux-gnueabi.tar.gz` and `rk2818.tar.gz` to the folder of your choice (e.g., `/opt`)

Export path variable
```
export PATH="${PATH}:/opt/arm-926ejs-linux-gnueabi/bin/"
```

</details>

<details>
	<summary>From scratch</summary>

*NB: It builds only on old distros like CentOS 6 because it need old versions of gcc and libc. If you succeed to compile it with actual distros, please let us know.*
To build the toolchain we should use CentOS 6. One might use LiveCD, or Doker, or VM. Here is my approach with WSL2 on Windows 10:
1. Getting CentOS image
   - prepared [image](https://github.com/mishamosher/CentOS-WSL/releases/tag/6.10-1907)
   - export Docker-[image](https://docs.microsoft.com/en-us/windows/wsl/use-custom-distro)
2. Add setting to `%userprofile%\.wslconfig` as in [microsoft/WSL#4694 (comment)](https://github.com/microsoft/WSL/issues/4694#issuecomment-556095344)
   ```
   [wsl2]
   kernelCommandLine = vsyscall=emulate
   ```
3. Import CentOS to WSL and start it
   ```
   wsl --import CentOS path-to-distro-folder path-to-image-archive
   wsl -d CentOS
   ```
4. yum source repo list is outdated, so we need to edit it (according to this [answer](https://stackoverflow.com/a/29559374/5169229))

   - Edit `/etc/yum.repos.d/CentOS-Base.repo`
     - Comment out `mirrorlist` and uncomment `baseurl`
     - Replace all `mirror.centos.org/centos` with `vault.centos.org`
   - Do `yum update`

5. Install prerequisites for building the toolchain
   ```
   yum groupinstall 'Development Tools'
   yum install texinfo ncurses-devel wget
   ```

6. Get crosstool-NG 1.12.4, unpack it, configure, build and install somewhere(`--prefix=/some/place`)
   ```
   wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.12.4.tar.bz2
   tar -xf crosstool-ng-1.12.4.tar.bz2
   cd crosstool-ng-1.12.4
   ./configure --prefix=/opt/ct-ng
   make
   make install
   export PATH="${PATH}:/opt/ct-ng/bin"
   ```

7. Prepare tarball of the original Boeye/Sibrary headers from its SDK:
   ```
   cd digma-e605-qt-apps-framework/headers/2.6.29-ARM-sibrary/usr
   make dist
   ```
   - Also move it (`kernel-headers-2.6.29.tgz`) to a convenient place like `~/`

8. Download toolchain dependency tarballs "manually" because some of crosstool-NG links are dead and some are slow (see inside `dl_tarballs.sh`):
   ```
   cd digma-e605-qt-apps-framework/toolchain
   ./dl_tarballs.sh
   ```

9. Configure and build the toolchain with ct-ng (`.config` file based on `crosstool-ng-1.12.4/samples/arm-unknown-linux-gnueabi/crosstool.config`)

   ```
   cd digma-e605-qt-apps-framework/toolchain
   ct-ng menuconfig
   ```
   > - Target Options: 
   >   - Target Architecture: arm
   >   - Architecture level: armv5te
   >   - Emit Assembly for CPU: arm926ej-s
   >   - Tune for CPU: arm926ej-s
   >   - Use specific FPU: vfp
   >   - Floating point: software
   > - Toolchain options: 
   >   - Tuple's vendor string: 926ejs
   > - Operating System:
   >   - Get kernel headers from: kernel's headers_install
   >   - Linux kernel version: custom tarball
   >   - Path to custom tarball: ${HOME}/kernel-headers-2.6.29.tgz ***#Path where you place    headers from p.7***
   > - C compiler: gcc version (4.4.3)
   >   - Additional supported languages: C++
   > - C-library:
   >   - glibc version (2.9)
   > - Companion libraries:
   >   - MPFR version (3.0.0)
   ```
   ct-ng build
   ```
   It will take a while. Path to the toolchain is `${HOME}/x-tools/arm-926ejs-linux-gnueabi` by default. You might move it somewhere or use it from here. Just export `bin` to the `PATH` (e.g., `export PATH="${PATH}:/opt/arm-926ejs-linux-gnueabi/bin/"`)

10. Building QT libraries
    NB: These libraries, as well as the QT library itself WILL NOT be installed to the target device, but only used on the build machine as build dependencies since the original SDK headers and libraries are unavailable.
    - expat (a dependency of libdbus-1)
    ```
    wget https://ftp.osuosl.org/pub/blfs/conglomeration/expat/expat-2.0.1.tar.gz
    tar -xf expat-2.0.1.tar.gz
    cd expat-2.0.1
    ./configure --host=arm-926ejs-linux-gnueabi --prefix=/opt/rk2818
    make
    make install
    ```
    - libdbus-1 (a dependency of QT)
    ```
    wget https://dbus.freedesktop.org/releases/dbus/dbus-1.2.24.tar.gz
    tar -xf dbus-1.2.24.tar.gz
    cd dbus-1.2.24
    RK2818=/opt/rk2818
    INCPATH=${RK2818}/include
    LIBPATH=${RK2818}/lib
    export CFLAGS=-I${INCPATH}
    export LDFLAGS=-L${LIBPATH}
    export CXXFLAGS=${CFLAGS}
    ./configure --host=arm-926ejs-linux-gnueabi --prefix=/opt/rk2818
    make
    make install
    ```
    - QT
    ```
    wget http://master.qt.io/archive/qt/4.7/qt-everywhere-opensource-src-4.7.4.tar.gz
    tar -xf qt-everywhere-opensource-src-4.7.4.tar.gz
    cd qt-everywhere-opensource-src-4.7.4
    # Use next QT configuration:
    cp -R ~/digma-e605-qt-apps-framework/qt/rk2818-g++ mkspecs/qws/
    patch -p1 < ~/digma-e605-qt-apps-framework/qt/qt-patches/patch-io.pri
    export PKG_CONFIG_PATH=${LIBPATH}/pkgconfig
    # A lot of modules disabled, change if you need something
    ./configure -lrt -opensource -confirm-license -prefix /opt/rk2818 -no-qt3support -embedded arm -little-endian -xplatform qws/rk2818-g++ -fast -no-xinput -no-xrandr -no-openvg -no-opengl -no-gtkstyle -no-nis -no-cups -xmlpatterns -exceptions -no-stl -no-accessibility -no-largefile -no-audio-backend -no-xfixes -no-webkit -no-javascript-jit -no-mitshm -no-script -no-scripttools -qt-gfx-linuxfb -dbus -force-pkg-config -nomake examples -nomake demos
    make
    make install    
    ```
    This will take even longer, but that's it. We are ready.
</details>

## Example application

[firstapp](app/firstapp), originally part of the QT distribution under demos/embedded/digiflip.

```
  cd ~/digma-e605-qt-apps-framework/app/firstapp
  /opt/rk2818/bin/qmake
  make
```

## Deploying:

Copy the binary you built to device's internal memory or SD Card. This could be done through a USB cable, whereas
the SD-CARD would be mounted as a normal disk device.

Once copied (unmount properly and unplug the cable first), just launch the executable from within the device's File Manager - simply click on it.

The [firstapp](app/firstapp) directory contains an installation [script](app/firstapp/install.sh) to integrate the app into the device environment, so as it would be available via a normal launch icon in the "More..." folder in the stock File Manager. Click on the [install.sh](app/firstapp/install.sh) file, when finished copying.

## Research

### Android Debug Bridge Daemon (ADB)
Gives you access to root shell. Copy file [`adbd.sh`](utils/adbd.sh) to internal memory and execute it from the reader's file manager (click once). Then connect to the reader with `adb shell`.

### Official Rockchip kernel

Rockchip officially supports RK2818 under Linux: http://linux-rockchip.org.

The kernel GIT repository:
```
git clone https://github.com/linux-rockchip/linux-rockchip.git 
cd linux-rockchip
# Checkout what appears to be a 2.6.20
git checkout 21d149db093c0d37e67620b281607844529fd0e8
```
NB: After the above commit, RK2818 support has been discontinued.

#### Generating the headers:
```
  make menuconfig
  # Select the RK2818 architecture and VMMU in the menu
  make ARCH=arm INSTALL_HDR_PATH=/tmp/1 \
    CROSS_COMPILE=/home/mac/x-tools/arm-926ejs-linux-gnueabi/bin/arm-926ejs-linux-gnueabi- \
    headers_install
```

### Keyboard input

  The Power, Home, Back, Refresh, OK, Arrow, Page up and Page down keys are
  handled through the standard QT framework, see [keypressapp](app/keypressapp).

  The Virtual keyboard is implemented in */usr/lib/boeye/libboeyeim.so*, see
  the [IM demo application](app/imdemoapp).

## Related works:
* [eView](https://github.com/S-trace/eView) although it works only on GTK firmware.

## Other possible devices
<details>
    <summary>List of devices based on boeye hardware and software</summary>

- Boeye C60A, C60B (original devices)
- Sibrary G5, G6, G10
- Digma E500, E501, E600, E601, E625, Q1000
- Gmini MagicBook Z6
- DNS AirBook EB601
- ORION EN600
- QUMO Colibri, Libro, Libro II
- Ergo Book 0502R, 0604R
</details>

## References

##### Similar works:
* [eView, russian forum](http://www.the-ebook.org/forum/viewtopic.php?p=1040672#1040672)

##### Rockchip platform:
* [Another Android Rockchip device modification page](http://freaktab.com/forum/tv-player-support/rk3188-devices/minix-x7-etc/647213-tutorial-modifying-the-boot-image)

##### Another possible devices:

##### QT:
* [QT Wiki page from Rockchip](http://opensource.rock-chips.com/wiki_Qt)
* [Another installing QT for Raspberry Pi](https://wiki.qt.io/Building_Qt_for_Embedded_Linux)
* [Solving QT build issues](http://bluelimn.tistory.com/entry/Qt-Cross-compile)
* [Another sample QT application](http://doc.qt.io/qt-5/qtwidgets-tools-echoplugin-example.html)

##### imgRePackerRK:
* URL: https://forum.xda-developers.com/showthread.php?t=2257331
* Name: imgRePackerRK_106.zip
* Size: 180485
* MD5: 852bcc8f56694d3658db0d7d5e117093
* SHA1: f17f013c59e29d7376bdb1e856590dbd4a315797

##### Stock DIGMA firmware:
* URL: http://digma.ru
* Name: e605_boot620_20141220.zip
* Size: 136682454 bytes
* MD5: b739257045ec5f32363db0ff3ef936e1
* SHA1: 6f2c9da77ccf88443c4b6f28bbe6655b15f320ca

##### QT Anywhere 4.7.4:
* URL: http://master.qt.io/archive/qt/4.7/qt-everywhere-opensource-src-4.7.4.tar.gz
* Size: 220388303
* MD5: 9831cf1dfa8d0689a06c2c54c5c65aaf
* SHA1: af9016aa924a577f7b06ffd28c9773b56d74c939

## TODO:
- [ ] Refresh issues - the application is not visible after start, unless the screen is forcibly refreshed. NB: Currently the refresh itself doesn't seem to work even through stock device library methods (ProxyScreen class).
- [ ] Put together a demo application to see how the following works:
  - [ ] LED control (see [firstapp/digma_hw.c](app/firstapp/digma_hw.c) for details)
  - [ ] keyboard and backlight checks,
  - [ ] networking, whatever that might mean for the platform,
  - [ ] partial display refresh (see [imdemoapp/main.cpp](app/imdemoapp/main.cpp)),
  - [ ] lots of other stuff from */usr/lib/boeye/libboeye.so*.
  - [ ] virtual keyboard, as it appears to be a singleton object, held by the boeyeserver process, so I wonder if one could use it in own applications?
- [ ] rectify linking to Boeye's shared libraries as currently it's a hack.
- [ ] dig a bit deeper int the DBUS messaging, in particular *com.sibrary.Service.GlobalKey*.
