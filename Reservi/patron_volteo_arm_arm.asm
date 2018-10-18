.global patron_volteo_arm_arm
.equ    DIM, 8
.equ	CASILLA_VACIA, 0
.equ	PATRON_ENCONTRADO, 1
.equ	NO_HAY_PATRON, 0
patron_volteo_arm_arm:
	MOV IP,SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	@ r0 = tablero
	@ r1 = *longitud
	@ r2 = FA
	@ r3 = CA
	LDMIB FP, {r4,r5,r6}
	@ r4 = SF
	@ r5 = SC
	@ r6 = color

	add r2, r2, r4 @ FA = FA + SF
	add r3, r3, r5 @ CA = CA + SC

	cmp r2, #DIM
	bge else
	cmp r2, #0
	blt else
	cmp r3, #DIM
	bge else
	cmp r3, #0
	blt else
	@mov r8, r2, LSL #3
	add r8, r3, r2, LSL #3 @Calculamos la posición del tablero
	add r8, r0, r8 @Sumamos la posición inicial del tablero
	ldrb r8, [r8]
	cmp r8, #CASILLA_VACIA
	beq else
	mov r9, #1 @posicion_valida=1
	b fin

else:
	mov r9, #0 @posicion_valida=0
	@mov r8, #CASILLA_VACIA

fin:
	cmp r9, #1 @ ¿posicion_valida==1?
	bne nouno
	cmp r8, r6 @ ¿ficha==color?
	beq igualcolor
@posicion_valida == 1 && casilla!=color
	ldr r7, [r1] @ r7 = longitud
	add r7, r7, #1
	str r7, [r1]
	sub sp,sp,#12 @ Reserva espacio en la pila para cargar los resultados
	STMIA SP, {r4,r5,r6}
	bl patron_volteo_arm_arm
	add sp,sp,#12  @ Se devuelve la cima de la pila a su estado original
	b epilogo
igualcolor:
@posicion_valida ==1 && casilla == color
	ldr r7, [r1] @ r7 = longitud
	cmp r7, #0
	ble nouno
	mov r0, #PATRON_ENCONTRADO @ return 1
	b epilogo
nouno:
@posicion_valida != 1 || (posicion_valida == 1 && casilla == color && longitud <= 0)
	mov r0, #NO_HAY_PATRON @ return 0

epilogo:
	LDMIA SP, {r4-r10, FP, SP, PC}


.end
