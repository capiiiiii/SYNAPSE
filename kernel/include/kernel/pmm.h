/* SYNAPSE SO - Physical Memory Manager */
/* Licensed under GPLv3 */

#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include <stdint.h>

/* Frame size is 4KB (4096 bytes) */
#define FRAME_SIZE 4096

/* Maximum memory size (4GB) */
#define MAX_MEMORY 0xFFFFFFFF

/* Number of frames in 4GB address space */
#define MAX_FRAMES (MAX_MEMORY / FRAME_SIZE)

/* Frame states */
#define FRAME_FREE 0
#define FRAME_USED 1

/* Memory map entry from Multiboot */
typedef struct {
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} __attribute__((packed)) mem_map_entry_t;

/* Multiboot memory map */
typedef struct {
    uint32_t size;
    mem_map_entry_t entries[0];
} __attribute__((packed)) mem_map_t;

/* Initialize PMM with memory map from Multiboot */
void pmm_init(mem_map_t* mmap, uint32_t mmap_size, uint32_t mmap_desc_size);

/* Allocate a physical frame */
uint32_t pmm_alloc_frame(void);

/* Free a physical frame */
void pmm_free_frame(uint32_t frame_addr);

/* Get number of free frames */
uint32_t pmm_get_free_frames(void);

/* Get number of used frames */
uint32_t pmm_get_used_frames(void);

/* Initialize kernel heap (uses physical memory directly) */
void pmm_init_kernel_heap(uint32_t start, uint32_t size);

/* Simple allocation for kernel before paging */
void* pmm_kmalloc(uint32_t size);
void pmm_kfree(void* ptr, uint32_t size);

#endif /* KERNEL_PMM_H */
