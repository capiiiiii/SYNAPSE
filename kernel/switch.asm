; SYNAPSE SO - Context Switching Assembly Routines
; Licensed under GPLv3

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

%define KERNEL_VIRT_START 0xC0000000

; Offsets into process_t (kernel/include/kernel/process.h)
%define PROC_PAGE_DIR   52
%define PROC_STACK_END  68
%define PROC_ESP        72
%define PROC_EBP        76
%define PROC_EIP        80
%define PROC_EFLAGS     84

; Context switch function
; Parameters:
;   old_proc: pointer to old process structure
;   new_proc: pointer to new process structure
global context_switch
context_switch:
    mov eax, [esp+4]          ; old_proc
    mov edx, [esp+8]          ; new_proc

    ; Save current stack pointers
    mov [eax+PROC_ESP], esp
    mov [eax+PROC_EBP], ebp

    ; Switch page directory (CR3) if provided
    mov ecx, [edx+PROC_PAGE_DIR]
    test ecx, ecx
    jz .skip_cr3
    sub ecx, KERNEL_VIRT_START
    mov cr3, ecx
.skip_cr3:

    ; Load new stack pointers
    mov esp, [edx+PROC_ESP]
    mov ebp, [edx+PROC_EBP]

    ret

; Initialize context for new process
; Parameters:
;   proc: pointer to process structure
;   entry_point: entry point address
global context_init
context_init:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]          ; proc
    mov ecx, [ebp+12]         ; entry_point

    mov edx, [eax+PROC_STACK_END]
    sub edx, 4

    ; Push entry point as return address
    mov [edx], ecx

    mov [eax+PROC_ESP], edx
    mov [eax+PROC_EBP], edx
    mov [eax+PROC_EIP], ecx
    mov dword [eax+PROC_EFLAGS], 0x202

    leave
    ret

; Get current stack pointer
global get_esp
get_esp:
    mov eax, esp
    ret

; Get current base pointer
global get_ebp
get_ebp:
    mov eax, ebp
    ret
