typedef enum
{
	calibrar,
    boot,
    instrucciones,
    inicial,
    elegir,
    gameover,
	confirmar
} maquina;

#include "8led.h"
#include "Bmp.h"
#include "botones_antirrebotes.h"
#include "lcd.h"
#include "timer.h"

extern STRU_BITMAP Stru_Bitmap_fGris;

char fila, columna, ready;
int contador;
maquina estado_jugada;
int retardo_parpadeo;
bool mostrando_ficha;
bool finDeLaPartida;
volatile int TS_x;          // Eje de las x de la pantalla
volatile int TS_y;          // Eje de las y de la pantalla
int retardo_boot;  // Tiempo que se muestra el logo del proyecto
int negras;
int blancas;
int retardo_TS;
int veces;
long tiempoInicialTotal;
int retardo_tiempoTotal;
int puntos_calibrar;
int retardo_confirmar;

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

void touchToPixel(int x, int y, int *pixelX, int *pixelY)
{
	*pixelY = (y - media_abajo) / ((media_arriba - media_abajo) / 240);
	*pixelX = (x - media_izquierda) / ((media_derecha - media_izquierda) / 320);
}

void jugada_init()
{
	mostrarCalibrar(1);
    estado_jugada = calibrar;
    botones_antirrebotes_init();
    D8Led_symbol(12 & 0x000f);

    mostrando_ficha = true;
    fila = (char)0;
    columna = (char)0;
    ready = (char)0;
    retardo_boot = 300;  // Para mostrar el logo del proyecto
    finDeLaPartida = false;
    TS_x = -1;
    TS_y = -1;
    retardo_TS = 10;
    retardo_tiempoTotal = 25;
    puntos_calibrar = 4;
    retardo_confirmar = 100; // 2 segundos para confirmar
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
				retardo_TS = 10;
				if(puntos_calibrar > 1)
				{
					switch(puntos_calibrar){
					case 4:
						tope_arriba1 = TS_y;
						tope_izquierda1 = TS_x;
						mostrarCalibrar(2);
						break;
					case 3:
						tope_arriba2 = TS_y;
						tope_derecha1 = TS_x;
						mostrarCalibrar(3);
						break;
					case 2:
						tope_abajo1 = TS_y;
						tope_derecha2 = TS_x;
						mostrarCalibrar(4);
						break;
					}
					puntos_calibrar--;
				}
				else
				{
					tope_abajo2 = TS_y;
					tope_izquierda2 = TS_x;
					TS_x = -1;
					TS_y = -1;
					media_arriba = (tope_arriba1 + tope_arriba2)/2;
					media_abajo = (tope_abajo1 + tope_abajo2)/2;
					media_izquierda = (tope_izquierda1 + tope_izquierda2)/2;
					media_derecha = (tope_derecha1 + tope_derecha2)/2;

					D8Led_symbol(19 & 0x001f);
					estado_jugada = boot;
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
		    tiempoInicialTotal = timer2_leer();

		    estado_jugada = inicial;
		}
	    break;
	case inicial:
	    if(finDeLaPartida)
		{
		    finDeLaPartida = false;
		    mostrarResultado(blancas, negras);
		    D8Led_symbol(0 & 0x000f);
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
		    D8Led_symbol(18 & 0x001f);
		    Lcd_Dma_Trans();
		    retardo_parpadeo = 10;
		    mostrando_ficha = false;
		    retardo_TS = 10;
		    estado_jugada = elegir;
		}
	    break;
	case elegir:
		if(retardo_TS > 0)
		{
			retardo_TS--;
			TS_x = -1;
			TS_y = -1;
		}
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
	    if(TS_x != -1 && retardo_TS == 0)
		{
		    int x, y;
		    touchToPixel(TS_x, TS_y, &x, &y);
		    retardo_TS = 10;
			if(x > 220) // 220 en el eje de las x es el final del tablero
			{
				if(y > 120) // 120 es el punto medio de la pantalla en vertical
				{
					columna = 8; // El 8 se utiliza para cuando el usuario se quiere rendir
					fila = 8;
				}
				else
				{
					columna = 9; // Se pone 9 para indicar que se pasa
					fila = 9;
				}
				ready = 1;
				estado_jugada = inicial;
			}
			else
			{
				LcdClrRect(40, 222, 220, 240, 0);
				Lcd_DspAscII8x16(44, 222, BLACK, "PULSA PARA CANCELAR");
				estado_jugada = confirmar;
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
				estado_jugada = inicial;
				tiempoInicialTotal = timer2_leer();
			}
		}
		break;
	case confirmar:
		if(retardo_parpadeo == 0) // Se queda la ficha parpadeando en la posición seleccionada
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
		if(retardo_TS == 0)
		{
			if(boton != 0 || TS_x != -1)
			{
				retardo_TS = 10;
				retardo_confirmar = 100;
				LcdClrRect(40, 222, 220, 240, 0); // Se elimina el mensaje de cancelar
				Lcd_DspAscII8x16(40, 222, BLACK, "PULSA PARA CONFIRMAR");
				TS_x = -1;
				TS_y = -1;
				estado_jugada = elegir; // Se cancela la operación y se vuelve a elegir
			}
			else
			{
				if(retardo_confirmar == 0)
				{
					retardo_TS = 10;
					ready = 1;
					estado_jugada = inicial;
					LcdClrRect(40, 222, 220, 240, 0); // Se elimina el mensaje de cancelar
					Lcd_DspAscII8x16(40, 222, BLACK, "PULSA PARA CONFIRMAR");
					retardo_confirmar = 100;
				}
				else
				{
					retardo_confirmar--;
				}
			}
		}
		else
		{
			TS_x = -1;
			TS_y = -1;
			retardo_TS--;
		}
		break;
	default:
	    while(1)
		;  // Aqui falla algo...
	}
}
