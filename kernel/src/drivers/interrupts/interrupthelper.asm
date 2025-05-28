; src/interrupts/interrupthelper.asm

; Specify 64-bit mode for NASM/YASM
bits 64

; Export symbols for linking with C code
global load_idt
global irq0
global irq1

; Declare external C functions (Interrupt Service Routines)
extern isr0
extern isr1

; ----------------------------------------------------------------------------
; load_idt: Loads the Interrupt Descriptor Table Register (IDTR)
;
; This function takes a pointer to an IDTR structure as its argument.
; In the System V AMD64 ABI (common for Linux/Unix-like kernels, which Limine
; often aligns with), the first argument is passed in RDI.
;
; Arguments:
;   RDI: Pointer to the IDTR structure (limit: 2 bytes, base: 8 bytes)
; ----------------------------------------------------------------------------
load_idt:
    ; RDI already contains the address of the IDTR structure
    lidt [rdi]  ; Load IDT register from the address in RDI
    sti         ; Enable interrupts (Set Interrupt Flag)
    ret         ; Return from function

; ----------------------------------------------------------------------------
; Macros for saving and restoring general-purpose registers
;
; In 64-bit mode, 'pusha' and 'popa' are not available. We must manually
; push/pop each 64-bit general-purpose register. RSP (stack pointer)
; is handled implicitly by the CPU when an interrupt occurs.
; ----------------------------------------------------------------------------

%macro SAVE_REGS 0
    push rax    ; Push RAX
    push rcx    ; Push RCX
    push rdx    ; Push RDX
    push rsi    ; Push RSI
    push rdi    ; Push RDI
    push rbp    ; Push RBP
    push r8     ; Push R8
    push r9     ; Push R9
    push r10    ; Push R10
    push r11    ; Push R11
    push r12    ; Push R12
    push r13    ; Push R13
    push r14    ; Push R14
    push r15    ; Push R15
    ; Note: RFLAGS is pushed by the CPU automatically during interrupt entry.
    ; We don't push RSP here as it's part of the interrupt stack frame.
%endmacro

%macro RESTORE_REGS 0
    pop r15     ; Pop R15
    pop r14     ; Pop R14
    pop r13     ; Pop R13
    pop r12     ; Pop R12
    pop r11     ; Pop R11
    pop r10     ; Pop R10
    pop r9      ; Pop R9
    pop r8      ; Pop R8
    pop rbp     ; Pop RBP
    pop rdi     ; Pop RDI
    pop rsi     ; Pop RSI
    pop rdx     ; Pop RDX
    pop rcx     ; Pop RCX
    pop rax     ; Pop RAX
%endmacro

; ----------------------------------------------------------------------------
; IRQ0 Handler (Timer Interrupt)
;
; This is the entry point for IRQ0. It saves the CPU context, calls the
; C-level Interrupt Service Routine (ISR), restores the context, and
; returns from the interrupt.
; ----------------------------------------------------------------------------
irq0:
    cli         ; Disable interrupts (CPU already does this on interrupt entry, but good for clarity)
    SAVE_REGS   ; Save all general-purpose registers onto the stack
    call isr0   ; Call the C-level ISR for IRQ0
    RESTORE_REGS; Restore all general-purpose registers from the stack
    sti         ; Enable interrupts (Set Interrupt Flag)
    iretq       ; Return from interrupt (64-bit instruction)

; ----------------------------------------------------------------------------
; IRQ1 Handler (Keyboard Interrupt)
;
; This is the entry point for IRQ1. It saves the CPU context, calls the
; C-level Interrupt Service Routine (ISR), restores the context, and
; returns from the interrupt.
; ----------------------------------------------------------------------------
irq1:
    cli         ; Disable interrupts
    SAVE_REGS   ; Save all general-purpose registers
    call isr1   ; Call the C-level ISR for IRQ1 (keyboard handler)
    RESTORE_REGS; Restore all general-purpose registers
    sti         ; Enable interrupts
    iretq       ; Return from interrupt (64-bit instruction)
