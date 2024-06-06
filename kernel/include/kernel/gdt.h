//
// Created by developful on 04.06.2024.
//

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t a: 1;
    uint8_t rw: 1;
    uint8_t dc: 1;
    uint8_t e: 1;
    uint8_t s: 1;
    uint8_t dpl: 2;
    uint8_t p: 1;
} access_byte_t;

typedef struct __attribute__((packed)) {
    uint8_t reserved: 1;
    uint8_t l: 1;
    uint8_t db: 1;
    uint8_t g: 1;
} flags_t;

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint16_t fbase;
    uint8_t sbase;
    access_byte_t access;
    uint8_t second_limit: 4;
    flags_t flags;
    uint8_t tbase;
} system_segment_descriptor_t;

typedef struct __attribute__((packed)) {
    uint32_t link;           // This segment's link to previous task, if nested
    uint32_t esp0;           // The stack pointer to load when changing privilege levels
    uint32_t ss0;            // The stack segment to load when changing privilege levels
    uint32_t esp1;           // Unused...
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;            // Page directory base register
    uint32_t eip;            // Instruction pointer
    uint32_t eflags;         // Flags register
    uint32_t eax;            // General purpose registers
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;             // Segment selectors
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;            // Local descriptor table segment selector
    uint16_t trap;           // Trap on task switch - if not 0, causes a debug trap on switch to this task
    uint16_t iomap_base;     // The offset in the TSS to the I/O bitmap, from the end of the TSS
} TSS_t;

void init_gdt();

#endif //GDT_H
