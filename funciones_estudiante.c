/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el �ltimo campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    (Solo para grupos de tres integrantes)
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/
#include <string.h>
#include <stdio.h>
#include "constantes.h"
#include "funciones_estudiante.h"

void solucion(int argc, char* argv[])
{
  t_parametrosConsola param;

  validarParametrosConsola(argc,argv,&param);

  if(param.sinNombreDeArchivo || !param.cantComandos)
    return;

  procesarImagen(&param);
}

////////////////////////////////////////////////////////////////////////////////
//             array de comandos y array funciones por camando
////////////////////////////////////////////////////////////////////////////////
const char comandosValidos[CANTIDAD_COMANDOS][TAMANIO_MAXIMO_COMANDO] =
{
  NEGATIVO,
  ESCALA_DE_GRISES,
  AUMENTAR_CONTRASTE,
  REDUCIR_CONTRASTE,
  TONALIDAD_AZUL,
  TONALIDAD_ROJO,
  TONALIDAD_VERDE,
  RECORTAR,
  ROTAR_DERECHA,
  ROTAR_IZQUIERDA,
  COMODIN

};


Accion vectorAccion[CANTIDAD_COMANDOS] =
{
  &negativo,
  &escalaDeGrises,
  &aumentarContraste,
  &reducirContraste,
  &tonalidadAzul,
  &tonalidadRojo,
  &tonalidadVerde,
  &recortar,
  &rotarDerecha,
  &rotarIzquierda,
  &comodin,
};

////////////////////////////////////////////////////////////////////////////////
// 	funcionalidades a implementar
////////////////////////////////////////////////////////////////////////////////

void negativo(t_metadata* heder, FILE* img)
{
    printf("\n Negativo sin implementar");
}

void escalaDeGrises(t_metadata* heder, FILE* img)
{
  printf("\n Escala de grises sin implementar.\n");
}
void aumentarContraste(t_metadata* heder, FILE* img)
{
  printf("\n Aumentar contraste sin implementar.\n");
}
void reducirContraste(t_metadata* heder, FILE* img)
{
  printf("\n Reducir contraste sin implementar.\n");
}
void tonalidadAzul(t_metadata* heder, FILE* img)
{
  FILE *newImg = fopen("estudiante-tonalidad-azul.bmp", "wb");

  if(newImg == NULL) {
    printf("Error al crear el archivo estudiante-tonalidad-azul.bmp \n");
    return ;
  }

    reescribirHeader(img, newImg);

    aumentarTonalidad(img, newImg, "blue");

    fclose(newImg);
}

void tonalidadRojo(t_metadata* heder, FILE* img)
{
  FILE *newImg = fopen("estudiante-tonalidad-roja.bmp", "wb");

  if(newImg == NULL) {
    printf("Error al crear el archivo estudiante-tonalidad-roja.bmp \n");
    return ;
  }

    reescribirHeader(img, newImg);

    aumentarTonalidad(img, newImg, "red");

    fclose(newImg);
}

void tonalidadVerde(t_metadata* heder, FILE* img)
{
  FILE *newImg = fopen("estudiante-tonalidad-verde.bmp", "wb");

  if(newImg == NULL) {
    printf("Error al crear el archivo estudiante-tonalidad-verde.bmp \n");
    return ;
  }

    reescribirHeader(img, newImg);

    aumentarTonalidad(img, newImg, "green");

    fclose(newImg);
}

void recortar(t_metadata* heder, FILE* img)
{
  printf("\n Recortar sin implementar.\n");
}
void rotarDerecha(t_metadata* heder, FILE* img)
{
  printf("\n Rotar derecha sin implementar.\n");
}
void rotarIzquierda(t_metadata* heder, FILE* img)
{
  printf("\n Rotar izquierda sin implementar.\n");
}
void comodin(t_metadata* heder, FILE* img)
{
  printf("\n Comodin sin implementar.\n");
}


////////////////////////////////////////////////////////////////////////////////
//             Validacion de parametros de entrada por consola
////////////////////////////////////////////////////////////////////////////////
t_parametrosConsola* validarParametrosConsola(
  int argc,
  char* argv[],
  t_parametrosConsola* parametros)
{
  parametros->cantComandos = 0;
  parametros->sinNombreDeArchivo = true;
  parametros->cantErrores = 0;


  int i;
  for(i = 1; i < argc; i++)
  {

    if(esComandoValido(argv[i]))
    {
      parametros->comandos[parametros->cantComandos] = argv[i];
      parametros->cantComandos++;
    }

    else if(
      parametros->sinNombreDeArchivo
      && esNombreArchivoValido(argv[i]))
    {
      parametros->nombreArchivo[0] = argv[i];
      parametros->sinNombreDeArchivo = false;
    }
    else
    {
      parametros->errores[parametros->cantErrores] = argv[i];
      parametros->cantErrores++;
    }
  }

  return parametros;
}

int esComandoValido(char* comando)
{
  int i;

  for(i=0; i<CANTIDAD_COMANDOS; i++)
  {
    if( strcmp(comando,comandosValidos[i])== 0)
      return true;
  }

  return false;
}

int esNombreArchivoValido(char* nombre)
{
  int len = strlen(nombre);

  return len > 4
         && nombre[len-1] == 'p'
         && nombre[len-2] == 'm'
         && nombre[len-3] == 'b'
         && nombre[len-4] == '.' ;
}

////////////////////////////////////////////////////////////////////////////////
//            funciones para el control del flujo de ejecucion
////////////////////////////////////////////////////////////////////////////////

void procesarImagen(t_parametrosConsola* param)
{
  consoleLog("Inicia proceso.....");
  FILE* img;

  img = fopen(param->nombreArchivo[0], "rb");// "wb+"
  if(!img)
  {
    consoleLog("Error abriendo archivo...Fin proceso");
    return;
  }

  t_metadata heder;

  if(!extraerHeder(&heder, img))
  {
    consoleLog("Error al extraer el heder...Fin proceso");
    return ;
  }

  if(img)
    printf("\nSe abrio la imagen");


  generadorManager(param,&heder,img);

  fclose(img);

  consoleLog("\nFin proceso.....");
}

int extraerHeder(t_metadata* h, FILE* pf)
{
  consoleLog("Extraer heder sin implementar \n");

    //comprobar que sea del tipo fichero 'BM'
    if(fgetc(pf) == 'B' && fgetc(pf) == 'M')
        printf("Se detecto el tipo de fichero 'BM' \n");

    fread(&h->tamArchivo, 4, 1, pf);
    printf("Tamanio de archivo: %d bytes\n", h->tamArchivo);

    fseek(pf, 4, SEEK_CUR); //espacio RESERVADO, el cursor se desplaza 4 bytes

    fread(&h->comienzoImagen, 4, 1, pf);
    printf("Comienzo de imagen: %d \n", h->comienzoImagen);

    fread(&h->tamEncabezado, 4, 1, pf);
    printf("Anchura de la cabecera del bitmap: %d \n", h->tamEncabezado);

    fread(&h->ancho, 4, 1, pf);
    printf("Anchura de la imagen: %d \n", h->ancho);

    fread(&h->alto, 4, 1, pf);
    printf("Altura de la imagen: %d \n", h->alto);

    fseek(pf, 28, SEEK_CUR); //datos anteriores a los pixeles, revisar cuales sirven

  return true;
}

void generadorManager(t_parametrosConsola* param, t_metadata*heder, FILE* img)
{
  int i;
  int indiceAccion;

  for(i=0; i < param->cantComandos; i++)
  {
    indiceAccion = accionIndexOf(param->comandos[i]);
    vectorAccion[indiceAccion](heder, img);
  }
}

int accionIndexOf(char* comando)
{
  int indice;

  for(indice=0; indice<CANTIDAD_COMANDOS; indice++)
  {
    if( strcmp(comando,comandosValidos[indice])== 0)
      return indice;
  }

  return -1;
};

////////////////////////////////////////////////////////////////////////////////
//              funciones para modularizar (dividir) codigo
////////////////////////////////////////////////////////////////////////////////
void reescribirHeader(FILE* img, FILE* newImg)
{
    unsigned int aux, i = 0;
    fseek(img, 0, SEEK_SET); //reinicio a la posicion 0 de la imagen

    //copiar header
    while(i < CANT_BYTES_BITMAP_HEADER + 1) {
        fread(&aux, sizeof(aux), 1, img);
        fwrite(&aux, sizeof(aux), 1, newImg);
        i++;
    }
}

void aumentarTonalidad(FILE* img, FILE* newImg, char color[4])
{
    t_pixel pixel;

    while(!feof(img)) {
        fread(&pixel.pixel, 3, 1, img);

        if(!strcmp(color, "blue"))
            pixel.pixel[0] = (pixel.pixel[0] + (pixel.pixel[0] >> 1)) > 255 ? 255 : (pixel.pixel[0] + (pixel.pixel[0] >> 1)); //blue
        else if(!strcmp(color, "green"))
            pixel.pixel[1] = (pixel.pixel[1] + (pixel.pixel[1] >> 1)) > 255 ? 255 : (pixel.pixel[1] + (pixel.pixel[1] >> 1)); //green
        else if(!strcmp(color, "red"))
            pixel.pixel[2] = (pixel.pixel[2] + (pixel.pixel[2] >> 1)) > 255 ? 255 : (pixel.pixel[2] + (pixel.pixel[2] >> 1)); //red

        fwrite(&pixel.pixel, 3, 1, newImg);
    }
}

////////////////////////////////////////////////////////////////////////////////
//                                log
////////////////////////////////////////////////////////////////////////////////
void consoleLog(const char* msj)
{
	printf("%s\n", msj);
};

