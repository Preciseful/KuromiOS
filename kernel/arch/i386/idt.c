//
// Created by developful on 06.06.2024.
//

#include <stdbool.h>
#include <kernel/idt.h>
#include <stdint.h>
#include <kernel/debug.h>

#define ENTRIES_COUNT 256
#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F

idt_register_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t entries[ENTRIES_COUNT];

extern void *isr_stub_table[32];
extern void load_idt(idt_register_t *idtr);

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    printf("ass");
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void idt_set_gate(uint8_t num, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &entries[num];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x0008;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void init_idt() {
    idtr.base = (uintptr_t)&entries[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * ENTRIES_COUNT - 1;

    for (uint8_t i = 0; i < 32; i++) {
        idt_set_gate(i, isr_stub_table[i], INTERRUPT_GATE);
    }

    load_idt(&idtr);
}


