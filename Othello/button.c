/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/
//#define SIM
//#define TEST

/*--- ficheros de cabecera ---*/
#include "button.h"

#ifndef SIM
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "pilaDebug.h"
#include "timer.h"
#else
/*
 * Leyenda
 * 0x00 -> No presionado
 * 0x80 -> Presionado el de la izquierda
 * 0x40 -> Presionado el de la derecha
 */
int state = 0x0;
#endif

#include "botones_antirrebotes.h"
int diferencia = -1;
int inicial;
int interrupciones = 0;
/*--- variables globales del módulo ---*/
/* int_count la utilizamos para sacar un número por el 8led.
  Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces hay rebotes! */
//static volatile unsigned int int_count = 0;

/*--- codigo de funciones ---*/
void botones_test();
#ifndef SIM
void Eint4567_ISR(void)
{
#ifndef TEST
	rINTMSK |= BIT_EINT4567; //Se desactivan las interrupciones del botón
	button_callback(rPDATG & 0xc0);
#else
	botones_test();
#endif
	unsigned int button_time = timer2_leer();
	push_debug(0x03, button_time);
	/* Identificar la interrupcion (hay dos pulsadores)*/
	// }
	//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)

	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void botones_test(){
	if(inicial == 0){
		inicial = timer2_leer();
		diferencia = 0;
	}
	else{
		diferencia = timer2_leer() - inicial;
	}
	interrupciones++;
}

int leer_interrupciones(){
	return interrupciones;
}

void boton_reset_test(){
	inicial = 0;
	diferencia = -1;
	interrupciones = 0;
}

int leer_tiempo(){
	return diferencia;
}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
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
}
#endif
void button_empezar(){
#ifndef SIM
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
	rINTMSK &= ~(BIT_EINT4567); // Se activan interrupciones
	//pISR_EINT4567 = (int) callback; // Se vincula la función callback para que se salte a ella en una interrupción del botón
#endif
}

unsigned int button_estado(){
#ifndef SIM
	return rPDATG & 0xc0;
#else
	return state;
#endif
}

#ifdef SIM
void cambiar_estado(int estado) {
	state = estado;
}
#endif
