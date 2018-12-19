/*********************************************************************************************
* Fichero:	8led.c
* Autor:
* Descrip:	Funciones de control del display 8-segmentos
* Version:
*********************************************************************************************/
//#define SIM
/*--- ficheros de cabecera ---*/
#include "8led.h"

#ifdef SIM
int estado8led = 0;
#endif

#ifndef SIM
#include "44b.h"
#include "44blib.h"

/*--- variables globales del m�dulo ---*/
/* tabla de segmentos */
static int
Symbol[size_8led] = { cero, uno, dos, tres, cuatro, cinco, seis, siete, ocho, nueve, A, B, C, D, E, F, blank, U, P, L};

/*--- c�digo de las funciones ---*/
void D8Led_init(void)
{
	/* Estado inicial del display con todos los segmentos iluminados
	   (buscar en los ficheros de cabecera la direccion implicada) */
	LED8ADDR = (unsigned char) cero;
}
#endif

void D8Led_symbol(int value)
{
#ifndef SIM
	/* muestra el Symbol[value] en el display (analogo al caso anterior) */
	if ((value >= 0) && (value < size_8led)) {
		LED8ADDR = (unsigned char) Symbol[value];
	}
#else
	estado8led = value;
#endif
}

#ifdef SIM
int getState8led() {
	return estado8led;
}
#endif
