#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void malloc(uint32_t size) {
#if defined(__is_libk)
    kheap_malloc(size);
#else
    // TODO: implement user-space malloc
#endif
}
