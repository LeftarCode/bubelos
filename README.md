# BubelOS

## Prerequisites
- GCC 10.x required for building EDK2(stable/202011)
## Clone repo
```
git clone --recurse-submodules https://github.com/LeftarCode/bubelos
```
## Building
### 1. EDK 2
```
cd externals/edk2
source edksetup.sh
make -C BaseTools
cd -
```
### 2. OVMF image (very, very, veryyyyy long ~1h)
You may use your own pre-built OVMF image. If not, it's worth building a image with EDK2.
```
./externals/edk2/OvmfPkg/build.sh -a X64 -t GCC5 -b RELEASE -D PKG_OUTPUT_DIR=$PKG_OUTPUT_DIR
```
### 3. Kernel image
```
./build.sh
```
## Run OS in QEMU
- Spawn UEFI shell:
```
qemu-system-x86_64 -cpu qemu64 -net none -drive if=pflash,format=raw,unit=0,file=./externals/edk2/Build/OvmfX64/RELEASE_GCC5/FV/OVMF_CODE.fd,readonly=on  -drive if=pflash,format=raw,unit=1,file=./externals/edk2/Build/OvmfX64/RELEASE_GCC5/FV/OVMF_VARS.fd -drive file=./build/uefi.img,if=ide
```
- Single OVMF image:
```
qemu-system-x86_64 -cpu qemu64 -net none -bios ./externals/edk2/Build/OvmfX64/RELEASE_GCC5/FV/OVMF.fd -drive file=./build/uefi.img,if=ide
```
## TODO
- Migrate to newer EDK2.
- Create script that runs qemu.
- Split script so EDK2 build/OVMF build and kernel build and seperated