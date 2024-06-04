//
// Created by developful on 01.06.2024.
//

#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void init_pmm();
void free_frame(uint32_t frame);
uint32_t get_frame();

#endif //PMM_H
