;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
	MODULE min_monitor_sdcc
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
	XDEF _execute
	XDEF _setMemoryValue
	XDEF _getMemoryValue
;--------------------------------------------------------
; Externals used
;--------------------------------------------------------
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
_D_ADDR_H	=	0x0000
_D_ADDR_L	=	0x0001
_D_DATA	=	0x0002
_KBD_PORT	=	0x0003
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
;min_monitor_sdcc.c:21: char getMemoryValue(unsigned int addr) {
;	---------------------------------
; Function getMemoryValue
; ---------------------------------
._getMemoryValue
;min_monitor_sdcc.c:22: return 0;
	ld	l, $00

.l_getMemoryValue00101
;min_monitor_sdcc.c:23: }
	ret
;min_monitor_sdcc.c:24: void setMemoryValue(unsigned int addr, char data) {
;	---------------------------------
; Function setMemoryValue
; ---------------------------------
._setMemoryValue
;min_monitor_sdcc.c:26: }

.l_setMemoryValue00101
	ret
;min_monitor_sdcc.c:27: void execute(unsigned int addr) {
;	---------------------------------
; Function execute
; ---------------------------------
._execute
;min_monitor_sdcc.c:29: }

.l_execute00101
	ret
;min_monitor_sdcc.c:31: void main() {
;	---------------------------------
; Function main
; ---------------------------------
._main
	push	ix
	ld	ix,0
	add	ix,sp
	push	af
;min_monitor_sdcc.c:32: unsigned int address = FIRST_RAM;
	ld	bc, $0C00
;min_monitor_sdcc.c:33: while(1) {

.l_main00118
;min_monitor_sdcc.c:34: char currentData = getMemoryValue(address);
	push	bc
	push	bc
	call	_getMemoryValue
	pop	af
	ld	a, l
	pop	bc
	ld	e, a
;min_monitor_sdcc.c:36: outData = address >> 8;
	ld	a, b
	ld	l, $00
	out	(_D_ADDR_H), a
;min_monitor_sdcc.c:38: outData = address;
	ld	a, c
	out	(_D_ADDR_L), a
;min_monitor_sdcc.c:41: D_DATA = outData;
	ld	a, e
	out	(_D_DATA), a
;min_monitor_sdcc.c:42: while(1) {

.l_main00115
;min_monitor_sdcc.c:43: char key = NOT_PRESSED;
	ld	d, $FF
;min_monitor_sdcc.c:44: while (key == NOT_PRESSED) {

.l_main00101
	ld	a, d
	inc	a
	jp	NZ,l_main00167
	jp	l_main00168

.l_main00167
	jp	l_main00103

.l_main00168
;min_monitor_sdcc.c:45: key = KBD_PORT;
	in	a, (_KBD_PORT)
	ld	d, a
	jp	l_main00101

.l_main00103
;min_monitor_sdcc.c:47: if (key < 0x10) {
	ld	a, d
	sub	a, $10
	jp	NC, l_main00105
;min_monitor_sdcc.c:48: currentData = currentData * 0x10 + key;
	ld	a, e
	add	a, a
	add	a, a
	add	a, a
	add	a, a
	ld	e, d
	add	a, e
;min_monitor_sdcc.c:49: D_DATA = currentData;
	ld	e, a
	out	(_D_DATA), a

.l_main00105
;min_monitor_sdcc.c:53: address = address & 0x00ff + currentData * 0x100;
	ld	(ix-2), e
	xor	a, a
	ld	(ix-1), a
;min_monitor_sdcc.c:52: if (key == C_SETHB) {
	ld	a, d
	sub	a, $10
	jp	NZ,l_main00169
	jp	l_main00170

.l_main00169
	jp	l_main00107

.l_main00170
;min_monitor_sdcc.c:53: address = address & 0x00ff + currentData * 0x100;
	ld	d, (ix-2)
	ld	e, $00
	ld	hl, $00FF
	add	hl, de
	ld	a, l
	and	a, c
	ld	c, a
	ld	a, h
	and	a, b
	ld	b, a
;min_monitor_sdcc.c:54: break;
	jp	l_main00118

.l_main00107
;min_monitor_sdcc.c:57: if (key == C_SETLB) {
	ld	a, d
	sub	a, $11
	jp	NZ,l_main00171
	jp	l_main00172

.l_main00171
	jp	l_main00109

.l_main00172
;min_monitor_sdcc.c:58: address = address & 0xff00 + currentData;
	ld	e, (ix-2)
	ld	l, (ix-1)
	ld	a, l
	dec	a
	push	af
	ld	a, c
	and	a, e
	ld	c, a
	pop	af
	and	a, b
	ld	b, a
;min_monitor_sdcc.c:59: break;
	jp	l_main00118

.l_main00109
;min_monitor_sdcc.c:62: if (key == C_SETDAT) {
	ld	a, d
	sub	a, $12
	jp	NZ,l_main00173
	jp	l_main00174

.l_main00173
	jp	l_main00111

.l_main00174
;min_monitor_sdcc.c:63: setMemoryValue(address, currentData);
	push	bc
	ld	a, e
	push	af
	inc	sp
	push	bc
	call	_setMemoryValue
	pop	af
	inc	sp
	pop	bc
;min_monitor_sdcc.c:64: address++;
	inc	bc
;min_monitor_sdcc.c:65: break;
	jp	l_main00118

.l_main00111
;min_monitor_sdcc.c:68: if (key == C_RUN) {
	ld	a, d
	sub	a, $13
	jp	NZ,l_main00175
	jp	l_main00176

.l_main00175
	jp	l_main00115

.l_main00176
;min_monitor_sdcc.c:69: execute(address);
	push	bc
	push	de
	push	bc
	call	_execute
	pop	af
	pop	de
	pop	bc
	jp	l_main00115

.l_main00120
;min_monitor_sdcc.c:73: }
	ld	sp, ix
	pop	ix
	ret
	; Area CODE
	; Area  INITIALIZER
	; Area  CABS (ABS)
