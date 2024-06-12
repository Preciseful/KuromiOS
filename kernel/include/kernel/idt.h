//
// Created by developful on 06.06.2024.
//

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_register_t;

typedef struct __attribute__((packed)) {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t isr_high;
} idt_entry_t;

typedef struct __attribute__((packed)) {
    uint8_t rpl: 2;
    uint8_t ti: 1;
    uint16_t index: 13;
} segment_selector_t;

void init_idt();

#endif //IDT_H
