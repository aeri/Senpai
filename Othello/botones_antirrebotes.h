#ifndef _BOTONES_ANTIRREBOTES_H_
#define _BOTONES_ANTIRREBOTES_H_

//#define SIM

#ifndef SIM
#include "button.h"
#endif

void button_callback();

void botones_antirrebotes_init();

void boton_callback();

void boton_pulsado();

int antirrebotes();

int getBoton();

char getReady();

char getFila();

char getColumna();

void resetReady();

#endif
