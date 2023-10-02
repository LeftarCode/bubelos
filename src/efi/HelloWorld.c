#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

EFI_STATUS EFIAPI EntryPoint(IN EFI_HANDLE imgHandle,
                             IN EFI_SYSTEM_TABLE* sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;
  gImageHandle = imgHandle;
  // UEFI apps automatically exit after 5 minutes. Stop that here
  gBS->SetWatchdogTimer(100, 0, 0, NULL);
  Print(L"Hello, world!\r\n");
  // Allocate a string
  CHAR16* str = NULL;
  gBS->AllocatePool(EfiLoaderData, 36, (VOID**)&str);
  // Copy over a string
  CHAR16* str2 = L"Allocated string\r\n";
  gBS->CopyMem((VOID*)str, (VOID*)str2, 36);
  Print(str);
  gBS->FreePool(str);
  return EFI_SUCCESS;
}