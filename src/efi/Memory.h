#pragma once
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>

EFI_MEMORY_DESCRIPTOR* GetMemoryMap(IN UINTN* EfiMemoryMapSize, IN UINTN* EfiMapKey, IN UINTN* EfiDescriptorSize, IN UINT32* EfiDescriptorVersion);