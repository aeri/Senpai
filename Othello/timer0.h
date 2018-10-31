/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/
#include "stdbool.h"
#ifndef _TIMER0_H_
#define _TIMER0_H_

/*--- variables visibles del m�dulo timer.c/timer.h ---*/
int switch_leds;

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/
void timer_init(void);

#endif /* _TIMER0_H_ */
