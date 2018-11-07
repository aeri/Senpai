#include "botones_antirrebotes.h"

//#define SIM
#include "8led.h"
#include "button.h"
typedef enum { inicio, boton_pressed, leer_boton} maquina_estados;
maquina_estados estado;

volatile unsigned int estado_boton;

int interrupciones_rebotes;
int interrupciones_retardo; // Interrupciones a esperar antes de leer el valor del boton
int retardo_trd;

static unsigned int count = 0;

void botones_antirrebotes_init(){
	estado = inicio;
	estado_boton = 0x0;
}

void button_callback(int estado){
	estado_boton = estado;
}
void timer_interruption(){
	unsigned int state = button_estado();
	switch(estado){
	case inicio:
		if(estado_boton != 0x0){
			estado = boton_pressed;
			interrupciones_retardo = 2;
		}
		break;
	case boton_pressed:
		if(interrupciones_retardo == 0){
			if ((state != 0x80) && (state != 0x40)) {	//si no estamos pulsando ningun boton
				estado = inicio; //declaramos como siguiente estado rebotes_bajada
#ifndef SIM
				estado_boton = 0x0;
				button_empezar();
#endif
			}

			else if(state == 0x80) { // Se ha pulsado el boton izquierdo
				count--;
#ifndef SIM
				D8Led_symbol(count & 0x000f);
#else
				cambiarEstado8led(count & 0x000f);
#endif
				estado = leer_boton;
			}
			else{ // Se ha pulsado el boton derecho
				count++;
#ifndef SIM
				D8Led_symbol(count & 0x000f);
#else
				cambiarEstado8led(count & 0x000f);
#endif
				estado = leer_boton;
			}
		}
		else{
			interrupciones_retardo--;
		}
		retardo_trd = 2;
		break;
	case leer_boton:

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
				retardo_trd = 2;
			}
		}
		break;

	/*case boton_soltado:
		if(interrupciones_rebotes == 0){
			estado = inicio;
			button_empezar(boton_callback);
		}
		else{
			interrupciones_rebotes--;
		}
		break;*/
	default:
		while(1); // Aqui falla algo...
	}
}
