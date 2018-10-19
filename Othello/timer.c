/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "44b.h"
#include "44blib.h"
#include "stdlib.h"

/*--- variables globales ---*/
static volatile int inter2 = 0;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	inter2++;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer2 */
	rTCFG0 &= 0xffff00ff; // ajusta el preescalado
	rTCFG1 &= 0xfffff0ff; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = 0;// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0xc000;
}

void timer2_empezar(void){
	inter2=0;
	// Activar manual update
	rTCON |= 0x2000;
	// Desactivar manual update y activar el bit start
	rTCON ^= 0x0003000;
}

inline static unsigned int ticks_to_ms(unsigned int ticks)
{
	return ticks >> 5; /* dividir por 32 */
}

unsigned int timer2_leer(){

	unsigned int ints_antes, ints_despues;
	size_t i = 0;

	ints_antes = inter2;
	unsigned int ticks=rTCNTO2;
	ints_despues = inter2;

	for(i=0; (i< 10) && (ints_antes != ints_despues); ++i ){
		ints_antes = inter2;
		ticks=rTCNTO2;
		ints_despues = inter2;
	}

	unsigned int ticks_totales = ints_antes * rTCNTB2 + (rTCNTB2-ticks);

	return ticks_to_ms(ticks_totales);
}

unsigned int timer2_parar(){
	// Parar el timer y bajar auto-reload
	rTCON &= 0xFFF6FFF;

	unsigned int toma,tics;
	toma=inter2;
	tics=rTCNTO2;
	return ticks_to_ms(toma*rTCNTB2+(rTCNTB2-tics));
}
