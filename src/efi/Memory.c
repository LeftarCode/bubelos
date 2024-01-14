#include "Memory.h"

EFI_MEMORY_DESCRIPTOR* GetMemoryMap(IN UINTN* MemoryMapSize, IN UINTN* MapKey, IN UINTN* DescriptorSize, IN UINT32* DescriptorVersion) {
  EFI_STATUS status;
  EFI_MEMORY_DESCRIPTOR *EfiMemoryMap;
 
  UINTN EfiMemoryMapSize;
  UINTN EfiMapKey;
  UINTN EfiDescriptorSize;
  UINT32 EfiDescriptorVersion;

  EfiMemoryMapSize  = 0;
  EfiMemoryMap      = NULL;
  status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  if (status != EFI_BUFFER_TOO_SMALL) {
    Print(L"Fatal Error: Error getting memory map size!. 0x%llx\n", status);
    return NULL;
  }
  
  EfiMemoryMap = (EFI_MEMORY_DESCRIPTOR*)AllocatePool(EfiMemoryMapSize + 2 * EfiDescriptorSize);
  if (EfiMemoryMap == NULL) {
    Print(L"Fatal Error: Error allocating memory map buffer!\n");
    return NULL;
  }

  status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  if (EFI_ERROR (status)) {
    FreePool(EfiMemoryMap);
    return NULL;
  }

  *MemoryMapSize = EfiMemoryMapSize;
  *MapKey = EfiMemoryMapSize;
  *DescriptorSize = EfiDescriptorSize;
  *DescriptorVersion = EfiDescriptorVersion;

  return EfiMemoryMap;
}