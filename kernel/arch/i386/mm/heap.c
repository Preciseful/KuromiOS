//
// Created by developful on 04.06.2024.
//

#include <kernel/heap.h>
#include <kernel/debug.h>

heap_t *kheap;

static uint8_t get_nid(uint8_t a, uint8_t b) {
    uint8_t c;
    for (c = a + 1; c == b || c == 0; ++c) { }
    return c;
}

void kheap_add_block(uintptr_t addr, uint32_t size, uint32_t block_size) {
    heap_block_t *block;
    uint32_t i;

    block = (heap_block_t*)addr;
    block->size = size - sizeof(heap_block_t);
    block->block_size = block_size;

    block->next = kheap->first_block;
    kheap->first_block = block;

    uint32_t block_count = block->size / block->block_size;
    uint8_t *bitmap = (uint8_t *) &block[1];

    // clear bitmap
    for (i = 0; i < block_count; ++i) {
        bitmap[i] = 0;
    }

    // reserve room for bitmap
    block_count = (block_count / block_size) * block_size < block_count ? block_count / block_size + 1 : block_count / block_size;
    for (i = 0; i < block_count; ++i) {
        bitmap[i] = 5;
    }

    block->last_found_block = block_count - 1;
    block->used = block_count;
}


void *kheap_malloc(uint32_t size) {
    heap_block_t *block;

    // iterate blocks
    for (block = kheap->first_block; block; block = block->next) {
        // check if block has enough room
        if (block->size - (block->used * block->block_size) >= size) {
            uint32_t block_count = block->size / block->block_size;
            uint32_t size_needed = (size / block->block_size) * block->block_size < size ? size / block->block_size + 1 : size / block->block_size;
            uint8_t *bitmap = (uint8_t *) &block[1];

            for (uint32_t i = (block->last_found_block + 1 >= block_count ? 0 : block->last_found_block + 1); i < block->last_found_block; i++) {
                // just wrap around
                if (i >= block_count) {
                    i = 0;
                }

                if (bitmap[i] > 0) {
                    continue;
                }

                // count free blocks
                uint32_t free_blocks = 0;
                while (bitmap[i + free_blocks] == 0 && free_blocks < size_needed && i + free_blocks < block_count)
                    free_blocks++;

                // we have enough, now allocate them
                if (free_blocks == size_needed) {
                    // find ID that does not match left or right
                    uint8_t nid = get_nid(bitmap[i - 1], bitmap[i + free_blocks]);

                    // allocate by setting id
                    for (uint32_t k = 0; k < free_blocks; ++k) {
                        bitmap[i + k] = nid;
                    }

                    // optimization
                    block->last_found_block = i + size_needed - 2;

                    // count used blocks NOT bytes
                    block->used += free_blocks;

                    return (void*)(i * block->block_size + (uintptr_t)&block[1]);
                }

                i += free_blocks - 1;
            }
        }
    }

    ERROR("Malloc failed!");
    return 0;
}

void kheap_free(void *ptr) {
    heap_block_t *block;
    uint32_t i;

    for (block = kheap->first_block; block; block = block->next) {
        if ((uintptr_t)ptr > (uintptr_t)block && (uintptr_t)ptr < (uintptr_t)block + sizeof(heap_block_t) + block->size) {
            // found block
            uintptr_t ptr_offset = (uintptr_t) ptr - (uintptr_t) &block[1];
            uint32_t block_index = ptr_offset / block->block_size;
            uint8_t *bitmap = (uint8_t *) &block[1];

            // clear allocation
            uint8_t id = bitmap[block_index];
            uint32_t max = block->size / block->block_size;
            for (i = block_index; bitmap[i] == id && i < max; ++i) {
                bitmap[i] = 0;
            }

            block->used -= i - block_index;
            return;
        }
    }

    ERROR("Freeing failed!");
}

void init_kheap(heap_t *heap) {
    kheap = heap;
    kheap->first_block = 0;
    kheap_add_block(0x200000, 0x100000, 16);  /* add block to heap
                                                       (starting 1MB mark and length of 1MB)
                                                       with default block size of 16 bytes
                                                       */
}