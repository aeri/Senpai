typedef enum
{
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
    estado_jugada = boot;
    botones_antirrebotes_init();
    D8Led_symbol(15 & 0x000f);
    mostrando_ficha = true;
    fila = (char)0;
    columna = (char)0;
    ready = (char)0;
    retardo_boot = 100;  // 2 segundos mostrando el logo del proyecto
    finDeLaPartida = false;
    frontera = 400;  // Línea en el eje de las x que separa el tablero de la parte derecha
    TS_x = -1;
    TS_y = -1;
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
void jugada_botones(char tablero[8][8])
{
    int boton = antirrebotes();
    switch(estado_jugada)
	{
	case boot:
	    if(retardo_boot == 0)
		{
		    estado_jugada = instrucciones;
		    mostrarInstrucciones();
		}
	    else
		{
		    retardo_boot--;
		}
	    break;
	case instrucciones:
	    if(boton != 0 || TS_x != -1)
		{
		    estado_jugada = inicial;
		}
	    break;
	case inicial:
	    if(finDeLaPartida)
		{
		    finDeLaPartida = false;
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
		    BitmapView(22 + (25 * fila), 22 + (25 * columna), Stru_Bitmap_fGris);
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
	    if(boton == 1)
		{
		    LcdClrRect(22 + (25 * columna), 22 + (25 * fila), 42 + (25 * columna), 42 + (25 * fila), 0);

		    bool encontrada = false;
		    while(!encontrada)
			{
			    fila++;
			    if(fila > 7)
				{
				    fila = 0;
				}
			    if(tablero[fila][columna] == 0)
				{
				    encontrada = true;
				}
			}
		    BitmapView(22 + (25 * columna), 22 + (25 * fila), Stru_Bitmap_fGris);
		    Lcd_Dma_Trans();
		}
	    else if(boton == 2)
		{
		    LcdClrRect(22 + (25 * columna), 22 + (25 * fila), 42 + (25 * columna), 42 + (25 * fila), 0);

		    bool encontrada = false;
		    while(!encontrada)
			{
			    columna++;
			    if(columna > 7)
				{
				    columna = 0;
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
		    estado_jugada = inicial;
		    if(TS_x > frontera)
			{
			    columna = 8;
			    fila = 8;
			}
		}
	    break;
	default:
	    while(1)
		;  // Aqui falla algo...
	}
}
