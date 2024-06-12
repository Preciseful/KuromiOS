//
// Created by developful on 04.06.2024.
//

#include <kernel/debug.h>
#include <kernel/gdt.h>

#define ENTRIES_COUNT 4

extern void setGdt(uint16_t, uint32_t);
extern void reloadSegments();
static gdtr_entry entries[ENTRIES_COUNT]; // 8 bytes an entry
TSS_t task_state;

void encode_gdt_entry(gdtr_entry *target, uint32_t base, uint16_t limit, uint8_t access_byte, uint8_t flags) {
    // Check the limit to make sure that it can be encoded
    if (limit > 0xFFFFF) {
        ERROR("GDT cannot encode limits larger than 0xFFFFF.");
    }

    // Encode the limit
    target->limit_1 = limit & 0xFFFF;
    target->limit_2 = (limit >> 16) & 0x0F;

    // Encode the base
    target->base_1 = base & 0xFFFF;
    target->base_2 = (base >> 16) & 0xFF;
    target->base_3 = (base >> 24) & 0xFF;

    gdt_access_byte_t access;
    access.a = (access_byte & 0x01) >> 0;
    access.rw = (access_byte & 0x02) >> 1;
    access.dc = (access_byte & 0x04) >> 2;
    access.e  = (access_byte & 0x08) >> 3;
    access.s  = (access_byte & 0x10) >> 4;
    access.dpl = (access_byte & 0x60) >> 5;
    access.p  = (access_byte & 0x80) >> 7;
    target->access = access;

    // Set the flags struct from a uint8_t variable
    gdt_flags_t flag;
    flag.reserved = (flags & 0x01) >> 0;
    flag.long_mode = (flags & 0x02) >> 1;
    flag.size = (flags & 0x04) >> 2;
    flag.granularity = (flags & 0x08) >> 3;
    target->flags = flag;
}

void init_gdt() {
    encode_gdt_entry(&entries[0x0000], 0, 0, 0, 0); // null
    encode_gdt_entry(&entries[0x0008], 0, sizeof(entries) - 1, 0x9A, 0xC); // kernel code segment
    encode_gdt_entry(&entries[0x0010], 0, sizeof(entries) - 1, 0x92, 0xC); // kernel data segment
    encode_gdt_entry(&entries[0x0018], 0, sizeof(entries) - 1, 0xFA, 0xC); // user code segment
    encode_gdt_entry(&entries[0x0020], 0, sizeof(entries) - 1, 0xF2, 0xC); // user data segment
    encode_gdt_entry(&entries[0x0028], (uint32_t)&task_state, sizeof(TSS_t) - 1, 0x89, 0x0);

    setGdt(sizeof(entries) - 1, 0);
}