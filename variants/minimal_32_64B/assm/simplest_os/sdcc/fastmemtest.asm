;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
	MODULE fastmemtest
	.optsdcc -mz80
	; Generated using the z80asm/z88 tokens.
	XREF __muluchar_rrx_s
	XREF __mulschar_rrx_s
	XREF __mulint_rrx_s
	XREF __mullong_rrx_s
	XREF __divuchar_rrx_s
	XREF __divschar_rrx_s
	XREF __divsint_rrx_s
	XREF __divuint_rrx_s
	XREF __divulong_rrx_s
	XREF __divslong_rrx_s
	XREF __rrulong_rrx_s
	XREF __rrslong_rrx_s
	XREF __rlulong_rrx_s
	XREF __rlslong_rrx_s

;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	XDEF _main
;--------------------------------------------------------
; Externals used
;--------------------------------------------------------
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	; Aread BSS
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	; Area  INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	; Area  DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	; Area  HOME
	; Area  GSINIT
	; Area  GSFINAL
	; Area  GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	; Area HOME
	; Area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	; Area CODE
;fastmemtest.c:1: void main() {
;	---------------------------------
; Function main
; ---------------------------------
._main
;fastmemtest.c:5: __endasm;
	ld	a, 0x55
	ld	(0x0e), a

.l_main00101
;fastmemtest.c:6: }
	ret
	; Area CODE
	; Area  INITIALIZER
	; Area  CABS (ABS)
