#include "fail.h"
#include "44b.h"
#include "44blib.h"
#include "stdlib.h"

void excepciones_ISR( void ) __attribute__((interrupt ( "ABORT" )));
/*void excepciones_ISR( void ) __attribute__((interrupt ( UNDEF )));
void excepciones_ISR( void ) __attribute__((interrupt ( SWI ))); */

void excepciones_ISR(void){
	__asm__("ldr r0, =tipo_excepcion\n\t"
			"ldr r1, =direccion\n\t"
			"mrs r2, CPSR\n\t"
			"sub r3, r14, #8\n\t"
			"str r3,[r1]\n\t"
			"str r2,[r0]");
	tipo_excepcion &= 0x1F;
	int a1=1;
}

void excepciones_inicializar(void){
	pISR_PABORT = (unsigned) excepciones_ISR;
	pISR_DABORT = (unsigned) excepciones_ISR;
	/*pISR_UNDEF = (unsigned) excepciones_ISR;
	pISR_SWI = (unsigned) excepciones_ISR;*/
}
