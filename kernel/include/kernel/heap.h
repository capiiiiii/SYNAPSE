/* SYNAPSE SO - Kernel Heap Manager */
/* Licensed under GPLv3 */

#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <stdint.h>

/* Heap block header */
typedef struct heap_block {
    uint32_t size;
    uint32_t magic;
    struct heap_block* next;
    struct heap_block* prev;
    uint32_t is_free;
} heap_block_t;

/* Magic number for heap blocks */
#define HEAP_MAGIC 0xDEADBEEF

/* Heap alignment */
#define HEAP_ALIGN 16

/* Initialize kernel heap */
void heap_init(void* start, uint32_t size);

/* Allocate memory */
void* kmalloc(uint32_t size);

/* Free memory */
void kfree(void* ptr);

/* Reallocate memory */
void* krealloc(void* ptr, uint32_t size);

/* Get heap statistics */
uint32_t heap_get_total_size(void);
uint32_t heap_get_used_size(void);
uint32_t heap_get_free_size(void);

#endif /* KERNEL_HEAP_H */
