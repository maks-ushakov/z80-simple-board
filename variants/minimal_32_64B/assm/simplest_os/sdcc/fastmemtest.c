void main() {
__asm
	ld a, 0x55
	ld (0x0e), a
__endasm;
}
