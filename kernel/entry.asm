; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%include "sys/memory_layout.inc"
%include "x64/paging.inc"

bits 64

; Use RIP-Relative Addressing by default
default rel

; Import main from main.c
extern main;

; Export early PML4
global early_pml4

; Export entry point with physical address
global _start
_start equ SYMBOL_PA(entry)

section .text

; Ensure that interrupts are disabled on entry
entry:
    cli

; Create new page maps before jump to kernel
; - Identity map: required to not crash
; - Direct map: used to map physical memory
; - Kernel map: required to run kernel code
build_page_maps:
    mov rdx, PAGE_ADDRESS_MASK

; Fill PML4 entries
.setup_pml4:

    ; Put Identity PML3 on PML4
    lea rax, [identity_pml3]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [early_pml4], rax

    ; Put Direct PML3 on PML4
    lea rax, [direct_pml3]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [early_pml4 + PML4_OFFSET(DIRECT_MAP_BASE)], rax

    ; Put Kernel PML3 on PML4
    lea rax, [kernel_pml3]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [early_pml4 + PML4_OFFSET(KERNEL_MAP_BASE)], rax

; Fill PML3 entries
.setup_pml3:

    ; Put Identity PML2 on Identity PML3
    lea rax, [identity_pml2]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [identity_pml3], rax

    ; Put Direct PML2 on Direct PML3
    lea rax, [direct_pml2]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [direct_pml3 + PML3_OFFSET(DIRECT_MAP_BASE)], rax

    ; Put Kernel PML2 on Kernel PML3
    lea rax, [kernel_pml2]
    and rax, rdx
    or rax, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG
    mov [kernel_pml3 + PML3_OFFSET(KERNEL_MAP_BASE)], rax

; Fill PML2 entries
.setup_pml2:

    ; Build identity paging for first 4MB
    mov rax, LARGE_PAGE_ENTRY(0x000000)
    mov [identity_pml2], rax
    mov rax, LARGE_PAGE_ENTRY(0x200000)
    mov [identity_pml2 + 8], rax

    ; Build direct map for first 4MB
    mov rax, LARGE_PAGE_ENTRY(0x000000)
    mov [direct_pml2 + PML2_OFFSET(DIRECT_MAP_BASE)], rax
    mov rax, LARGE_PAGE_ENTRY(0x200000)
    mov [direct_pml2 + PML2_OFFSET(DIRECT_MAP_BASE) + 8], rax

    ; Build kernel map (2MB starting from kernel load address)
    mov rax, LARGE_PAGE_ENTRY(KERNEL_LOAD_ADDRESS)
    mov [kernel_pml2 + PML2_OFFSET(KERNEL_MAP_BASE)], rax

; Switch to the new page maps
switch_page_maps:
    lea rax, [early_pml4]
    mov cr3, rax

; Set up kernel stack
setup_kernel_stack:
    xor rbp, rbp
    mov rsp, kernel_stack

; Perform a absolute indirect jump to main
jump_main:
    mov rax, main
    jmp rax

section .bss

; Reserve memory for page maps
RESERVE_PAGE_MAP early_pml4
RESERVE_PAGE_MAP identity_pml3
RESERVE_PAGE_MAP identity_pml2
RESERVE_PAGE_MAP direct_pml3
RESERVE_PAGE_MAP direct_pml2
RESERVE_PAGE_MAP kernel_pml3
RESERVE_PAGE_MAP kernel_pml2

; Reserve memory for kernel stack
align KERNEL_STACK_ALIGNMENT
resb KERNEL_STACK_SIZE
kernel_stack equ $