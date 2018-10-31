/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer0.h"
#include "led.h"
#include "44b.h"
#include "44blib.h"
#include "stdbool.h"
#include "botones_antirrebotes.h"

/*--- variables globales ---*/
int switch_leds = 0;
static volatile int latido = 25;
static volatile bool led_rebote = false;
/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer_ISR(void) __attribute__((interrupt("IRQ")));
volatile bool interrupcion_timer0;

/*--- codigo de las funciones ---*/
void timer_ISR(void)
{
	switch_leds = 1;
	timer_interruption();
	if(latido != 0){
		latido--;
	}
	else{
		latido = 25;
		if(led_rebote){
			led1_on();
			led_rebote = !led_rebote;
		}
		else{
			led1_off();
			led_rebote = !led_rebote;
		}
	}

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer_ISR;

	/* Configura el Timer0 */
	rTCFG0 |= 0x000000c7; // ajusta el preescalado
	rTCFG1 |= 0x00000004; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 200;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 0;// valor de comparaci�n
	/* establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON &= 0xFFFFFFFD;
	rTCON |= 0xD;
	interrupcion_timer0 = false;
}
