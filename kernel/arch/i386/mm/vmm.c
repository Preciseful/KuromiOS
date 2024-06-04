//
// Created by developful on 01.06.2024.
//

#include <stdint.h>
#include <kernel/vmm.h>

#include <stddef.h>
#include <kernel/debug.h>
#include <kernel/pmm.h>

#define ENTRIES 1024
#define PAGE_SIZE 4096
#define PAGE_MASK (~(PAGE_SIZE - 1))

uint32_t directory[ENTRIES] __attribute__((aligned(4096)));
uint32_t first_table[ENTRIES] __attribute__((aligned(4096)));

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void init_vmm() {
    for (int i = 0; i < 1024; i++) {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        directory[i] = 0x00000002;
    }

    for (int i = 0; i < 1024; i++) {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes (supervisor level, read/write, present)
        first_table[i] = (i * 0x1000) | 3;
    }

    // attributes: supervisor level, read/write, present
    directory[0] = (unsigned int)first_table | 3;

    loadPageDirectory(directory);
    enablePaging();
}


void *get_physaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    if (!(directory[pdindex] & 0x01)) {
        ERROR("In vmm/get_physaddr: directory[pdindex] is not present.") AND_RETURN(NULL);
    }

    unsigned long *pt = (unsigned long *)directory + 0x400 * pdindex;
    if (pt == NULL || !(pt[ptindex] & 0x01)) {
        ERROR("In vmm/get_physaddr: pt does not exist or pt[ptindex] is not present.") AND_RETURN(NULL);
    }

    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
    if (((unsigned long)physaddr & PAGE_MASK) != (unsigned long)physaddr ||
        ((unsigned long)virtualaddr & PAGE_MASK) != (unsigned long)virtualaddr) {
        ERROR("In vmm/map_page: attempted to map with non-page-aligned addresses.");
    }

    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    // Page directory entry isn't present -> page table doesn't exist
    if (!(directory[pdindex] & 0x01)) {
        // You need to create a new empty PT and adjust the PDE accordingly.

        unsigned long* new_pt = (unsigned long*)get_frame();  // allocate a new page
        if (new_pt == NULL) {   // make sure the allocation was successful
            ERROR("In vmm/map_page: declaring the new page table wasn't successful.");
        }

        for (int i = 0; i < 1024; i++) {
            // As the address is page aligned, it will always leave 12 bits zeroed.
            // Those bits are used by the attributes (supervisor level, read/write, present)
            new_pt[i] = (i * 0x1000) | 3;
        }

        // attributes: supervisor level, read/write, present
        directory[pdindex] = (unsigned int)first_table | 3;
    }

    unsigned long *pt = (unsigned long *)directory + 0x400 * pdindex;
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?

    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present

    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}