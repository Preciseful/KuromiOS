//
// Created by developful on 04.06.2024.
//

#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct heap_block {
    struct heap_block *next;
    uint32_t size;
    uint32_t used;
    uint32_t block_size;
    uint32_t last_found_block;
} heap_block_t;

typedef struct heap {
    heap_block_t *first_block;
} heap_t;

void init_kheap(heap_t *heap);
void *kheap_malloc(uint32_t size);
void kheap_free(void *ptr);


#endif //HEAP_H
