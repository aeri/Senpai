#include "botones_antirrebotes.h"
#include "button.h"
#include "stdbool.h"
typedef enum { inicio, boton_pressed, leer_boton} maquina_estados;
maquina_estados estado;

volatile unsigned int estado_boton;

int interrupciones_rebotes;
int interrupciones_retardo; // Interrupciones a esperar antes de leer el valor del boton
int retardo_trd;
int retardo2;
int boton;

unsigned int state;

void botones_antirrebotes_init(){
	estado = inicio;
	estado_boton = 0x0;
	boton = 0;
}

void button_callback(int estado){
	estado_boton = estado;
}

int antirrebotes(){
	switch(estado){
	case inicio:
		if(estado_boton != 0x0){
			interrupciones_retardo = 1;
			estado = boton_pressed;
		}
		break;
	case boton_pressed:
		state = button_estado();
		if(interrupciones_retardo == 0){
			if ((state != 0x80) && (state != 0x40)) {	//si no estamos pulsando ningun boton
				estado = inicio; //declaramos como siguiente estado rebotes_bajada
				estado_boton = 0x0;
				button_empezar();
			}

			else if(state == 0x80) { // Se ha pulsado el boton izquierdo
				boton = 1;
				retardo_trd = 1;
				retardo2 = 20;
				estado = leer_boton;
			}
			else{ // Se ha pulsado el boton derecho
				boton = 2;
				retardo_trd = 1;
				retardo2 = 20;
				estado = leer_boton;
			}
		}
		else{
			interrupciones_retardo--;
		}
		break;
	case leer_boton:
		boton = 0;
		state = button_estado();
		if(retardo_trd == 0){
			estado = inicio;
			estado_boton = 0x0;
			button_empezar();
		}
		else{
			if((state != 0x80) && (state != 0x40)){
				retardo_trd--;
			}
			else{
				retardo_trd = 1;
				if(retardo2 == 0){
					if(state == 0x80){
						boton = 1;
					}
					retardo2 = 20;
				}
				else {
					if(state == 0x80){
						retardo2--;
					}
				}
			}
		}
		break;
	default:
		while(1); // Aqui falla algo...
	}
	return boton;
}
