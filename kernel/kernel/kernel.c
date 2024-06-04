#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/debug.h>

#include <kernel/debug.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>

#include "kernel/pmm.h"

void kernel_main(void) {
	init_terminal();

    // center the title
	for (int i = 0; i < 35; i++)
		printf(" ");
	printf("[KuromiOS]\n\n");

	SUCCESS("Terminal initialized.\n");
	init_vmm();
	init_pmm();
	WARN("VMM and PMM initialized, but function 'map_page' has not been tested.\n");

	heap_t kheap;
	init_kheap(&kheap);
	WARN("Heap initialized, but the start mark might not be good.\n");
}
