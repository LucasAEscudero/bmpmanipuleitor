#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#define DEBUG 1
#define TEST 1
#include "estructuras.h"
#include <stdio.h>

void solucion(int argc, char* argv[]);

/*lectura y validacions de parametros consola */
t_parametrosConsola* validarParametrosConsola(
  int argc,
  char* argv[],
  t_parametrosConsola* parametros);

int esComandoValido(char* comando);
int esNombreArchivoValido(char* nombre);


/* lectura y validacion del encabesado de la imagen bpm */
void procesarImagen(t_parametrosConsola* parametros);
int  extraerHeder(t_metadata*,FILE*);

/* controlador. ejecuta los comandos */
void generadorManager(t_parametrosConsola* param,t_metadata*h,FILE* pf);
int accionIndexOf(char* comando);

/* comandos */
typedef void(*Accion)(t_metadata*, FILE*);

void negativo(t_metadata*, FILE*);
void escalaDeGrises(t_metadata*, FILE*);
void aumentarContraste(t_metadata*, FILE*);
void reducirContraste(t_metadata*, FILE*);
void tonalidadAzul(t_metadata*, FILE*);
void tonalidadRojo(t_metadata*, FILE*);
void tonalidadVerde(t_metadata*, FILE*);
void recortar(t_metadata*, FILE*);
void rotarDerecha(t_metadata*, FILE*);
void rotarIzquierda(t_metadata*, FILE*);
void comodin(t_metadata*, FILE*);

void reescribirHeader(FILE* ,FILE*);
void aumentarTonalidad(FILE*, FILE*, char[4]);

/*debug*/
void consoleLog(const char* msj);
//void testParametroConsola(t_parametrosConsola* param);

#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED

