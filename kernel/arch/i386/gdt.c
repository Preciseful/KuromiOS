//
// Created by developful on 04.06.2024.
//

#include <kernel/debug.h>
#include <kernel/gdt.h>

#define ENTRIES_COUNT 4

extern void setGdt(uint16_t, uint32_t);
extern void reloadSegments();
uint8_t entries[ENTRIES_COUNT * 8]; // 8 bytes an entry
TSS_t task_state;

void encode_gdt_entry(uint8_t *target, uint32_t base, uint16_t limit, uint8_t access_byte, uint8_t flags) {
    // Check the limit to make sure that it can be encoded
    if (limit > 0xFFFFF) {
        ERROR("GDT cannot encode limits larger than 0xFFFFF.");
    }

    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] = (limit >> 16) & 0x0F;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = access_byte;

    // Encode the flags
    target[6] |= (flags << 4);
}

void init_gdt() {
    encode_gdt_entry(&entries[0], 0, 0, 0, 0); // null
    encode_gdt_entry(&entries[8], 0, 0xFFFFF, 0x9A, 0xC); // kernel code segment
    encode_gdt_entry(&entries[16], 0, 0xFFFFF, 0x92, 0xC); // kernel data segment
    encode_gdt_entry(&entries[24], 0, 0xFFFFF, 0xFA, 0xC); // user code segment
    encode_gdt_entry(&entries[32], 0, 0xFFFFF, 0xF2, 0xC); // user data segment
    encode_gdt_entry(&entries[40], &task_state, sizeof(TSS_t) - 1, 0x89, 0x0);

    setGdt(sizeof(entries) - 1, 0);
}