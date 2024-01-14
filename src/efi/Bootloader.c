#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>
#include "FileSystem.h"
#include "Memory.h"
EFI_STATUS EFIAPI EntryPoint(IN EFI_HANDLE ImageHandle,
                             IN EFI_SYSTEM_TABLE* sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;

  UINTN EfiMemoryMapSize;
  UINTN EfiMapKey;
  UINTN EfiDescriptorSize;
  UINT32 EfiDescriptorVersion;
  EFI_MEMORY_DESCRIPTOR* EfiMemoryMap = GetMemoryMap(&EfiMemoryMapSize, &EfiMapKey, &EfiDescriptorSize, &EfiDescriptorVersion);

  CHAR16 KERNEL_PATH[24] = L"\\EFI\\BUBELOS\\kernel.elf";
  EFI_FILE* KernelFile = OpenFile(ImageHandle, KERNEL_PATH);
  if (KernelFile == NULL) {
    Print(L"Could not load \\EFI\\BUBELOS\\kernel.elf file!");
    return EFI_NOT_FOUND;
  }

  // TODO: Remember to Free FileInfo
  EFI_FILE_INFO* KernelFileInfo = GetFileInfo(KernelFile);
  if (KernelFileInfo == NULL) {
    Print(L"Could not get \\EFI\\BUBELOS\\kernel.elf info!");
    return EFI_NOT_FOUND;
  }

  Print(L"FileName: %s\r\n", KernelFileInfo->FileName);
  Print(L"Size: %llu\r\n", KernelFileInfo->Size);
  Print(L"FileSize: %llu\r\n", KernelFileInfo->FileSize);
  Print(L"PhysicalSize: %llu\r\n", KernelFileInfo->PhysicalSize);
  Print(L"Attribute: %llx\r\n", KernelFileInfo->Attribute);

  FreePool(EfiMemoryMap);

  for(;;){}

  return EFI_SUCCESS;
}