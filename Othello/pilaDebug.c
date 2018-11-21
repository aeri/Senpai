#include "stdint.h"
#include "pilaDebug.h"

#define stack 0xC7FEFFC;
uint32_t* pointer;
int words = 0;

void push_debug(uint8_t ID_evento, uint32_t auxData){
	if (words == 0){
		pointer = (uint32_t *) stack;
		words = 10;
	}
	uint32_t evento = ((uint32_t) ID_evento) << 24;
	auxData &= 0x00ffffff;
	evento = auxData | evento;
	words -= 1;
	*pointer = evento;
	pointer = pointer - 1;
}


void lastData(uint32_t *ID_evento, uint32_t *auxData){
	*ID_evento = *(pointer+2);
	*auxData = *(pointer+1);
}
