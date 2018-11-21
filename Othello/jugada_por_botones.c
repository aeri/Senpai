typedef enum {inicial, eligiendo_fila, eligiendo_columna, elegida_fila, elegida_columna} maquina;

#include "8led.h"
#include "botones_antirrebotes.h"

char fila, columna, ready;
int contador;
maquina estado_jugada;

void jugada_init(){
	estado_jugada = inicial;
	botones_antirrebotes_init();
	D8Led_symbol(15 & 0x000f);
	fila = (char) 0;
	columna = (char) 0;
	ready = (char) 0;
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
void jugada_botones(){
	int boton = antirrebotes();
	switch(estado_jugada){
	case inicial:
		if(boton == 1){
			contador = 0;
			estado_jugada = eligiendo_fila;
			D8Led_symbol(contador & 0x000f);
		}
		break;
	case eligiendo_fila:
		if(boton == 1){
			if (contador == 8){
				contador = 0;
			}
			else{
				contador++;
			}
			int abc = contador;
			D8Led_symbol(contador & 0x000f);
		}
		else if(boton == 2){
			estado_jugada=elegida_fila;
			D8Led_symbol(12 & 0x000f);
			fila = (char) contador;
		}
		break;
	case eligiendo_columna:
		if(boton == 1){
			if (contador == 8){
				contador = 0;
			}
			else{
				contador++;
			}
			D8Led_symbol(contador & 0x000f);
		}
		else if(boton == 2){
			estado_jugada=elegida_columna;
			D8Led_symbol(15 & 0x000f);
			columna = (char) contador;
			ready = (char) 1;
		}
		break;
	case elegida_fila:
		if(boton == 1){
			contador = 0;
			estado_jugada = eligiendo_columna;
			D8Led_symbol(contador & 0x000f);
		}
		break;
	case elegida_columna:
		if(boton == 1){
			contador = 0;
			estado_jugada = eligiendo_fila;
			D8Led_symbol(contador & 0x000f);
		}
		break;
	}
}
