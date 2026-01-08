/* SYNAPSE SO - ELF Loader */
/* Licensed under GPLv3 */

#ifndef KERNEL_ELF_H
#define KERNEL_ELF_H

#include <stdint.h>

/* Forward declaration */
typedef struct process process_t;

/* ELF identification */
#define EI_NIDENT 16

/* ELF magic number */
#define ELFMAG "\177ELF"
#define SELFMAG 4

/* ELF classes */
#define ELFCLASS32 1
#define ELFCLASS64 2

/* ELF data encoding */
#define ELFDATA2LSB 1  /* Little endian */
#define ELFDATA2MSB 2  /* Big endian */

/* ELF version */
#define EV_CURRENT 1

/* ELF machine types */
#define EM_386 3

/* ELF file types */
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

/* ELF 32-bit header */
typedef struct {
    uint8_t e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed)) elf32_header_t;

/* ELF 32-bit program header */
typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} __attribute__((packed)) elf32_phdr_t;

/* ELF 32-bit section header */
typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} __attribute__((packed)) elf32_shdr_t;

/* Program header types */
#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6

/* Program header flags */
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

/* Section header types */
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_DYNSYM 11

/* Section header flags */
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4

/* ELF loader functions */
int elf_check_header(elf32_header_t* header);
int elf_load(uint8_t* elf_data, uint32_t size, uint32_t* entry_point);
int elf_load_to_process(uint8_t* elf_data, uint32_t size, process_t* proc);

#endif /* KERNEL_ELF_H */
