#include "FileSystem.h"

EFI_FILE* OpenFile(IN EFI_HANDLE ImageHandle, IN CHAR16* filename) {
  EFI_STATUS status;
  EFI_FILE* File = NULL;

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

  EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
  EFI_GUID SimpleFileSystemProtocol = SIMPLE_FILE_SYSTEM_PROTOCOL;

  status = gBS->OpenProtocol(ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if(EFI_ERROR(status)) {
    Print(L"LoadedImage OpenProtocol error. 0x%llx\r\n", status);
    return NULL;
  }

  status = gBS->OpenProtocol(LoadedImage->DeviceHandle, &SimpleFileSystemProtocol, (void**)&FileSystem, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if(EFI_ERROR(status)) {
    Print(L"FileSystem OpenProtocol error. 0x%llx\r\n", status);
    return NULL;
  }

  
  EFI_FILE* CurrentDriveRoot;
  status = FileSystem->OpenVolume(FileSystem, &CurrentDriveRoot);
  if(EFI_ERROR(status)) {
    Print(L"OpenVolume error. 0x%llx\r\n", status);
    return NULL;
  }
  

  status = CurrentDriveRoot->Open(CurrentDriveRoot, &File, filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (EFI_ERROR(status)) {
    Print(L"Could not read %s", filename);
    return NULL;
  }
  
  return File;
}

EFI_FILE_INFO* GetFileInfo(EFI_FILE* File) {
  EFI_STATUS status;
  EFI_GUID FileInfoGUID = EFI_FILE_INFO_ID;

  UINTN FileInfoSize;
  status = File->GetInfo(File, &FileInfoGUID, &FileInfoSize, NULL);

  EFI_FILE_INFO* FileInfo = (EFI_FILE_INFO*)AllocatePool(FileInfoSize);
  if (FileInfo == NULL) {
    Print(L"Error allocating File Info buffer!\n");
    return NULL;
  }

  status = File->GetInfo(File, &FileInfoGUID, &FileInfoSize, FileInfo);
  if (EFI_ERROR(status)) {
    Print(L"Could not read file info");
    return NULL;
  }

  return FileInfo;
}

VOID PrintFileInfo(IN EFI_FILE_INFO* FileInfo) {
  Print(L"FileName: %s\r\n", FileInfo->FileName);
  Print(L"Size: %llu\r\n", FileInfo->Size);
  Print(L"FileSize: %llu\r\n", FileInfo->FileSize);
  Print(L"PhysicalSize: %llu\r\n", FileInfo->PhysicalSize);
  Print(L"Attribute: %llx\r\n", FileInfo->Attribute);
}