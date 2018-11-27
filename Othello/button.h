/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/
#include "stdbool.h"
#ifndef _BUTTON_H_
#define _BUTTON_H_
//#define SIM

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */

#ifndef SIM
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));
void Eint4567_init(void);


int leer_tiempo();

void button_empezar();

#else
void cambiar_estado(int state);
#endif

unsigned int button_estado();

void boton_reset_test();

int leer_interrupciones();

#endif /* _BUTTON_H_ */
