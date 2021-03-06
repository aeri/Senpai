/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*--- variables visibles del módulo timer.c/timer.h ---*/
static volatile unsigned long long inter2;

void timer2_inicializar(void);

void timer2_empezar(void);

unsigned long long timer2_leer();

unsigned long long timer2_parar();

#endif /* _TIMER_H_ */
