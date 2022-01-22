# Examples for LPC1343 QSB

## Introduction

Small examples for LPC1343 QuickStart Board (QSB), includes:
- **eltica** blinks the LED in 1-second cycles.
- **sw2** turn on the LED while the switch SW2 is pressed.

See the URL for details (in Japanese):<br>
[https://retrotecture.jp](https://retrotecture.jp)

## Requirements

- Embedded Artists LPC1343 QSB
- POSIX oriented operating system (FreeBSD is recommended)
- GNU Arm Embedded Toolchain (arm-none-eabi-gcc) 8.4.0
- GNU Make 4.3

Other versions may work.<br>
In addition, the followings are required for JTAG debugging, See [here](#debugging-with-gdb).
- OLIMEX ARM-USB-TINY-H
- OLIMEX ARM-JTAG-20-10
- OLIMEX ARM-JTAG-SWD
- GDB (arm-none-eabi-gdb) 11.1
- OpenOCD 0.11.0
- libFTDI 0.20

## Building

Move to the each  project's directory, and run 'make', or 'gmake' in some cases.
```
% cd path/to/lpc1343qsb-examples/eltica/
% gmake
```
This makes the firmware image file (*.bin) into the project's sub directory named 'build'.

Depending on your environment, tools/lpcsum.sh, which adds the checksum into the image file,
may not work. In that case, you need to modify the script to fit your environment,
or make an equivalent program in C, Perl, or some languages of your choice.
About checksum, see section 21.7 of the LPC13xx User Manual (UM10375) Rev.5.

## Writing to the flash memory

1. Turn on the board by connecting to the computer via USB, while pressing the switch SW2.
   This makes the flash memory recognized as a USB Mass Storage device.
2. Mount it on some directory.
3. Delete (or move to somewhere, if you need) the 'firmware.bin' that is originally there.
4. Copy your firmware to the mounted directory. The file name can be anything.
5. Unmount the directory.

in case FreeBSD, for example:
```
# mount_msdosfs /dev/da1 /media
# rm /media/firmware.bin
# cp path/to/lpc1343qsb-examples/eltica/build/eltica.bin /media
# umount /media
```
If you use FreeBSD, tools/lpcwrite.sh automatically do this.

## Debugging with GDB

1. Modify the value of 'DEBUG' in Makefile 0 to 1, and rebuild the example.
2. Connect the board, ARM-USB-TINY-H, and its attachments like a chain, and
   connect both ends of the chain to the computer with the USB cables.
3. Open a terminal emurator, and run OpenOCD.
   ```
   # cd path/to/lpc1343qsb-examples/
   # openocd -f lpc1343qsb.cfg -c "init" -c "reset halt"
   ```
4. Open the another terminal emurator, and run GDB. It starts in text interface mode.
   ```
   % cd path/to/lpc1343qsb-examples/eltica/build/
   % arm-none-eabi-gdb -tui --eval-command="target remote localhost:3333" eltica.elf
   ```
5. In GDB, run load command to download the ELF file, and run 'c'ontinue command to
   start the program.
   ```
   (gdb) load
   (gdb) c
   ```

#### Quick installation guide for FreeBSD

In case FreeBSD, GDB is not included in packages/ports, so I append a quick
installation guide as shown below.

1. Get gdb-x.x.tar.xz from ftp.gnu.org/gnu/gdb/ or their mirror sites.
2. Install the packages required for building GDB.<br>
   gcc, gmake, gmp, expat, guile, iconv, lzma, mpfr, and python, in my case.
3. Extract, make, and install.
   ```
   % tar Jxf gdb-11.1.tar.xz
   % mkdir build-gdb-11.1
   % ls
   build-gdb-11.1/    gdb-11.1/
   % cd build-gdb-11.1
   % ../gdb-11.1/configure --target=arm-none-eabi --prefix=/usr/local
   % gmake
   # gmake install
   ```
4. Install openocd and libftdi with packages/ports.

## License

Released under the MIT License, see LICENSE.
