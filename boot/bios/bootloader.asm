; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%include "x64/control_registers.inc"
%include "x64/segmentation.inc"
%include "x64/paging.inc"

; Page maps buffer info
PAGE_MAPS_BUFFER equ 0x1000
PAGE_MAPS_BUFFER_SIZE equ 0x3000
PAGE_MAPS_DWORDS equ PAGE_MAPS_BUFFER_SIZE / 4

; Page maps addresses
PML4 equ 0x1000
PML3 equ 0x2000
PML2 equ 0x3000

; Stage 2 size info
STAGE2_SIZE equ bootloader_end - load_kernel
STAGE2_SECTORS equ STAGE2_SIZE / 512
STAGE2_MAX_SIZE equ 31744

%if STAGE2_SIZE > STAGE2_MAX_SIZE
    %error "Stage 2 is longer than expected"
%endif

; BootSector load address
org 0x7C00

bits 16

; Disable interrupts and perform a far jump to clear CS
start:
    cli
    jmp 0x0000:.clear_segment_selectors

; Initialize segment registers and re-enable interrupts
.clear_segment_selectors:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    sti

; Load Stage 2 from disk
; DL contains boot drive number
.load_stage2:
    mov ah, 0x02            ; Service 2 (Read sectors)
    mov al, STAGE2_SECTORS  ; Sectors to read
    xor ch, ch              ; Cylinder number
    mov cl, 0x02            ; Starting sector
    xor dh, dh              ; Head number
    mov bx, load_kernel     ; Address of memory buffer (ES:BX)
    int 0x13                ; Disk I/O services interrupt

; Ensure that A20 line is enabled
.enable_a20_line:
    in al, 0x92             ; Get System Control Port A value
    or al, 0x02             ; Set Fast A20 Gate bit
    and al, 0xFE            ; Clear Fast Reset bit
    out 0x92, al            ; Save modified value

; Fill page maps buffer with zero
.clear_page_maps:
    mov di, PAGE_MAPS_BUFFER
    mov cx, PAGE_MAPS_DWORDS
    xor eax, eax
    cld
    rep stosd

; Build identity paging for first 4MB
.build_page_maps:
    mov dword [PML4], BASE_PAGE_ENTRY(PML3)
    mov dword [PML3], BASE_PAGE_ENTRY(PML2)
    mov dword [PML2], LARGE_PAGE_ENTRY(0x000000)
    mov dword [PML2+PAGE_MAP_ENTRY_SIZE], LARGE_PAGE_ENTRY(0x200000)

; Prepare to switch directly to 64-bit Long Mode
.enter_long_mode:
    cli

    mov eax, cr4
    or eax, CR4_PAE         ; Set PAE (Physical Address Extension) bit
    mov cr4, eax

    mov eax, PML4
    mov cr3, eax            ; Set PML4 as root page map

    mov ecx, IA32_EFER_MSR  ; Select Extended Feature Enabled Register
    rdmsr
    or eax, EFER_LME        ; Set Long Mode Enabled bit
    wrmsr

    lgdt [GDT.Pointer]      ; Load Global Descriptor Table

    mov eax, cr0
    or eax, CR0_PG | CR0_PE ; Set PG (Paging) and PE (Protection Enabled) bits
    mov cr0, eax

    jmp GDT.Code:long_mode  ; Perform a far jump to load 64-bit CS

bits 64

; Setup stack and segment selectors
long_mode:
    mov ax, GDT.Data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0
    mov esp, 0x7c00

; Call Stage 2 entry point
.call_stage2:
    call load_kernel

; GDT with 64-bit segment descriptors
align 8
GDT:
    .Null: equ $ - GDT
        NULL_SEGMENT_DESCRIPTOR
    .Code: equ $ - GDT
        CODE_SEGMENT_DESCRIPTOR
    .Data: equ $ - GDT
        DATA_SEGMENT_DESCRIPTOR
    .Pointer:
        dw $ - GDT - 1
        dd GDT

; Zero padding and MBR magic number
times 510-($-$$) db 0
dw 0xAA55

; Pick Stage 2 binary path from a compile-time defined macro
%defstr BOOTLOADER_STAGE2_BIN BOOTLOADER_STAGE2_BIN

; Include Stage 2 binary aligned to 512 bytes
load_kernel:
    incbin BOOTLOADER_STAGE2_BIN
    align 512, db 0

; Mark bootloader end
bootloader_end: