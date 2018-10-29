.arm
.global Undef
.global Abort

Undef:
			mov r1, #0x55
			mov pc,r1


Abort:
			mov r1,#0x55
			ldr r0,[r1]
			str r0,[r0]
			mov pc,lr

.end
