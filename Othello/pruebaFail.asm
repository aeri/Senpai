.arm
.global Undef
.global Abort
.global SWI
.equ DIR, 0xc7fef94

Undef:
	ldr r1, =DIR
	mov pc,r1
Abort:
	mov r1,#0x55
	ldr r0,[r1]
	str r0,[r0]
	mov pc,lr
SWI:
	SWI 12
	mov pc,lr
.end
