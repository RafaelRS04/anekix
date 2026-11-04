; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%ifndef X64_SEGMENTATION_INC
%define X64_SEGMENTATION_INC

; Execute/Read Code Segment
CODE_SEGMENT_TYPE equ 1010b

; Read/Write Data Segment
DATA_SEGMENT_TYPE equ 0010b

; Legacy Mode Segment Descriptor (32-bit)
; %1 = Base, %2 = Limit, %3 = Type
%macro LEGACY_SEGMENT_DESCRIPTOR 3
    dw ((%2) >> 12) & 0xFFFF       ; Limit 15:00
    dw (%1) & 0xFFFF               ; Base 15:00
    db ((%1) >> 16) & 0xFF         ; Base 23:16
    db ((%3) & 0xF) | 0x90         ; P=1, DPL=00, S=1, Type 3:0
    db (((%2) >> 28) & 0xF) | 0xC0 ; G=1, D/B=1, L=0, AVL=0, Limit 19:16
    db ((%1) >> 24) & 0xFF         ; Base 31:24
%endmacro

%macro CODE_SEGMENT_DESCRIPTOR 0
    dw 0                           ; Ignored
    dw 0                           ; Ignored
    db 0                           ; Ignored
    db 0x90 | CODE_SEGMENT_TYPE    ; P=1, DPL=00, S=1
    db 0x20                        ; D/B=0, L=1
    db 0                           ; Ignored
%endmacro

%macro DATA_SEGMENT_DESCRIPTOR 0
    dw 0                           ; Ignored
    dw 0                           ; Ignored
    db 0                           ; Ignored
    db 0x90 | DATA_SEGMENT_TYPE    ; P=1, DPL=00, S=1
    db 0                           ; Ignored
    db 0                           ; Ignored
%endmacro

%macro NULL_SEGMENT_DESCRIPTOR 0
    dq 0
%endmacro

%endif