#pragma once
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>

EFI_FILE* OpenFile(IN EFI_HANDLE ImageHandle, IN CHAR16* filename);
EFI_FILE_INFO* GetFileInfo(IN EFI_FILE* File);

VOID PrintFileInfo(IN EFI_FILE_INFO* FileInfo);