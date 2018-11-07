#include "fail.h"
#include "44b.h"
#include "44blib.h"
#include "stdlib.h"
#include "8led.h"

volatile unsigned int tipo_excepcion = 7;
volatile unsigned int direccion = 7;

void excepciones_ISR( void ) __attribute__((interrupt ( "ABORT" )));

void excepciones_ISR(void){
	__asm__("ldr r0, =tipo_excepcion\n"
			"ldr r1, =direccion\n"
			"mrs r2, CPSR\n"
			"sub r3, r14, #8\n"
			"str r3,[r1]\n"
			"str r2,[r0]\n");
	tipo_excepcion &= 0x1F;
	switch(tipo_excepcion){
	case 0x17:
		D8Led_symbol(10);
		break;
	case 0x1B:
		D8Led_symbol(17);
		break;
	case 0x13:
		D8Led_symbol(5);
		break;
	default:
		D8Led_symbol(14);
	}
	while(1);
}

void excepciones_inicializar(void){
	pISR_PABORT = (unsigned) excepciones_ISR;
	pISR_DABORT = (unsigned) excepciones_ISR;
	pISR_UNDEF = (unsigned) excepciones_ISR;
	pISR_SWI = (unsigned) excepciones_ISR;
}

