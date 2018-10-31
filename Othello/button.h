/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/
#include "stdbool.h"
#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */

void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));
void Eint4567_init(void);

unsigned int button_estado();

void button_empezar(void *callback);

bool haInterrumpido_boton();

void reset_interrupcion_boton();

#endif /* _BUTTON_H_ */
