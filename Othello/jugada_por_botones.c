typedef enum
{
	calibrar,
    boot,
    instrucciones,
    inicial,
    elegir,
    gameover
} maquina;

#include "8led.h"
#include "Bmp.h"
#include "botones_antirrebotes.h"
#include "lcd.h"

char __attribute__ ((aligned (8))) tablero_calibrar1[8][8] = {
	        {1,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0}};

char __attribute__ ((aligned (8))) tablero_calibrar2[8][8] = {
	        {0,0,0,0,0,0,0,1},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0}};

char __attribute__ ((aligned (8))) tablero_calibrar3[8][8] = {
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,1}};

char __attribute__ ((aligned (8))) tablero_calibrar4[8][8] = {
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {0,0,0,0,0,0,0,0},
	        {1,0,0,0,0,0,0,0}};

extern STRU_BITMAP Stru_Bitmap_fGris;

char fila, columna, ready;
int contador;
maquina estado_jugada;
int retardo_parpadeo;
bool mostrando_ficha;
bool finDeLaPartida;
int TS_x;          // Eje de las x de la pantalla
int TS_y;          // Eje de las y de la pantalla
int retardo_boot;  // Tiempo que se muestra el logo del proyecto
int negras;
int blancas;
int frontera;
int retardo_TS;
bool rendido;
int veces;
long tiempoInicialTotal;
int retardo_tiempoTotal;
int puntos_calibrar;

int tope_arriba1, tope_arriba2, tope_abajo1, tope_abajo2, tope_derecha1, tope_derecha2, tope_izquierda1, tope_izquierda2;
int media_arriba, media_abajo, media_izquierda, media_derecha;

void reset_suma()
{
	veces = 0;
}

void suma_callback()
{
	veces++;
}

void mostrar_veces()
{
	mostrarVeces(veces);
}

void TS_callback(int x, int y)
{
    TS_x = x;
    TS_y = y;
}

void avisarFin(int numNegras, int numBlancas)
{
    negras = numNegras;
    blancas = numBlancas;
    finDeLaPartida = true;
}

void jugada_init()
{
	crearTablero();
	mostrarTablero(tablero_calibrar1);
    estado_jugada = calibrar;
    botones_antirrebotes_init();
    D8Led_symbol(15 & 0x000f);
    mostrando_ficha = true;
    fila = (char)0;
    columna = (char)0;
    ready = (char)0;
    retardo_boot = 300;  // 2 segundos mostrando el logo del proyecto
    finDeLaPartida = false;
    rendido = false;
    frontera = 400;  // Línea en el eje de las x que separa el tablero de la parte derecha
    TS_x = -1;
    TS_y = -1;
    retardo_TS = 2;
    retardo_tiempoTotal = 25;
    puntos_calibrar = 4;
}

char getFila()
{
    return fila;
}
char getColumna()
{
    return columna;
}

void resetReady()
{
    ready = (char)0;
}

char getReady()
{
    return ready;
}

void setRendido()
{
	rendido = true;
}

void jugada_botones(char tablero[8][8])
{
    int boton = antirrebotes();

    if(estado_jugada == inicial || estado_jugada == elegir)
    {
    	if(retardo_tiempoTotal == 0)
    	{
			long tiempoActual = timer2_leer();
			mostrarTiempoTotal((tiempoActual - tiempoInicialTotal)/1000000);
			retardo_tiempoTotal = 25;
    	}
    	else{
    		retardo_tiempoTotal--;
    	}
    }

    switch(estado_jugada)
	{
    case calibrar:
    	if(retardo_TS == 0){
			if(TS_x != -1)
			{
				retardo_TS = 2;
				if(puntos_calibrar > 1)
				{
					switch(puntos_calibrar){
					case 4:
						tope_arriba1 = TS_y;
						tope_izquierda1 = TS_x;
						mostrarTablero(tablero_calibrar2);
						break;
					case 3:
						tope_arriba2 = TS_y;
						tope_derecha1 = TS_x;
						mostrarTablero(tablero_calibrar3);
						break;
					case 2:
						tope_abajo1 = TS_y;
						tope_derecha2 = TS_x;
						mostrarTablero(tablero_calibrar4);
						break;
					}
					puntos_calibrar--;
				}
				else
				{
					tope_abajo2 = TS_y;
					tope_izquierda2 = TS_x;
					estado_jugada = boot;
					TS_x = -1;
					TS_y = -1;
					media_arriba = (tope_arriba1 + tope_arriba2)/2;
					media_abajo = (tope_abajo1 + tope_abajo2)/2;
					media_izquierda = (tope_izquierda1 + tope_izquierda2)/2;
					media_derecha = (tope_derecha1 + tope_derecha2)/2;
				}
			}
    	}
		else
		{
			retardo_TS--;
			TS_x = -1;
			TS_y = -1;
		}
    	break;

	case boot:
		if(retardo_boot == 300)
		{
			mostrarLogo(1);
		}
		else if(retardo_boot == 150)
		{
			mostrarLogo(2);
		}

		if(retardo_boot == 0)
		{
		    estado_jugada = instrucciones;
		    mostrarInstrucciones();
		    TS_x = -1;
		    TS_y = -1;
		}
	    else
		{
		    retardo_boot--;
		}
	    break;
	case instrucciones:
	    if(boton != 0 || TS_x != -1)
		{
	    	crearTablero();
	    	mostrarTablero(tablero);
		    estado_jugada = inicial;
		    tiempoInicialTotal = timer2_leer();
		    rendido = false;
		}
	    break;
	case inicial:
	    if(finDeLaPartida)
		{
		    finDeLaPartida = false;
		    mostrarResultado(blancas, negras);
		    estado_jugada = gameover;
		}
	    else
		{
	    	fila = 0;
	    	columna = -1;
	    	bool encontrada = false;
		    while(!encontrada)
			{
				columna++;
				if(columna > 7)
				{
					columna = 0;
					fila++;
				}
				if(tablero[fila][columna] == 0)
				{
					encontrada = true;
				}
			}

		    TS_x = -1;
		    TS_y = -1;
		    D8Led_symbol(6 & 0x000f);
		    BitmapView(22 + (25 * columna), 22 + (25 * fila), Stru_Bitmap_fGris);
		    Lcd_Dma_Trans();
		    retardo_parpadeo = 10;
		    mostrando_ficha = true;
		    estado_jugada = elegir;
		}
	    break;
	case elegir:
	    if(retardo_parpadeo == 0)
		{
		    if(mostrando_ficha)
			{
			    LcdClrRect(22 + (25 * columna), 22 + (25 * fila), 42 + (25 * columna), 42 + (25 * fila), 0);
			}
		    else
			{
			    BitmapView(22 + (25 * columna), 22 + (25 * fila), Stru_Bitmap_fGris);
			}
		    retardo_parpadeo = 10;
		    mostrando_ficha = !mostrando_ficha;
		    Lcd_Dma_Trans();
		}
	    else
		{
		    retardo_parpadeo--;
		}
	    if(boton == 2)
		{
		    LcdClrRect(22 + (25 * columna), 22 + (25 * fila), 42 + (25 * columna), 42 + (25 * fila), 0);

		    bool encontrada = false;
		    while(!encontrada)
			{
			    fila++;
			    if(fila > 7)
				{
				    fila = 0;
				    columna++;
				    if(columna >= 8){
				    	columna = 0;
				    }
				}
			    if(tablero[fila][columna] == 0)
				{
				    encontrada = true;
				}
			}
		    BitmapView(22 + (25 * columna), 22 + (25 * fila), Stru_Bitmap_fGris);
		    Lcd_Dma_Trans();
		}
	    else if(boton == 1)
		{
		    LcdClrRect(22 + (25 * columna), 22 + (25 * fila), 42 + (25 * columna), 42 + (25 * fila), 0);

		    bool encontrada = false;
		    while(!encontrada)
			{
			    columna++;
			    if(columna > 7)
				{
				    columna = 0;
				    fila++;
				    if(fila >= 8){
				    	fila = 0;
				    }
				}
			    if(tablero[fila][columna] == 0)
				{
				    encontrada = true;
				}
			}
		    BitmapView(22 + (25 * columna), 22 + (25 * fila), Stru_Bitmap_fGris);
		    Lcd_Dma_Trans();
		}
	    if(TS_x != -1)
		{
		    ready = 1;
		    int mediaarriba = media_arriba;
		    int mediaabajo = media_abajo;
		    int mediaizquierda = media_izquierda;
		    int mediaderecha = media_derecha;
		    int valorx = TS_x;
		    int valory = TS_y;
		    estado_jugada = inicial;
		    retardo_TS = 2;
		    //if(!(TS_x < media_arriba && TS_x > media_abajo && TS_y < media_derecha && TS_y > media_izquierda))
			if(TS_x > media_derecha)
		    {
			    columna = 8;
			    fila = 8;
			}
		}
	    break;
	case gameover:
		if(retardo_TS == 0){
			TS_x = -1;
			TS_y = -1;
			retardo_TS--;
		}
		else if(retardo_TS > 0)
		{
			retardo_TS--;
		}
		else
		{
			if(boton != 0 || TS_x != -1)
			{
				crearTablero();
				mostrarTablero(tablero);
				rendido = false;
				estado_jugada = inicial;
				tiempoInicialTotal = timer2_leer();
			}
		}
		break;
	default:
	    while(1)
		;  // Aqui falla algo...
	}
}
