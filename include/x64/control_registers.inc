; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%ifndef X64_CONTROL_REGISTERS_INC
%define X64_CONTROL_REGISTERS_INC

IA32_EFER_MSR equ 0xC0000080
EFER_LME equ 1 << 8

CR0_PG equ 1 << 31
CR0_PE equ 1 << 0

CR4_PAE equ 1 << 5

%endif