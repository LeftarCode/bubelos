#include "ELF.h"
#define MAX_PH_COUNT 0x10
#define PAGE_SIZE 4096

int GetPagesCount(int size) {
  float pages = (float)size / 4096;
  int inum = (int)pages;
  if (pages == (float)inum) {
      return inum;
  }
  return inum + 1;
}

void ZeroMemory(void* dst, uint64_t size) {
  gBS->SetMem(dst, size, 0);
}

BOOLEAN VerifyELFSignature(unsigned char ei_mag[4]) {
  return ei_mag[0] == 0x7F && ei_mag[1] == 'E' && ei_mag[2] == 'L' && ei_mag[3] == 'F';
}

struct ELFProgramHeader* LoadProgramHeader(IN EFI_FILE* ElfFile, uint64_t offset) {
  EFI_STATUS Status;
  UINTN ELFProgramHeaderSize = sizeof(struct ELFProgramHeader);

  Status = ElfFile->SetPosition(ElfFile, offset);
  if(EFI_ERROR(Status)) {
    Print(L"Could not set ELF file position. 0x%llx\r\n", Status);
    return NULL;
  }

  struct ELFProgramHeader* ProgramHeader = (struct ELFProgramHeader*)AllocatePool(ELFProgramHeaderSize);
  if (ProgramHeader == NULL) {
    Print(L"Error allocating ELF program header buffer!\n");
    return NULL;
  }

  Status = ElfFile->Read(ElfFile, &ELFProgramHeaderSize, (VOID*)ProgramHeader);
  if(EFI_ERROR(Status)) {
    Print(L"Could not read ELF program header! 0x%llx\r\n", Status);
    return NULL;
  }

  return ProgramHeader;
}

BOOLEAN LoadSectionToMemory(IN EFI_FILE* ElfFile, struct ELFProgramHeader* ElfProgramHeader) {
  EFI_STATUS Status;

  Status = ElfFile->SetPosition(ElfFile, ElfProgramHeader->p_offset);
  if(EFI_ERROR(Status)) {
    Print(L"Could not set ELF file position. 0x%llx\r\n", Status);
    return FALSE;
  }

  // TODO: Validate p_memsz and p_filesz

  uint64_t PagesCount = GetPagesCount(ElfProgramHeader->p_memsz);
  // TODO: Allocate different memory type depending on p_flags - EfiLoaderCode, EfiLoaderData
  Status = gBS->AllocatePages(AllocateAddress, EfiLoaderCode, PagesCount, (EFI_PHYSICAL_ADDRESS*)&ElfProgramHeader->p_vaddr);
  if (EFI_ERROR(Status)) {
    Print(L"Error allocating program section buffer! 0x%llx\r\n", Status);
    return FALSE;
  }
  ZeroMemory((void*)ElfProgramHeader->p_vaddr, ElfProgramHeader->p_memsz);

  UINTN SizeToLoadFromFile = ElfProgramHeader->p_filesz;
  Status = ElfFile->Read(ElfFile, &SizeToLoadFromFile, (VOID*)ElfProgramHeader->p_vaddr);
  if(EFI_ERROR(Status)) {
    Print(L"Could not read program section! 0x%llx\r\n", Status);
    return FALSE;
  }

  Print(L"vaddr: 0x%llx, p_memsz: 0x%llx, p_filesz: 0x%llx, pages: 0x%llx\n", ElfProgramHeader->p_vaddr, ElfProgramHeader->p_memsz, ElfProgramHeader->p_filesz, PagesCount);
  return TRUE;
}

VOID LoadELF(IN EFI_FILE* ElfFile, OUT VOID** ElfEntryFunction) {
  EFI_STATUS Status;
  UINTN ElfHeaderSize = sizeof(struct ELFHeader);

  Status = ElfFile->SetPosition(ElfFile, 0);
  if(EFI_ERROR(Status)) {
    Print(L"Could not set ELF file position. 0x%llx\r\n", Status);
    return;
  }

  struct ELFHeader* ElfHeader = (struct ELFHeader*)AllocatePool(ElfHeaderSize);
  if (ElfHeader == NULL) {
    Print(L"Error allocating ELF header buffer!\n");
    return;
  }

  Status = ElfFile->Read(ElfFile, &ElfHeaderSize, (VOID*)ElfHeader);
  if(EFI_ERROR(Status)) {
    Print(L"Could not read ELF header! 0x%llx\r\n", Status);
    return;
  }
  
  if(!VerifyELFSignature(ElfHeader->ei_mag)) {
    Print(L"Invalid ELF signature!\r\n");
    return;
  }

  if(ElfHeader->e_machine != 0x3E /*AMD x86-64*/) {
    Print(L"Executable must be compiled for AMD x86-64!\r\n");
    return;
  }

  // VALID ELF from now - I guess
  *ElfEntryFunction = (VOID*)ElfHeader->e_entry;

  uint64_t phOffset = ElfHeader->e_phoff;
  for (uint16_t i = 0; i < ElfHeader->e_phnum; i++) {
    struct ELFProgramHeader* ProgramHeader = LoadProgramHeader(ElfFile, phOffset);
    phOffset += sizeof(struct ELFProgramHeader);
    if (ProgramHeader->p_type != 0x01) {
      continue;
    }

    // Assume that sections are aligned to 4kB (at least offsets)
    if(!LoadSectionToMemory(ElfFile, ProgramHeader)) {
      Print(L"Could not load ELF section. Abort!");
      FreePool(ProgramHeader);
      return;
    }

    FreePool(ProgramHeader);
  }
}