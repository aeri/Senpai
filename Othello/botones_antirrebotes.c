#include "botones_antirrebotes.h"

typedef enum { inicio, boton_pressed, leer_boton, boton_soltado } maquina_estados;
maquina_estados estado;

int interrupciones_rebotes;
int interrupciones_retardo; // Interrupciones a esperar antes de leer el valor del boton

int count;

void botones_antirrebotes_init(){
	estado = inicio;
	count = 0;
}

void boton_pulsado(){
	switch(estado){
	case inicio:
		estado = boton_pressed;
		break;
	case boton_pressed:
		break;
	case leer_boton:
		break;
	case boton_soltado:
		break;
	default:
		while(1); // Aqui falla algo...
	}
}

void timer_interruption(){
	unsigned int state;
	switch(estado){
	case inicio:
		interrupciones_retardo = 2; // Se van a esperar 2 interrupciones antes de leer el valor del boton
		break;
	case boton_pressed:
		state = button_estado();
		if(interrupciones_retardo == 0){
			if ((state != 0x80) && (state != 0x40)) {	//si no estamos pulsando ningun boton
				estado = inicio; //declaramos como siguiente estado rebotes_bajada
				button_empezar(Eint4567_ISR);
			}
			else if(state == 0x80) { // Se ha pulsado el boton izquierdo
				count--;
				estado = leer_boton;
			}
			else{ // Se ha pulsado el boton derecho
				count++;
				estado = leer_boton;
			}
		}
		else{
			interrupciones_retardo--;
		}
		break;
	case leer_boton:
		if((state != 0x80) && (state != 0x40)){
			estado = boton_soltado;
			interrupciones_rebotes = 5; // 5 interrupciones de 20ms => 100ms de retardo para rebotes
		}
		break;
	case boton_soltado:
		if(interrupciones_rebotes == 0){
			estado = inicio;
			button_empezar(Eint4567_ISR);
		}
		else{
			interrupciones_rebotes--;
		}
		break;
	default:
		while(1); // Aqui falla algo...
	}
}
