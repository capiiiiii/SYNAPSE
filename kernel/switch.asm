; SYNAPSE SO - Context Switching Assembly Routines
; Licensed under GPLv3

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

; Context switch function
; Parameters:
;   old_proc: pointer to old process structure (in stack)
;   new_proc: pointer to new process structure (in stack)
; Clobbers: eax, ebx, ecx, edx, esi, edi, ebp
global context_switch
context_switch:
    ; Save old process state
    ; old_proc pointer is in [esp+4]
    ; new_proc pointer is in [esp+8]

    ; Push general-purpose registers
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    ; Save stack pointers
    mov eax, [esp+4]      ; old_proc
    mov [eax+0], esp       ; Save ESP (offset 0 in process_t)
    mov ebp, [eax+4]       ; Save EBP

    ; Load new process state
    mov eax, [esp+8]      ; new_proc

    ; Switch page directory if needed
    mov ecx, [eax+16]     ; page_dir pointer
    mov cr3, ecx

    ; Load new stack pointer
    mov esp, [eax+0]
    mov ebp, [eax+4]

    ; Restore general-purpose registers
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    ; Return to new process
    ret

; Initialize context for new process
; Parameters:
;   proc: pointer to process structure
;   entry_point: entry point address
global context_init
context_init:
    ; proc pointer in [esp+4]
    ; entry_point in [esp+8]

    push ebp
    mov ebp, esp

    mov eax, [ebp+8]      ; proc
    mov ecx, [ebp+12]     ; entry_point

    ; Set up initial stack
    mov edx, [eax+8]      ; stack_end
    sub edx, 4

    ; Push entry point as return address
    mov [edx], ecx

    ; Save ESP
    mov [eax+0], edx

    ; Set EIP to entry point
    mov [eax+10], ecx

    ; Set EFLAGS (enable interrupts)
    mov dword [eax+14], 0x202

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
