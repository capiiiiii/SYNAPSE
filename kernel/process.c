/* SYNAPSE SO - Process Management Implementation */
/* Licensed under GPLv3 */

#include <kernel/process.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <kernel/vga.h>
#include <kernel/string.h>
#include <kernel/pmm.h>

/* Process list */
process_t* process_list = 0;
static process_t* current_process = 0;

/* Next PID to assign */
static pid_t next_pid = 1;

/* Scheduler */
static process_t* ready_queue = 0;
static uint32_t time_slice = 10; /* Time slice in ticks */

/* Initialize process management */
void process_init(void) {
    vga_print("[+] Initializing Process Management...\n");
    process_list = 0;
    current_process = 0;
    ready_queue = 0;
}

/* Create a new process */
process_t* process_create(const char* name, uint32_t flags) {
    /* Allocate process structure */
    process_t* proc = (process_t*)kmalloc(sizeof(process_t));
    if (proc == 0) {
        return 0;
    }

    /* Initialize fields */
    proc->pid = next_pid++;
    proc->ppid = (current_process != 0) ? current_process->pid : 0;
    proc->state = PROC_STATE_READY;
    proc->flags = flags;
    proc->priority = 10; /* Default priority */
    proc->quantum = time_slice;
    proc->exit_code = 0;

    /* Set name */
    if (name != 0) {
        strncpy(proc->name, name, 31);
        proc->name[31] = '\0';
    } else {
        strcpy(proc->name, "unknown");
    }

    /* Create page directory */
    if (flags & PROC_FLAG_KERNEL) {
        /* Kernel process - use kernel page directory */
        proc->page_dir = vmm_get_current_directory();
    } else {
        /* User process - create new page directory */
        proc->page_dir = vmm_create_page_directory();
    }

    /* Allocate stack (8KB for kernel, 4KB for user) */
    uint32_t stack_size = (flags & PROC_FLAG_KERNEL) ? 0x2000 : 0x1000;
    uint32_t stack_phys = pmm_alloc_frame();

    if (flags & PROC_FLAG_KERNEL) {
        /* Kernel stack */
        proc->stack_start = (uint32_t)kmalloc(stack_size);
        proc->stack_end = proc->stack_start + stack_size;
    } else {
        /* User stack - map to user space */
        uint32_t stack_virt = 0x7FFFF000; /* Just below 2GB */
        vmm_map_page(stack_virt, stack_phys, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
        proc->stack_start = stack_virt - stack_size;
        proc->stack_end = stack_virt;
    }

    /* Initialize context */
    proc->esp = proc->stack_end;
    proc->ebp = proc->stack_end;
    proc->eip = (flags & PROC_FLAG_KERNEL) ? (uint32_t)idle_process : 0;
    proc->eflags = 0x202; /* Enable interrupts */

    /* Clear registers */
    proc->eax = proc->ebx = proc->ecx = proc->edx = 0;
    proc->esi = proc->edi = 0;

    /* Add to process list */
    if (process_list == 0) {
        process_list = proc;
        proc->next = proc;
        proc->prev = proc;
    } else {
        proc->next = process_list;
        proc->prev = process_list->prev;
        process_list->prev->next = proc;
        process_list->prev = proc;
    }

    vga_print("    Created process: ");
    vga_print(proc->name);
    vga_print(" (PID: ");
    vga_print_dec(proc->pid);
    vga_print(")\n");

    return proc;
}

/* Destroy a process */
void process_destroy(process_t* proc) {
    if (proc == 0) {
        return;
    }

    /* Remove from process list */
    if (proc->next == proc) {
        process_list = 0;
    } else {
        proc->next->prev = proc->prev;
        proc->prev->next = proc->next;
        if (process_list == proc) {
            process_list = proc->next;
        }
    }

    /* Remove from ready queue */
    scheduler_remove_process(proc);

    /* Free memory */
    kfree((void*)proc->heap_start);
    kfree(proc);
}

/* Get current process */
process_t* process_get_current(void) {
    return current_process;
}

/* Get process list */
process_t* process_get_list(void) {
    return process_list;
}

/* Find process by PID */
process_t* process_find_by_pid(pid_t pid) {
    process_t* proc = process_list;

    do {
        if (proc->pid == pid) {
            return proc;
        }
        proc = proc->next;
    } while (proc != process_list);

    return 0;
}

/* Set process state */
void process_set_state(process_t* proc, uint32_t state) {
    if (proc != 0) {
        proc->state = state;
    }
}

/* Make process ready */
void process_ready(process_t* proc) {
    if (proc != 0) {
        proc->state = PROC_STATE_READY;
        scheduler_add_process(proc);
    }
}

/* Block a process */
void process_block(process_t* proc) {
    if (proc != 0) {
        proc->state = PROC_STATE_BLOCKED;
        scheduler_remove_process(proc);
    }
}

/* Unblock a process */
void process_unblock(process_t* proc) {
    if (proc != 0) {
        proc->state = PROC_STATE_READY;
        scheduler_add_process(proc);
    }
}

/* Execute an ELF binary */
int process_exec(uint8_t* elf_data, uint32_t size) {
    /* TODO: Implement ELF loader */
    (void)elf_data;
    (void)size;
    vga_print("[-] ELF loader not yet implemented\n");
    return -1;
}

/* Exit current process */
void process_exit(int exit_code) {
    if (current_process != 0) {
        current_process->state = PROC_STATE_ZOMBIE;
        current_process->exit_code = exit_code;

        vga_print("Process exited: ");
        vga_print(current_process->name);
        vga_print(" (PID: ");
        vga_print_dec(current_process->pid);
        vga_print(", exit code: ");
        vga_print_dec(exit_code);
        vga_print(")\n");

        /* Schedule next process */
        schedule();

        /* Destroy process */
        process_destroy(current_process);
    }
}

/* Get current PID */
pid_t process_get_pid(void) {
    return (current_process != 0) ? current_process->pid : 0;
}

/* Get parent PID */
pid_t process_get_ppid(void) {
    return (current_process != 0) ? current_process->ppid : 0;
}

/* Set process name */
void process_set_name(process_t* proc, const char* name) {
    if (proc != 0 && name != 0) {
        strncpy(proc->name, name, 31);
        proc->name[31] = '\0';
    }
}

/* Idle process */
void idle_process(void) {
    while (1) {
        __asm__ volatile("hlt");
    }
}
