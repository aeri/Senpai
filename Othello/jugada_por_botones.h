#ifndef _BOTONES_ANTIRREBOTES_H_
#define _BOTONES_ANTIRREBOTES_H_

//#define SIM

#ifndef SIM
#include "button.h"
#endif

char getReady();

char getFila();

char getColumna();

void resetReady();

#endif