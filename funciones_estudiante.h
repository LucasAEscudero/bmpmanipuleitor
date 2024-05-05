#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

#include "estructuras.h"
#include <stdio.h>

#define true 1
#define false 0

#define CANTIDAD_COMANDOS 11
#define TAMANIO_MAXIMO_COMANDO 25
#define MAX_ERRORES_GUARDADOS 11

#define NEGATIVO "--negativo"
#define ESCALA_DE_GRISES "--escala-de-grises"
#define AUMENTAR_CONTRASTE "--aumentar-contraste"
#define REDUCIR_CONTRASTE "--reducir-contraste"
#define TONALIDAD_AZUL "--tonalidad-azul"
#define TONALIDAD_VERDE "--tonalidad-verde"
#define TONALIDAD_ROJO "--tonalidad-roja"
#define RECORTAR "--recortar"
#define ROTAR_DERECHA "--rotar-derecha"
#define ROTAR_IZQUIERDA "--rotar-izquierda"
#define COMODIN "--comodin"
#define HELP "--help"

int solucion(int argc, char* argv[]);


/** MODELO **/
typedef struct
{
  char* comandos[CANTIDAD_COMANDOS];
  int cantComandos;
  char* nombreArchivo[1];
  int sinNombreDeArchivo;
  char* errores[MAX_ERRORES_GUARDADOS];
  int cantErrores;
  int esHelp;
} t_parametrosConsola;


/** VISTA **/
/* Validar parametro pasados por el usuario
   Mostrar Ayuda
   Mostrar errores.
 */
t_parametrosConsola* extraerParametrosConsola(
  int argc,
  char* argv[],
  t_parametrosConsola* parametros);

int esComandoValido(char* comando);
int esNombreArchivoValido(char* nombre);

void mostrarErrores(t_parametrosConsola* param);

void mostrarAyuda();

/** CONTROLADOR **/
/* lectura y validacion del encabesado de la imagen bpm */
int procesarImagen(t_parametrosConsola* parametros);
int esTipoArchivoBMP(FILE* img);
int esTamnioPixel24bist(t_metadata* heder, FILE* img);
int extraerHeder(t_metadata*,FILE*);

/* controlador. ejecuta los comandos */
int generadorManager(t_parametrosConsola* param, t_metadata*h, FILE* pf);
int accionIndexOf(char* comando);

/* handle comando */
typedef int(*Accion)(t_metadata*, FILE*);

int negativo(t_metadata* heder, FILE* img);
int escalaDeGrises(t_metadata*, FILE*);
int aumentarContraste(t_metadata*, FILE*);
int reducirContraste(t_metadata*, FILE*);
int tonalidadAzul(t_metadata*, FILE*);
int tonalidadRojo(t_metadata*, FILE*);
int tonalidadVerde(t_metadata*, FILE*);
int recortar(t_metadata*, FILE*);
int rotarDerecha(t_metadata*, FILE*);
int rotarIzquierda(t_metadata*, FILE*);
int comodin(t_metadata*, FILE*);

void copiarHeder(t_metadata* heder,FILE* img, FILE* newImg);
unsigned char duplicarValorColor(unsigned char color);


/*debug*/
void logString(const char* msj);
//void testParametroConsola(t_parametrosConsola* param);

#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
