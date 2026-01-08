/* SYNAPSE SO - Scheduler Implementation */
/* Licensed under GPLv3 */

#include <kernel/scheduler.h>
#include <kernel/process.h>
#include <kernel/vga.h>
#include <kernel/idt.h>

/* Forward declaration for process_list */
extern process_t* process_list;

/* Scheduler quantum */
static uint32_t quantum = DEFAULT_QUANTUM;

/* Initialize scheduler */
void scheduler_init(void) {
    vga_print("[+] Initializing Scheduler...\n");
    quantum = DEFAULT_QUANTUM;

    /* Process initialization is done in process_init() */
    vga_print("    Scheduler ready\n");
}

/* Add process to scheduler */
void scheduler_add_process(process_t* proc) {
    if (proc == 0) {
        return;
    }

    proc->state = PROC_STATE_READY;
    proc->quantum = quantum;

    /* Add to process ready queue */
    if (process_get_current() == 0 || proc->pid != process_get_current()->pid) {
        /* Already handled in process.c */
    }
}

/* Remove process from scheduler */
void scheduler_remove_process(process_t* proc) {
    if (proc == 0) {
        return;
    }

    proc->state = PROC_STATE_STOPPED;

    /* Already handled in process.c */
}

/* Schedule next process (called by timer interrupt) */
void scheduler_tick(void) {
    process_t* current = process_get_current();

    if (current == 0) {
        return;
    }

    /* Decrement quantum */
    current->quantum--;

    /* If quantum expired, schedule next process */
    if (current->quantum == 0) {
        schedule();
    }
}

/* Force schedule */
void schedule(void) {
    process_t* current = process_get_current();
    process_t* next = 0;

    /* Find next ready process */
    if (process_list == 0) {
        return;
    }

    /* Simple round-robin: next in list */
    if (current != 0) {
        next = current->next;

        /* Skip zombie and blocked processes */
        while (next != 0 && (next->state == PROC_STATE_ZOMBIE ||
               next->state == PROC_STATE_BLOCKED)) {
            next = next->next;
            if (next == current) {
                break;
            }
        }
    }

    if (next == 0 || next == current) {
        /* No other process to run */
        return;
    }

    /* Switch to next process */
    if (current != 0) {
        /* Save current state */
        current->state = PROC_STATE_READY;
        current->quantum = quantum;

        /* Perform context switch */
        context_switch(current, next);
    } else {
        /* First process */
        current = next;
        current->state = PROC_STATE_RUNNING;
    }
}

/* Set quantum */
void scheduler_set_quantum(uint32_t q) {
    if (q > 0) {
        quantum = q;
    }
}

/* Get quantum */
uint32_t scheduler_get_quantum(void) {
    return quantum;
}

/* Get number of ready processes */
uint32_t scheduler_get_ready_count(void) {
    uint32_t count = 0;
    process_t* proc = process_list;

    if (proc == 0) {
        return 0;
    }

    process_t* start = proc;
    while (proc != 0) {
        if (proc->state == PROC_STATE_READY || proc->state == PROC_STATE_RUNNING) {
            count++;
        }
        proc = proc->next;
        if (proc == start) {
            break;
        }
    }

    return count;
}
