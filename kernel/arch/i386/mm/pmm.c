//
// Created by developful on 01.06.2024.
//

#include <kernel/pmm.h>
#include <kernel/debug.h>

#define MEMORY_SIZE 4096    // Size of memory (in bytes)
#define PAGE_SIZE   4096    // Size of each page (in bytes)
#define TOTAL_SIZE  MEMORY_SIZE / PAGE_SIZE

extern uint32_t endkernel;
uint8_t frame_map[TOTAL_SIZE];

void init_pmm() {
    for (int i = 0; i < TOTAL_SIZE; i++)
        frame_map[i] = 0;
}

uint32_t get_frame() {
    uint32_t i = 0;
    while (frame_map[i] != 0x00) {
        i++;
        if (i == TOTAL_SIZE) {
            ERROR("In vmm/get_frame: no available frame found.") AND_RETURN(0);
        }
    }

    frame_map[i] = 0x01;
    return endkernel + i * 0x1000; //return the address of the page frame based on the location declared free
}

void free_frame(uint32_t frame) {
    frame = frame - endkernel;
    if (frame == 0)
        frame_map[frame] = 0x00;
    else {
        uint32_t index = frame / 0x1000;
        frame_map[index] = 0x00;
    }
}
