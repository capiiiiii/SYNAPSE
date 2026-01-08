/* SYNAPSE SO - Scheduler */
/* Licensed under GPLv3 */

#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include <stdint.h>
#include <kernel/process.h>

/* Scheduler quantum (time slice) */
#define DEFAULT_QUANTUM 10

/* Initialize scheduler */
void scheduler_init(void);

/* Add process to scheduler */
void scheduler_add_process(process_t* proc);

/* Remove process from scheduler */
void scheduler_remove_process(process_t* proc);

/* Schedule next process (called by timer interrupt) */
void scheduler_tick(void);

/* Force schedule */
void schedule(void);

/* Set quantum */
void scheduler_set_quantum(uint32_t quantum);

/* Get quantum */
uint32_t scheduler_get_quantum(void);

/* Get number of ready processes */
uint32_t scheduler_get_ready_count(void);

/* Context switch function (assembly) */
void context_switch(process_t* old_proc, process_t* new_proc);

/* Initialize context for new process */
void context_init(process_t* proc, uint32_t entry_point);

#endif /* KERNEL_SCHEDULER_H */
