#!/bin/bash

SRC_PATH=$PWD/src
PKG_OUTPUT_DIR=$PWD/build

# Setup
mkdir -p build/
rm build/uefi.img

# Build EDK2
#cd externals/edk2
#source edksetup.sh
#make -C BaseTools
#cd -

# Build UEFI Application
build -p $SRC_PATH/efi/HelloWorld.dsc -m $SRC_PATH/efi/HelloWorld.inf -a X64 -t GCC5 -b RELEASE -D PKG_OUTPUT_DIR=$PKG_OUTPUT_DIR

# Pack to FAT32 disk image
if [[ -f "build/uefi.img" ]]; then
    echo "build/uefi.img exists."
fi
dd if=/dev/zero of=build/uefi.img bs=512 count=93750
dd if=/dev/zero of=build/part.img bs=512 count=91669

parted build/uefi.img -s -a minimal mklabel gpt
parted build/uefi.img -s -a minimal mkpart EFI FAT32 2048s 93716s
parted build/uefi.img -s -a minimal toggle 1 boot

mformat -i build/part.img -h 32 -t 32 -n 64 -c 1

dd if=build/part.img of=build/uefi.img bs=512 count=91669 seek=2048 conv=notrunc
rm build/part.img

mkdir -p build/image_mnt

sudo mount -o loop,offset=1048576 build/uefi.img build/image_mnt
sudo mkdir -p build/image_mnt/EFI/BOOT/
sudo cp build/RELEASE_GCC5/X64/HelloWorld.efi build/image_mnt/EFI/BOOT/BOOTX64.efi
sudo umount build/image_mnt

rm -rf build/image_mnt