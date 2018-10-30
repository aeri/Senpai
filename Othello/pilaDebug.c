#include "stdint.h"
#include "pilaDebug.h"

uint32_t* stack = 0x0c7ffffe;
uint32_t* top = 0x0c7fef9c;
uint32_t* pointer;
int words = 0;

void push_debug(uint32_t ID_evento, uint32_t auxData){
	if (words == 0){
		pointer = (uint32_t) stack;
		words = 100;
	}
	words -= 2;
	*pointer = ID_evento;
	pointer--;
	*pointer = auxData;
	pointer--;
}


void lastData(uint32_t *ID_evento, uint32_t *auxData){
	*ID_evento = *(pointer+2);
	*auxData = *(pointer+1);
}
