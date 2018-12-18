//#define SIM
//#define EXCEPT
//#define TEST

#include "led.h"
#include "timer.h"
#include "44blib.h"
#include "44b.h"
#include "fail.h"
#include "8led.h"
#include "button.h"
#include "timer0.h"
#include "jugada_por_botones.h"

#include "tp.h"
#include "lcd.h"

#ifdef SIM
#include "botones_antirrebotes.h"
#endif
//Selector de modo de prueba a modo de juego
//#define PRUEBA
// Tamaño del tablero
enum { DIM=8 };

// Valores que puede devolver la función patron_volteo())
enum {
	NO_HAY_PATRON = 0,
  PATRON_ENCONTRADO = 1
};

// Estados de las casillas del tablero
enum {
CASILLA_VACIA = 0,
FICHA_BLANCA = 1,
FICHA_NEGRA = 2
};



//Número de pruebas a realizar en el test
enum { NP = 5 };


// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
const char  NO              = 0;
const char  SI              = 1;
const char  CASILLA_OCUPADA = 2;

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__ ((aligned (8))) tabla_valor[DIM][DIM] =
{
    {8,2,7,3,3,7,2,8},
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {3,4,5,0,0,5,4,3},
    {3,4,5,0,0,5,4,3},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {8,2,7,3,3,7,2,8}
};


// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
signed const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
signed const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};




#ifdef PRUEBA
char __attribute__ ((aligned (8))) tableros[NP][DIM][DIM] = {
	        {{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA},
	        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	    	{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {FICHA_BLANCA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA}},

	        {{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
	        {FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA}}

	    };

char __attribute__ ((aligned (8))) resultados[NP][DIM][DIM] = {
	        {{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA},
	        {FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}},

	        {{CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {FICHA_BLANCA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA}},

	        {{FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA}}
	    };

char movimientos[NP][2] = {
	{1,4}, {5,3}, {0,0}, {2,1}, {4,4}
};


#endif




//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };

  ////////////////////////////////////////////////////////////////////
     // VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
     // Pregunta: ¿hay que hacer algo con ellas para que esto funcione bien?
     // (por ejemplo añadir alguna palabra clave para garantizar que la sincronización a través de esa variable funcione)
volatile unsigned char fila=0, columna=0, ready = 0;


int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, signed char SF, signed char SC, char color);
extern int patron_volteo_arm_c(char tablero[][8], int *longitud,char f, char c, signed char SF, signed char SC, char color);
extern int patron_volteo_arm_arm(char tablero[][8], int *longitud,char f, char c, signed char SF, signed char SC, char color);
#ifdef EXCEPT
extern void Abort();
extern void Undef();
extern void SWI();
#endif

#ifndef SIM
int patron_volteo_test(char tablero[][DIM], int *longitud, char FA, char CA, signed char SF, signed char SC, char color){
	int longc = *longitud;
	int longarmc = *longitud;
	int longarmarm = *longitud;
	timer2_inicializar();	    // Inicializacion del temporizador
	timer2_empezar();

	volatile unsigned int tiempo_comienzo1 = timer2_leer();
	int respuestac = patron_volteo(tablero, &longc, FA, CA, SF, SC, color);
	volatile unsigned int tiempoc = timer2_leer() - tiempo_comienzo1;

	volatile unsigned int tiempo_comienzo2 = timer2_leer();
	int respuestarmc = patron_volteo_arm_c(tablero, &longarmc, FA, CA, SF, SC, color);
	volatile unsigned int tiempoarmc = timer2_leer() - tiempo_comienzo2;

	volatile unsigned int tiempo_comienzo3 = timer2_leer();
	int respuestarmarm = patron_volteo_arm_arm(tablero, &longarmarm, FA, CA, SF, SC, color);
	volatile unsigned int tiempoarmarm = timer2_parar() - tiempo_comienzo3;
	while (respuestac!=respuestarmc || respuestarmc!=respuestarmarm || longc!=longarmc || longarmc!=longarmarm){/*La respuesta no es correcta*/}
	*longitud=longc;
	return respuestac;
}
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM])
{
    int i, j;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
	for(j = 3; j < 5; ++j) {
	    tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
	}
    }

    for (i = 2; i < 6; ++i) {
	for (j = 2; j < 6; ++j) {
	    if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
		candidatas[i][j] = CASILLA_OCUPADA;
	    } else {
		candidatas[i][j] = SI; //CASILLA_LIBRE;
	    }
	}
    }
#endif
    // arriba hay versión alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionará.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(volatile unsigned char *ready)
{
    while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida)
{
    char ficha;

    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que algún índice puede ser negativo

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, signed char SF, signed char SC, char color)
{
	suma_callback();
    int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
    int patron; //indica si se ha encontrado un patrón o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla está en el tablero, no está vacía,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color))
    {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
        return patron;
    }
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
    else if ((posicion_valida == 1) && (casilla == color))
    {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
            {
            return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
            //printf("PATRON_ENCONTRADO \n");
            }
        else {
            return NO_HAY_PATRON; // si no hay que voltear no hay patrón
            //printf("NO_HAY_PATRON \n");
            }
    }
    // en caso contrario es que no hay patrón
    else
    {
        return NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }
}
////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, signed char SF, signed char SC, int n, char color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}
////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(char tablero[][DIM], char f, char c, char color)
{
	signed char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;
    int haVolteado = 0;
    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
        	haVolteado = 1;
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return haVolteado;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], unsigned char *f, unsigned char *c)
{
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    signed char SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos todo el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {   // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM))
                    {
                        SF = vSF[k];    // k representa la dirección que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO)
                        {
                            found = 1;
                            if (tabla_valor[i][j] > mejor)
                            {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (char) mf;
    *c = (char) mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
void contar(char tablero[][DIM], int *b, int *n)
{
    int i,j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {
            if (tablero[i][j] == FICHA_BLANCA)
            {
                (*b)++;
            }
            else if (tablero[i][j] == FICHA_NEGRA)
            {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(char candidatas[][DIM], char f, char c)
{
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor sí
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f-1][c] != CASILLA_OCUPADA)
            candidatas[f-1][c] = SI;

        if ((c > 0) && (candidatas[f-1][c-1] != CASILLA_OCUPADA))
            candidatas[f-1][c-1] = SI;

        if ((c < 7) && (candidatas[f-1][c+1] != CASILLA_OCUPADA))
            candidatas[f-1][c+1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f+1][c] != CASILLA_OCUPADA)
            candidatas[f+1][c] = SI;

        if ((c > 0) && (candidatas[f+1][c-1] != CASILLA_OCUPADA))
            candidatas[f+1][c-1] = SI;

        if ((c < 7) && (candidatas[f+1][c+1] != CASILLA_OCUPADA))
            candidatas[f+1][c+1] = SI;
    }
    if ((c > 0) && (candidatas[f][c-1] != CASILLA_OCUPADA))
        candidatas[f][c-1] = SI;

    if ((c < 7) && (candidatas[f][c+1] != CASILLA_OCUPADA))
        candidatas[f][c+1] = SI;
}




////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la señal de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deberían ser locales de reversi8,
// la razón es que al meterlas en la pila no las pone juntas, y así jugar es más complicado.
// en esta versión el humano lleva negras y la máquina blancas
// no se comprueba que el humano mueva correctamente.
// Sólo que la máquina realice un movimiento correcto.
void reversi8()
{



	/*Delay (45);
	volatile int tiempo = timer2_leer();*/


	//Iterador para cargar y comprobar tableros de forma sucesiva
	int i = 0;
	 ////////////////////////////////////////////////////////////////////
	 // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
	// sólo se exploran las que están alrededor de las fichas colocadas
	 ////////////////////////////////////////////////////////////////////
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
    {
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO}
    };


    int done;     // la máquina ha conseguido mover o no
    int move = 0; // el humano ha conseguido mover o no
    int blancas, negras; // número de fichas de cada color
    int fin = 0;  // fin vale 1 si el humano no ha podido mover
                  // (ha introducido un valor de movimiento con algún 8)
                  // y luego la máquina tampoco puede
    unsigned char f, c;    // fila y columna elegidas por la máquina para su movimiento

    init_table(tablero, candidatas);
    while (fin == 0)
    {
        move = 0;
#ifdef PRUEBA

        	a = 0;
        	b = 0;
			while (a < DIM){
				while (b < DIM){
					tablero[a][b] = tableros [i][a][b];
					b++;
				}
				b = 0;
				a++;
			}
        	fila=movimientos[i][0];
        	columna=movimientos[i][1];
        	ready = 1;

#endif
        esperar_mov(&ready);
        // si la fila o columna son 8 asumimos que el jugador no puede mover
        if (((fila) != DIM) && ((columna) != DIM))
        {

            tablero[fila][columna] = FICHA_NEGRA;
            actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
            actualizar_candidatas(candidatas, fila, columna);
            move = 1;
        }
#ifdef PRUEBA

        	j = 0;
        	k = 0;
        	while (j < DIM){
        		while (k < DIM){
        			if (tablero[j][k] != resultados [i][j][k]){
        				//El resultado no coincide, bucle para depurar fallos
        				while (1);
        			}
        			k++;
        		}
        		k = 0;
        		j++;
        	}
        	i++;
        	if (i == NP){
        		fin = 1;
        	}

#else

        		// escribe el movimiento en las variables globales fila columna
				done = elegir_mov(candidatas, tablero, &f, &c);
				if (done == -1)
				{
					if (move == 0)
						fin = 1;
				}
				else
				{
					tablero[f][c] = FICHA_BLANCA;
					actualizar_tablero(tablero, f, c, FICHA_BLANCA);
					actualizar_candidatas(candidatas, f, c);
				}

#endif
		contar(tablero, &blancas, &negras);
    }
}

void reversi_main() {
#ifndef SIM
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se ver� reflejado en el 8led
	D8Led_init();       // inicializamos el 8led
	timer2_inicializar();	    // Inicializacion del temporizador
	timer_init(); //Iniciar el timer0
	timer2_empezar();
	Lcd_Init();
	TS_init();
	__asm__(
			"mrs r0,cpsr"
			"bic r0,r0,#MODEMASK"
			"orr r1,r0,#USERMODE"
			"msr cpsr_cxsf,r1"
			"ldr sp,=UserStack"

	);

#endif
#ifdef TEST
	volatile int anterior = -2;
	volatile int actual = -1;
	volatile int interrupciones = 0;
	unsigned int tiempo = timer2_leer();
	while(1){
		anterior = -2;
		actual = -1;
		while(anterior != actual){
			if(timer2_leer() - tiempo >= 100000){
				tiempo = timer2_leer();
				if(leer_tiempo() != -1){
					anterior = actual;
					actual = leer_tiempo();
					interrupciones = leer_interrupciones();
				}
			}
		}
		boton_reset_test(); // Poner breakpoint aquí para mirar el retardo tras cada pulsación
	}
#endif
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
	    {
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO},
	        {NO,NO,NO,NO,NO,NO,NO,NO}
	    };


	int done;     // la máquina ha conseguido mover o no
	int move = 0; // el humano ha conseguido mover o no
	int blancas, negras; // número de fichas de cada color

	unsigned char f, c;    // fila y columna elegidas por la máquina para su movimiento

#ifdef SIM
	unsigned volatile int timer_int, button_state;
	volatile int led8_state;
	button_state = 0;
	timer_int = 0;
#endif
#ifdef EXCEPT
	excepciones_inicializar();
	Undef();
#endif
	jugada_init();
	while (1){
		int fin = 0;  // fin vale 1 si el humano no ha podido mover
						// (ha introducido un valor de movimiento con algún 8)
						// y luego la máquina tampoco puede
		init_table(tablero, candidatas);
		while(fin == 0){
#ifdef SIM
			if(timer_int == 1){
				timer_int = 0;
				if (button_state != 0x0){
					button_callback(button_state);
				}
				cambiar_estado(button_state);
				jugada_botones();
				led8_state = getState8led();
#else
			if(interrupcionesTimer() == 1){
				resetTimer();
				jugada_botones(tablero);
#endif
				ready = getReady();
				if(ready == 1){
					resetReady();
					fila = getFila();
					columna = getColumna();
					long tiempoInicial = timer2_leer();
					// si la fila o columna son 8 asumimos que el jugador no puede mover
					reset_suma();
					if (((fila) != DIM) && ((columna) != DIM))
					{

						tablero[fila][columna] = FICHA_NEGRA;

						if(actualizar_tablero(tablero, fila, columna, FICHA_NEGRA) == 0){
							tablero[fila][columna] = CASILLA_VACIA;
							setRendido();
							fin = 1;
						}
						else{
							actualizar_candidatas(candidatas, fila, columna);
							move = 1;
						}
					}
					else
					{
						setRendido();
						fin = 1;
					}
					long tiempoFinal = timer2_leer();
					mostrarTiempoJugada(tiempoFinal - tiempoInicial);
					mostrar_veces();

					// escribe el movimiento en las variables globales fila columna
					done = elegir_mov(candidatas, tablero, &f, &c);
					if (done == -1)
					{
						if (move == 0)
							fin = 1;
					}
					else
					{
						tablero[f][c] = FICHA_BLANCA;
						actualizar_tablero(tablero, f, c, FICHA_BLANCA);
						actualizar_candidatas(candidatas, f, c);
					}
					contar(tablero, &blancas, &negras);
					if(fin == 1){
						avisarFin(negras, blancas);
					}
					else{
						mostrarTablero(tablero);
					}
				}
			}
		}
	}
}
