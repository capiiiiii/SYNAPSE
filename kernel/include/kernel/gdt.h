/* SPDX-License-Identifier: GPL-3.0-or-later */
/* SYNAPSE SO - Global Descriptor Table Header */

#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

/* GDT segment selectors
 * Format: index << 3 | RPL (Requested Privilege Level)
 * Index 0: NULL descriptor
 * Index 1: Kernel Code Segment (0x08)
 * Index 2: Kernel Data Segment (0x10)
 * Index 3: User Code Segment (0x18)
 * Index 4: User Data Segment (0x20)
 */
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE   0x18
#define GDT_USER_DATA   0x20

/* GDT initialization function */
void gdt_init(void);

#endif /* KERNEL_GDT_H */
