//
// Created by developful on 01.06.2024.
//

#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void free(void *ptr) {
#if defined(__is_libk)
    kheap_free(ptr);
#else
    // TODO: implement user-space free
#endif
}
