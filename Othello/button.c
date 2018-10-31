/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "botones_antirrebotes.h"

/*--- variables globales del m�dulo ---*/
/* int_count la utilizamos para sacar un n�mero por el 8led.
  Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces hay rebotes! */
//static volatile unsigned int int_count = 0;

volatile bool interrupcion_button;

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
	rINTMSK |= BIT_EINT4567; //Se desactivan las interrupciones del botón
	push_debug(0x01, 0xBEBACAFE);
	boton_callback();
	/* Identificar la interrupcion (hay dos pulsadores)*/
	// }
	//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)

	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;
	interrupcion_button = false;
}

unsigned int button_estado(){
	return rPDATG & 0xc0;
}

void button_empezar(void *callback){
	rINTMSK &= ~(BIT_EINT4567); // Se activan interrupciones
	//pISR_EINT4567 = (int) callback; // Se vincula la función callback para que se salte a ella en una interrupción del botón
	//boton_callback = callback;
}
