/* SPDX-License-Identifier: GPL-3.0-or-later */
/* SYNAPSE SO - Global Descriptor Table Header */

#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H


/* GDT initialization function */
void gdt_init(void);

/* Segment selectors */
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE   0x1B
#define GDT_USER_DATA   0x23

#endif /* KERNEL_GDT_H */
