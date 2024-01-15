#pragma once
#include <stdint.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Uefi.h>

#pragma pack(1)
struct ELFHeader {
    unsigned char ei_mag[4];
    unsigned char ei_class;
    unsigned char ei_data;
    unsigned char ei_version;
    unsigned char ei_osabi;
    unsigned char ei_abiversion;
    unsigned char ei_pad[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct ELFProgramHeader {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};
#pragma pack()

BOOLEAN VerifyELFSignature(unsigned char ei_mag[4]);
struct ELFProgramHeader* LoadProgramHeader(IN EFI_FILE* ElfFile, uint64_t offset);
BOOLEAN LoadSectionToMemory(IN EFI_FILE* ElfFile, struct ELFProgramHeader* ElfProgramHeader);
VOID LoadELF(IN EFI_FILE* ElfFile, OUT VOID** ElfEntryFunction);