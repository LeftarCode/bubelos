#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>
#include "FileSystem.h"

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

EFI_STATUS EFIAPI EntryPoint(IN EFI_HANDLE ImageHandle,
                             IN EFI_SYSTEM_TABLE* sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;
  OpenFile();
  // EFI_STATUS                  Status;
  // EFI_MEMORY_DESCRIPTOR       *EfiMemoryMap;
  // UINTN                       EfiMemoryMapSize;
  // UINTN                       EfiMapKey;
  // UINTN                       EfiDescriptorSize;
  // UINT32                      EfiDescriptorVersion;
 
  // //
  // // Get the EFI memory map.
  // //
  // EfiMemoryMapSize  = 0;
  // EfiMemoryMap      = NULL;
  // Status = gBS->GetMemoryMap (
  //                 &EfiMemoryMapSize,
  //                 EfiMemoryMap,
  //                 &EfiMapKey,
  //                 &EfiDescriptorSize,
  //                 &EfiDescriptorVersion
  //                 );
  // if (Status != EFI_BUFFER_TOO_SMALL) {
  //   Print(L"Fatal Error: Error getting memory map size!\n");
  //   return Status;
  // }
 
  // //
  // // Use size returned for the AllocatePool.
  // //
  // EfiMemoryMap = (EFI_MEMORY_DESCRIPTOR *) AllocatePool (EfiMemoryMapSize + 2 * EfiDescriptorSize);
  // if (EfiMemoryMap == NULL) {
  //   Print(L"Fatal Error: Error allocating memory map buffer!\n");
  //   return Status;
  // }
  // Status = gBS->GetMemoryMap (
  //                 &EfiMemoryMapSize,
  //                 EfiMemoryMap,
  //                 &EfiMapKey,
  //                 &EfiDescriptorSize,
  //                 &EfiDescriptorVersion
  //                 );

  // if (EFI_ERROR (Status)) {
  //   FreePool(EfiMemoryMap);
  // }

  EFI_STATUS status;

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

  EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
  EFI_GUID SimpleFileSystemProtocol = SIMPLE_FILE_SYSTEM_PROTOCOL;
  EFI_GUID FileInfo = EFI_FILE_INFO_ID;


  status = gBS->OpenProtocol(ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if(EFI_ERROR(status)) {
    Print(L"LoadedImage OpenProtocol error. 0x%llx\r\n", status);
    return status;
  }

  status = gBS->OpenProtocol(LoadedImage->DeviceHandle, &SimpleFileSystemProtocol, (void**)&FileSystem, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if(EFI_ERROR(status)) {
    Print(L"FileSystem OpenProtocol error. 0x%llx\r\n", status);
    return status;
  }

  
  EFI_FILE *CurrentDriveRoot;
  status = FileSystem->OpenVolume(FileSystem, &CurrentDriveRoot);
  if(EFI_ERROR(status)) {
    Print(L"OpenVolume error. 0x%llx\r\n", status);
    return status;
  }
  
  CHAR16 KERNEL_PATH[24] = L"\\EFI\\BUBELOS\\kernel.elf";
  EFI_FILE* KernelFile;

  status = CurrentDriveRoot->Open(CurrentDriveRoot, &KernelFile, KERNEL_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (EFI_ERROR(status)) {
    Print(L"Could not read kernel file");
    return status;
  }

  UINTN KernelFileInfoSize;
  status = KernelFile->GetInfo(KernelFile, &FileInfo, &KernelFileInfoSize, NULL);
  Print(L"KernelFileInfoSize: %llu Bytes\r\n", KernelFileInfoSize);

  EFI_FILE_INFO* KernelFileInfo = (EFI_FILE_INFO *)AllocatePool(KernelFileInfoSize);
  if (KernelFileInfo == NULL) {
    Print(L"Error allocating Kernel File Info buffer!\n");
    return EFI_BUFFER_TOO_SMALL;
  }

  status = KernelFile->GetInfo(KernelFile, &FileInfo, &KernelFileInfoSize, KernelFileInfo);
  if (EFI_ERROR(status)) {
    Print(L"Could not read kernel file info");
    return status;
  }

  Print(L"FileName: %s\r\n", KernelFileInfo->FileName);
  Print(L"Size: %llu\r\n", KernelFileInfo->Size);
  Print(L"FileSize: %llu\r\n", KernelFileInfo->FileSize);
  Print(L"PhysicalSize: %llu\r\n", KernelFileInfo->PhysicalSize);
  Print(L"Attribute: %llx\r\n", KernelFileInfo->Attribute);


  for(;;){}

  return EFI_SUCCESS;
}