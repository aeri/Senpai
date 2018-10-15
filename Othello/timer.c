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
#include "stdbool.h"

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
	rTCMPB2 = 12800;// valor de comparación
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

unsigned int timer2_leer(){
	bool correcto=false;
	int toma1,toma2,tics;
	while (!correcto){
		toma1=inter2;
		tics=rTCNTO2;
		toma2=inter2;
		correcto=(toma1==toma2);
	}
	return (toma1*rTCNTB2+(rTCNTB2-tics))/32;
}

unsigned int timer2_parar(){
	// Parar el timer y bajar auto-reload
	rTCON &= 0xFFF6FFF;

	int toma,tics;
	toma=inter2;
	tics=rTCNTO2;
	return (toma*rTCNTB2+(rTCNTB2-tics))/32;
}
