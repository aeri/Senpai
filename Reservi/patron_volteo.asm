.global patron_volteo
.extern ficha_valida

patron_volteo:
	MOV IP,SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	@ re:zero = tablero
	mov r7, r1			@ r7 = *longitud
	mov r8, r2			@ r8 = FA
	mov r9, r3			@ r9 = CA
	ldrb r4 , [FP, #4] @ r4 = SF
	ldrb r5 , [FP, #8] @ r5 = SC
	ldrb r6 , [FP, #12] @ r6 = color

	add r8, r8, r4 @ FA = FA + SF
	add r9, r9, r5 @ CA = CA + SC
	mov r1, r8
	mov r2, r9
	sub SP, SP, #4 @ Reserva
	mov r3, SP
	mov r10, r0 @ r10 = tablero
	bl ficha_valida
	ldr r3, [SP]
	add SP, SP, #4
	cmp r3, #1 @ ¿posicion_valida==1?
	bne nouno
	cmp r0, r6 @ ¿casilla==color?
	beq igualcolor
@posicion_valida == 1 && casilla!=color
	mov r2, r8 @ r2 = FA
	mov r3, r9 @ r3 = CA
	mov r1, r7 @ r1 = *longitud
	ldr r7, [r1] @ r7 = longitud
	add r7, r7, #1
	str r7, [r1]
	mov r0, r10 @ re:zero = tablero
	str r4, [SP]
	str r5, [SP, #4]
	str r6, [SP, #8]
	bl patron_volteo
	b epilogo
igualcolor:
@posicion_valida ==1 && casilla == color
	ldr r7, [r7] @ r7 = longitud
	cmp r7, #0
	ble nouno
	mov r0, #1 @ return 1
	b epilogo
nouno:
@posicion_valida != 1 || (posicion_valida == 1 && casilla == color && longitud <= 0)
	mov r0, #0 @ return 0

epilogo:
	@SUB SP, FP, #12
	LDMIA SP!, {r4-r10, FP, SP, PC}


.end
