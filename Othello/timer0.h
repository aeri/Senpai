/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/
#include "stdbool.h"
#ifndef _TIMER0_H_
#define _TIMER0_H_
//#define SIM
/*--- variables visibles del m�dulo timer.c/timer.h ---*/
#ifndef SIM
int switch_leds;

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/
void timer_init(void);
#else
void interrumpirTimer();
#endif

int interrupcionesTimer();

void resetTimer();

#endif /* _TIMER0_H_ */
