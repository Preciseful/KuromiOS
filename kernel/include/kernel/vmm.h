#ifndef VMM_H
#define VMM_H

void init_vmm();
void *get_physaddr(void *virtualaddr);
void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

#endif
