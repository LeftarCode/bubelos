#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>
#include "FileSystem.h"
#include "Memory.h"
#include "ELF.h"

EFI_STATUS EFIAPI EntryPoint(IN EFI_HANDLE ImageHandle,
                             IN EFI_SYSTEM_TABLE* sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;

  // UINTN EfiMemoryMapSize;
  // UINTN EfiMapKey;
  // UINTN EfiDescriptorSize;
  // UINT32 EfiDescriptorVersion;
  // EFI_MEMORY_DESCRIPTOR* EfiMemoryMap = GetMemoryMap(&EfiMemoryMapSize, &EfiMapKey, &EfiDescriptorSize, &EfiDescriptorVersion);

  CHAR16 KERNEL_PATH[24] = L"\\EFI\\BUBELOS\\kernel.elf";
  EFI_FILE* KernelFile = OpenFile(ImageHandle, KERNEL_PATH);
  if (KernelFile == NULL) {
    Print(L"Could not load kernel image: %s!", KERNEL_PATH);
    return EFI_NOT_FOUND;
  }

  typedef int(*kmain_t)(void);
  kmain_t KernelMainFunction = NULL;
  LoadELF(KernelFile, (VOID**)&KernelMainFunction);
  FreePool(KernelFile);

  int res = KernelMainFunction();
  Print(L"Res: %x", res);

  for(;;){}

  return EFI_SUCCESS;
}