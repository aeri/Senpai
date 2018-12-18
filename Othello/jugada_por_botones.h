#ifndef _JUGADA_POR_BOTONES_H_
#define _JUGADA_POR_BOTONES_H_

char getReady();

char getFila();

char getColumna();

void resetReady();

void jugada_botones();

void jugada_init();

void TS_callback(int x, int y);

void avisarFin(int numNegras, int numBlancas);

void setRendido();

void reset_suma();

void suma_callback();

void mostrar_veces();

#endif
