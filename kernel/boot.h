//
// Created by Lazurin Eugene on 10.05.2021.
//

#ifndef MEMOS_BOOT_H
#define MEMOS_BOOT_H

enum boot_memmap_entry_type_t {
    USABLE      = 1,
    RESERVED    = 2,
    ACPIRECLAIM = 3,
    ACPINVS     = 4
};

struct boot_memmap_entry_t {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t unused;
} __attribute__((packed));

struct boot_memmap_t {
    struct boot_memmap_entry_t *address;
    uint64_t entries;
} __attribute__((packed));

struct boot_framebuffer_t {
    uint64_t address;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint16_t bpp;
} __attribute__((packed));

struct boot_module_t {
    uint64_t  begin;
    uint64_t  end;
    char      name[128];
    uint64_t  next;
} __attribute__((packed));

struct boot_struct_t {
    char* cmdline;
    struct boot_memmap_t memmap;
    struct boot_framebuffer_t fb;
    uint64_t rsdp;
    uint64_t module_count;
    struct boot_module_t module;
    uint64_t epoch;
} __attribute__((packed));


#endif //MEMOS_BOOT_H
