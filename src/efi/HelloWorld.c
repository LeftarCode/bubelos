#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Uefi.h>

STATIC CONST CHAR16 mem_types[16][27] = {
      L"EfiReservedMemoryType     ",
      L"EfiLoaderCode             ",
      L"EfiLoaderData             ",
      L"EfiBootServicesCode       ",
      L"EfiBootServicesData       ",
      L"EfiRuntimeServicesCode    ",
      L"EfiRuntimeServicesData    ",
      L"EfiConventionalMemory     ",
      L"EfiUnusableMemory         ",
      L"EfiACPIReclaimMemory      ",
      L"EfiACPIMemoryNVS          ",
      L"EfiMemoryMappedIO         ",
      L"EfiMemoryMappedIOPortSpace",
      L"EfiPalCode                ",
      L"EfiPersistentMemory       ",
      L"EfiMaxMemoryType          "
};

EFI_STATUS Keywait(CHAR16 *String)
{
  EFI_STATUS Status;
  EFI_INPUT_KEY Key;
  Print(String);

  Status = gST->ConOut->OutputString(gST->ConOut, L"Press any key to continue...");
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Clear keystroke buffer
  Status = gST->ConIn->Reset(gST->ConIn, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Poll for key
  while ((Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key)) == EFI_NOT_READY);

  // Clear keystroke buffer (this is just a pause)
  Status = gST->ConIn->Reset(gST->ConIn, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Print(L"\r\n");

  return Status;
}

EFI_STATUS EFIAPI EntryPoint(IN EFI_HANDLE imgHandle,
                             IN EFI_SYSTEM_TABLE* sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;
  EFI_STATUS                  Status;
  EFI_MEMORY_DESCRIPTOR       *EfiMemoryMap;
  UINTN                       EfiMemoryMapSize;
  UINTN                       EfiMapKey;
  UINTN                       EfiDescriptorSize;
  UINT32                      EfiDescriptorVersion;
 
  //
  // Get the EFI memory map.
  //
  EfiMemoryMapSize  = 0;
  EfiMemoryMap      = NULL;
  Status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    Print(L"Fatal Error: Error getting memory map size!\n");
    return Status;
  }
 
  //
  // Use size returned for the AllocatePool.
  //
  EfiMemoryMap = (EFI_MEMORY_DESCRIPTOR *) AllocatePool (EfiMemoryMapSize + 2 * EfiDescriptorSize);
  if (EfiMemoryMap == NULL) {
    Print(L"Fatal Error: Error allocating memory map buffer!\n");
    return Status;
  }
  Status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );

  if (EFI_ERROR (Status)) {
    FreePool(EfiMemoryMap);
  }

  for(;;){}

  return EFI_SUCCESS;
}