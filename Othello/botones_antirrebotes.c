#include "botones_antirrebotes.h"

//#define SIM
#include "8led.h"
#include "button.h"
#include "stdbool.h"
typedef enum { inicio, boton_pressed, leer_boton} maquina_estados;
maquina_estados estado;

volatile unsigned int estado_boton;

char fila, columna, ready;
int interrupciones_rebotes;
bool elige_fila;
bool elige_columna;
bool eligiendo; // 0, eligiendo fila, 1 eligiendo columna
int interrupciones_retardo; // Interrupciones a esperar antes de leer el valor del boton
int retardo_trd;

static unsigned int count = 0;
unsigned int state;

void botones_antirrebotes_init(){
	estado = inicio;
	estado_boton = 0x0;
	D8Led_symbol(15 & 0x000f);
	elige_fila=true;
	eligiendo = 0;
	ready = (char) 0;
}

void button_callback(int estado){
	estado_boton = estado;
}

char getFila(){
	return fila;
}
char getColumna(){
	return columna;
}

void resetReady(){
	ready = (char) 0;
}

char getReady(){
	return ready;
}

void timer_interruption(){
	switch(estado){
	case inicio:
		if(estado_boton != 0x0){
			interrupciones_retardo = 4;
			estado = boton_pressed;
		}
		break;
	case boton_pressed:
		state = button_estado();
		if(interrupciones_retardo == 0){
			if ((state != 0x80) && (state != 0x40)) {	//si no estamos pulsando ningun boton
				estado = inicio; //declaramos como siguiente estado rebotes_bajada
#ifndef SIM
				estado_boton = 0x0;
				button_empezar();
#endif
			}

			else if(state == 0x80) { // Se ha pulsado el boton izquierdo
				if(elige_fila){
					count = 0;
					elige_fila = false;
				}
				else if (elige_columna){
					count = 0;
					elige_columna = false;
				}
				else{
					if (count == 8){
						count = 0;
					}
					else{
						count++;
					}

				}
#ifndef SIM
				D8Led_symbol(count & 0x000f);
#else
				cambiarEstado8led(count & 0x000f);
#endif
				retardo_trd = 4;
				estado = leer_boton;
			}
			else{ // Se ha pulsado el boton derecho
				if (!eligiendo && !elige_fila && !elige_columna){
					elige_columna=true;
					D8Led_symbol(12 & 0x000f);
					eligiendo = 1;
					fila = (char) count;

				}
				else{
					elige_fila=true;
					D8Led_symbol(15 & 0x000f);
					eligiendo = 0;
					columna = (char) count;
					ready = (char) 1;

				}
				retardo_trd = 4;
				estado = leer_boton;
			}
		}
		else{
			interrupciones_retardo--;
		}
		break;
	case leer_boton:
		state = button_estado();
		if(retardo_trd == 0){
			estado = inicio;
#ifndef SIM
			estado_boton = 0x0;
			button_empezar();
#endif
		}
		else{
			if((state != 0x80) && (state != 0x40)){
				retardo_trd--;
			}
			else{
				retardo_trd = 4;
			}
		}
		break;
	default:
		while(1); // Aqui falla algo...
	}
}
