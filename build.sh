#!/bin/bash

SRC_PATH=$PWD/src
PKG_OUTPUT_DIR=$PWD/build

# Setup
mkdir -p build/

# Build kernel
cd src/kernel
make all
cd -

# Build UEFI Application
cd externals/edk2
source edksetup.sh
cd -
build -p $SRC_PATH/efi/Bootloader.dsc -m $SRC_PATH/efi/Bootloader.inf -a X64 -t GCC5 -b RELEASE -D PKG_OUTPUT_DIR=$PKG_OUTPUT_DIR

# Create FAT32 disk image if doesn't exist
if [[ ! -f "build/uefi.img" ]]; then
    dd if=/dev/zero of=build/uefi.img bs=512 count=93750
    dd if=/dev/zero of=build/part.img bs=512 count=91669

    parted build/uefi.img -s -a minimal mklabel gpt
    parted build/uefi.img -s -a minimal mkpart EFI FAT32 2048s 93716s
    parted build/uefi.img -s -a minimal toggle 1 boot

    mformat -i build/part.img -h 32 -t 32 -n 64 -c 1

    dd if=build/part.img of=build/uefi.img bs=512 count=91669 seek=2048 conv=notrunc
    rm build/part.img
fi

# Copy bootloader and kernel to disk image
mkdir -p build/image_mnt

sudo mount -o loop,offset=1048576 build/uefi.img build/image_mnt
sudo mkdir -p build/image_mnt/EFI/BOOT/
sudo mkdir -p build/image_mnt/EFI/BUBELOS/
sudo cp build/RELEASE_GCC5/X64/Bootloader.efi build/image_mnt/EFI/BOOT/BOOTX64.efi
sudo cp build/kernel/kernel.elf build/image_mnt/EFI/BUBELOS/kernel.elf
sudo umount build/image_mnt

rm -rf build/image_mnt