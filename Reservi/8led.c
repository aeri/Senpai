/*********************************************************************************************
* Fichero:	8led.c
* Autor:
* Descrip:	Funciones de control del display 8-segmentos
* Version:
*********************************************************************************************/
#define SIM
/*--- ficheros de cabecera ---*/
#include "8led.h"
#ifndef SIM
#include "44b.h"
#include "44blib.h"

/*--- variables globales del m�dulo ---*/
/* tabla de segmentos */
static int
Symbol[size_8led] = { cero, uno, dos, tres, cuatro, cinco, seis, siete, ocho, nueve, A, B, C, D, E, F, blank};

/*--- c�digo de las funciones ---*/
void D8Led_init(void)
{
	/* Estado inicial del display con todos los segmentos iluminados
	   (buscar en los ficheros de cabecera la direccion implicada) */
	LED8ADDR = (unsigned char) cero;
}

void D8Led_symbol(int value)
{
	/* muestra el Symbol[value] en el display (analogo al caso anterior) */
	if ((value >= 0) && (value < size_8led)) {
		LED8ADDR = (unsigned char) Symbol[value];
	}
}
#else
int estado8led = 0;

void cambiarEstado8led(int state) {
	estado8led = state;
}

int getState8led() {
	return estado8led;
}
#endif
