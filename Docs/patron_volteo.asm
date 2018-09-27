          patron_volteo:
0c000324:   mov r12, sp
0c000328:   push {r11, r12, lr, pc}
0c00032c:   sub r11, r12, #4
0c000330:   sub sp, sp, #48 ; 0x30
0c000334:   str r0, [r11, #-32]
0c000338:   str r1, [r11, #-36]     ; 0x24
0c00033c:   strb r2, [r11, #-37]    ; 0x25
0c000340:   strb r3, [r11, #-38]    ; 0x26
196           FA = FA + SF;
0c000344:   ldrb r2, [r11, #-37]    ; 0x25
0c000348:   ldrb r3, [r11, #4]
0c00034c:   add r3, r2, r3
0c000350:   strb r3, [r11, #-37]    ; 0x25
197           CA = CA + SC;
0c000354:   ldrb r2, [r11, #-38]    ; 0x26
0c000358:   ldrb r3, [r11, #8]
0c00035c:   add r3, r2, r3
0c000360:   strb r3, [r11, #-38]    ; 0x26
198           casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
0c000364:   ldrb r1, [r11, #-37]    ; 0x25
0c000368:   ldrb r2, [r11, #-38]    ; 0x26
0c00036c:   sub r3, r11, #24
0c000370:   ldr r0, [r11, #-32]
0c000374:   bl 0xc000270 <ficha_valida>
0c000378:   mov r3, r0
0c00037c:   strb r3, [r11, #-13]
201           if ((posicion_valida == 1) && (casilla != color))
0c000380:   ldr r3, [r11, #-24]
0c000384:   cmp r3, #1
0c000388:   bne 0xc0003e8 <patron_volteo+196>
0c00038c:   ldrb r2, [r11, #-13]
0c000390:   ldrb r3, [r11, #12]
0c000394:   cmp r2, r3
0c000398:   beq 0xc0003e8 <patron_volteo+196>
203               *longitud = *longitud + 1;
0c00039c:   ldr r3, [r11, #-36]     ; 0x24
0c0003a0:   ldr r3, [r3]
0c0003a4:   add r2, r3, #1
0c0003a8:   ldr r3, [r11, #-36]     ; 0x24
0c0003ac:   str r2, [r3]
204               patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
0c0003b0:   ldrb r2, [r11, #-37]    ; 0x25
0c0003b4:   ldrb r3, [r11, #-38]    ; 0x26
0c0003b8:   ldrb r1, [r11, #4]
0c0003bc:   str r1, [sp]
0c0003c0:   ldrb r1, [r11, #8]
0c0003c4:   str r1, [sp, #4]
0c0003c8:   ldrb r1, [r11, #12]
0c0003cc:   str r1, [sp, #8]
0c0003d0:   ldr r0, [r11, #-32]
0c0003d4:   ldr r1, [r11, #-36]     ; 0x24
0c0003d8:   bl 0xc000324 <patron_volteo>
0c0003dc:   str r0, [r11, #-20]
207               return patron;
0c0003e0:   ldr r3, [r11, #-20]
0c0003e4:   b 0xc000428 <patron_volteo+260>
211           else if ((posicion_valida == 1) && (casilla == color))
0c0003e8:   ldr r3, [r11, #-24]
0c0003ec:   cmp r3, #1
0c0003f0:   bne 0xc000424 <patron_volteo+256>
0c0003f4:   ldrb r2, [r11, #-13]
0c0003f8:   ldrb r3, [r11, #12]
0c0003fc:   cmp r2, r3
0c000400:   bne 0xc000424 <patron_volteo+256>
213               if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
0c000404:   ldr r3, [r11, #-36]     ; 0x24
0c000408:   ldr r3, [r3]
0c00040c:   cmp r3, #0
0c000410:   ble 0xc00041c <patron_volteo+248>
215                   return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
0c000414:   mov r3, #1
0c000418:   b 0xc000428 <patron_volteo+260>
219                   return NO_HAY_PATRON; // si no hay que voltear no hay patrón
0c00041c:   mov r3, #0
0c000420:   b 0xc000428 <patron_volteo+260>
226               return NO_HAY_PATRON;
0c000424:   mov r3, #0
229       }
0c000428:   mov r0, r3
0c00042c:   sub sp, r11, #12
0c000430:   ldm sp, {r11, sp, lr}
0c000434:   bx lr
236       {