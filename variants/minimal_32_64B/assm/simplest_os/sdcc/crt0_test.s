.module crt0
	.globl _main
	.area   _HEADER (ABS)
    ;; Reset vector
    .org    0
init:
    call    _main
    
;; Ordering of segments for the linker.
    .area   _HOME
    .area   _CODE

    .area   _DATA

    .area   _CODE
