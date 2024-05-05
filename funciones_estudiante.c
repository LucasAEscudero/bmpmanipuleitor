/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
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
    (Sólo para grupos de tres integrantes)
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"
#include "funciones_estudiante.h"

#define TAM_MAXIMO_HEDER_BMP  256
#define PIXEL_24  3l
#define RED 2
#define GREEN 1
#define BLUE 0

#define LOG 1

/*
#define TODO_OK 0
#define ARCHIVO_NO_ENCONTRADO 10
#define NO_SE_PUEDE_CREAR_ARCHIVO 20
*/

#define SIN_NOMBRE_DE_ARCHIVO 30
#define SIN_COMANDOS 40
#define FORMATO_DE_ARCHIVO_INVALIDO 50
#define INICIO_ARCHIVO 0l


/** MODELO **/

////////////////////////////////////////////////////////////////////////////////
//             array comando y array funciones por camando
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
    &comodin
};

////////////////////////////////////////////////////////////////////////////////

/** VISTA **/

int solucion(int argc, char* argv[])
{
    t_parametrosConsola param;

    extraerParametrosConsola(argc,argv,&param);

    if(param.esHelp)
        mostrarAyuda();

    if(param.cantErrores > 0)
        mostrarErrores(&param);

    if(param.sinNombreDeArchivo)
        return SIN_NOMBRE_DE_ARCHIVO ;

    if(param.cantComandos == 0)
        return SIN_COMANDOS ;

    return procesarImagen(&param);
}

void mostrarErrores(t_parametrosConsola* param)
{
    printf("\n Argumentos erroneos.\n");
    printf("  Comandos:\n");
    int indice;
    for(indice=0; indice<param->cantErrores; indice++)
    {
        printf("   %s\n", param->errores[indice]);
    }
}

void mostrarAyuda()
{
    printf("\n Manipulador de imagens bmp.\n");
    printf("  Comandos:\n");
    int indice;
    for(indice=0; indice<CANTIDAD_COMANDOS; indice++)
    {
        printf("   %s\n", comandosValidos[indice]);
    }
}


////////////////////////////////////////////////////////////////////////////////
///       Servicio de Validacion de parametros de entrada por consola
////////////////////////////////////////////////////////////////////////////////

t_parametrosConsola* extraerParametrosConsola(
    int argc,
    char* argv[],
    t_parametrosConsola* parametros)
{
    parametros->cantComandos = 0;
    parametros->sinNombreDeArchivo = true;
    parametros->cantErrores = 0;
    parametros->esHelp = false;

    int i;

    for(i = 1; i < argc; i++)
    {
        if(!parametros->esHelp && strcmp(HELP,argv[i]) == 0)
        {
            parametros->esHelp = true;
            continue;
        }

        if( parametros->sinNombreDeArchivo && esNombreArchivoValido(argv[i]))
        {
            parametros->nombreArchivo[0] = argv[i];
            parametros->sinNombreDeArchivo = false;
            continue;
        }

        if( parametros->cantComandos < CANTIDAD_COMANDOS && esComandoValido(argv[i]))
        {
            parametros->comandos[parametros->cantComandos] = argv[i];
            parametros->cantComandos++;
            continue;
        }
        if( parametros->cantErrores < MAX_ERRORES_GUARDADOS)
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
        if(strcmp(comando,comandosValidos[i])== 0)
            return true;
    }
    return false;
}

int esNombreArchivoValido(char* nombre)
{
    int len = strlen(nombre);

    return strlen(nombre) > 4 &&
           nombre[len-1] == 'p' &&
           nombre[len-2] == 'm' &&
           nombre[len-3] == 'b' &&
           nombre[len-4] == '.' ;
}

/** CONTROLADOR **/

////////////////////////////////////////////////////////////////////////////////
///       Servicio validacion formato del archivo y extraccion de metadata
////////////////////////////////////////////////////////////////////////////////
int esTipoArchivoBMP(FILE* img)
{
    assert(img);

    char tipo[2];
    fseek(img,0l,SEEK_SET);
    fread(tipo,2l,1,img);

    return tipo[0] =='B' && tipo[1] =='M';
}

int extraerHeder(t_metadata* heder,FILE* img)
{
    assert(img && heder);
    logString(" Extrayendo cebacera... \n");

    fseek(img, 0l, SEEK_END);
    if( ftell(img) < TAM_MAXIMO_HEDER_BMP)
        return false;

    // tipo 2 byts
    fseek(img,2l,SEEK_SET);
    // tamaño archivo
    fread(&heder->tamArchivo,sizeof(int),1l,img);
    // reservado
    fseek(img,sizeof(int),SEEK_CUR);
    // inicio datos imagen
    fread(&heder->comienzoImagen,sizeof(int),1l,img);
    // tamaño cabecera bitmap
    fread(&heder->tamEncabezado,sizeof(int),1l,img);
    // anchura
    fread(&heder->ancho,sizeof(int),1l,img);
    // altura
    fread(&heder->alto,sizeof(int),1l,img);
    // numero de planos
    fread(&heder->profundidad,sizeof(short),1l,img);

    //mostrarHeder(heder);
    return true;
}

int esTamnioPixel24bist(t_metadata* heder, FILE* img)
{
    assert(img && heder);

    fseek(img,heder->comienzoImagen,SEEK_SET);

    unsigned long bytesInicioImg = ftell(img);

    fseek(img,0l,SEEK_END);

    unsigned long bytesArchivo = ftell(img);


    int esTamanioCorrecto =
        bytesArchivo - bytesInicioImg == heder->alto * heder->ancho * PIXEL_24;

    return esTamanioCorrecto;
}

////////////////////////////////////////////////////////////////////////////////
///            Controlador del proceso de generacion de imagenes
////////////////////////////////////////////////////////////////////////////////
int procesarImagen(t_parametrosConsola* param)
{
    logString("\nInicia proceso.....\n");

    FILE* img;
    img = fopen(param->nombreArchivo[0], "rb");

    if(!img)
    {
        logString("Error abriendo archivo...Fin proceso.");
        return ARCHIVO_NO_ENCONTRADO; // ERROR_ABRIENDO_ARCHIVO
    }

    if(!esTipoArchivoBMP(img))
    {
        logString("Error tipo de archivo incorrecto...Fin proceso.");
        return FORMATO_DE_ARCHIVO_INVALIDO; // ERROR_TIPO_ARCHIVO
    }

    t_metadata heder;

    if(!extraerHeder(&heder, img))
    {
        logString("Error al extraer el heder...Fin proceso.");
        return FORMATO_DE_ARCHIVO_INVALIDO; // ERROR_TIPO_ARCHIVO
    }
    if(!esTamnioPixel24bist(&heder, img))
    {
        logString("Error tipo de archivo no es pixel 24 bits...Fin proceso.");
        return  FORMATO_DE_ARCHIVO_INVALIDO; // ERROR_TIPO_ARCHIVO
    }

    int erroresALGenerarArchivos = generadorManager(param,&heder,img);

    fclose(img);

    logString("\nFin proceso.");

    return !erroresALGenerarArchivos ? TODO_OK : erroresALGenerarArchivos;
}

/// generador de archivos tranformados
int generadorManager(t_parametrosConsola* param, t_metadata*heder, FILE* img)
{
    int i;
    int indiceAccion;
    int error = 0;

    for(i=0; i<param->cantComandos; i++)
    {
        indiceAccion = accionIndexOf(param->comandos[i]);
        if(indiceAccion != -1)
            error = vectorAccion[indiceAccion](heder, img);
        else
            logString("no existe accion para este comando.");
    }

    if(error)
        return error;
    return TODO_OK;
}

int accionIndexOf(char* comando)
{
    int indice;
    for(indice=0; indice < CANTIDAD_COMANDOS; indice++)
    {
        if(strcmp(comando,comandosValidos[indice])== 0)
            return indice;
    }
    return -1;
};

////////////////////////////////////////////////////////////////////////////////
/// 	generadores de archivo transformado
////////////////////////////////////////////////////////////////////////////////
int negativo(t_metadata* heder, FILE* img)
{
    logString("\n Negativo ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_negativo.bmp","w+b");
    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo 'estudiante_negativo.bmp'");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img,imgGenerada);

    t_pixel pixel;

    while(fread(&pixel,PIXEL_24,1l,img)>0)
    {
        pixel.pixel[RED] = ~pixel.pixel[RED];
        pixel.pixel[GREEN] = ~pixel.pixel[GREEN];
        pixel.pixel[BLUE] = ~pixel.pixel[BLUE];

        fwrite(&pixel,PIXEL_24,1l,imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Negativo finalizado.\n ");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
int escalaDeGrises(t_metadata* heder, FILE* img)
{
    logString("\n Escala de grises ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_escala-de-grises.bmp","w+b");
    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_escala-de-grises.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img,imgGenerada);

    t_pixel pixel;
    unsigned int promedioPixel ;

    while(fread(&pixel,PIXEL_24,1l,img)>0)
    {
        promedioPixel =
            (pixel.pixel[RED]+pixel.pixel[GREEN]+pixel.pixel[BLUE]) / 3;////////////////////////////////////////////////

        pixel.pixel[RED] = (unsigned char)promedioPixel;
        pixel.pixel[GREEN] = (unsigned char)promedioPixel;
        pixel.pixel[BLUE] = (unsigned char)promedioPixel;

        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Escala de grises finalizado.\n ");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
unsigned char incrementoPixelAl25(unsigned char pixel)
{
    unsigned int incrementoPixel = pixel + (pixel >> 2);
    return  incrementoPixel < 256 ? incrementoPixel : 255;
}

int aumentarContraste(t_metadata* heder, FILE* img)
{
    logString("\n Aumentar contraste ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_aumentar-contraste.bmp","w+b");
    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_aumentar-contraste.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img,imgGenerada);

    t_pixel pixel;
    unsigned int condicion;

    while(fread(&pixel,PIXEL_24,1l,img)>0)
    {
        condicion = (pixel.pixel[RED] + pixel.pixel[GREEN] + pixel.pixel[BLUE] / 3) > 127;

        if(condicion)
        {
            pixel.pixel[RED] = incrementoPixelAl25(pixel.pixel[RED]);
            pixel.pixel[GREEN] = incrementoPixelAl25(pixel.pixel[GREEN]);
            pixel.pixel[BLUE] = incrementoPixelAl25(pixel.pixel[BLUE]);
        }
        else
        {
            pixel.pixel[RED] = pixel.pixel[RED] - (pixel.pixel[RED] >> 2);
            pixel.pixel[GREEN] = pixel.pixel[GREEN] - (pixel.pixel[GREEN] >> 2);
            pixel.pixel[BLUE] = pixel.pixel[BLUE] - (pixel.pixel[BLUE] >> 2);
        }

        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Aumentar contraste finalizada.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
int reducirContraste(t_metadata* heder, FILE* img)
{
    logString("\n Reducir contraste ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_reducir-contraste.bmp","w+b");
    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_reducir-contraste.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img,imgGenerada);

    t_pixel pixel;
    unsigned int condicion;

    while(fread(&pixel,PIXEL_24,1l,img) > 0)
    {
        condicion = (pixel.pixel[RED] + pixel.pixel[GREEN] + pixel.pixel[BLUE] / 3) > 127;

        if(!condicion)
        {
            pixel.pixel[RED] = incrementoPixelAl25(pixel.pixel[RED]);
            pixel.pixel[GREEN] = incrementoPixelAl25(pixel.pixel[GREEN]);
            pixel.pixel[BLUE] = incrementoPixelAl25(pixel.pixel[BLUE]);
        }
        else
        {
            pixel.pixel[RED] = pixel.pixel[RED] - (pixel.pixel[RED] >> 2);
            pixel.pixel[GREEN] = pixel.pixel[GREEN] - (pixel.pixel[GREEN] >> 2);
            pixel.pixel[BLUE] = pixel.pixel[BLUE] - (pixel.pixel[BLUE] >> 2);
        }

        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }

    fclose(imgGenerada);

    logString(" Reducir contraste finalizado.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
int tonalidadAzul(t_metadata* heder, FILE* img)
{
    logString("\n Tonalidad azul ejecutando.");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_tonalidad-azul.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_tonalidad-azul.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img,imgGenerada);

    t_pixel pixel;

    while(fread(&pixel, PIXEL_24, 1l, img) > 0)
    {
        pixel.pixel[BLUE] = duplicarValorColor(pixel.pixel[BLUE]);
        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Tonalidad azul finalizado.\n");
    return TODO_OK;
}

int tonalidadRojo(t_metadata* heder, FILE* img)
{
    logString("\n Tonalidad rojo ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_tonalidad-rojo.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_tonalidad-rojo.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img, imgGenerada);

    t_pixel pixel;

    while(fread(&pixel, PIXEL_24, 1l, img)>0)
    {
        pixel.pixel[RED] = duplicarValorColor(pixel.pixel[RED]);

        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Tonalidad rojo finalizado.\n");
    return TODO_OK;
}

int tonalidadVerde(t_metadata* heder, FILE* img)
{
    logString("\n Tonalidad verde ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_tonalidad-verde.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_tonalidad-verde.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img, imgGenerada);

    t_pixel pixel;

    while(fread(&pixel, PIXEL_24, 1l, img) > 0)
    {
        pixel.pixel[GREEN] = duplicarValorColor(pixel.pixel[GREEN]);
        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Tonalidad verde finalizado.\n");
    return TODO_OK;
}

unsigned char duplicarValorColor(unsigned char color)
{
    unsigned int colorDuplicado = color << 1 ;
    return colorDuplicado < 255 ? (unsigned char)colorDuplicado : 255;
}
////////////////////////////////////////////////////////////////////////////////
int recortar(t_metadata* heder, FILE* img)
{
    logString("\n Recortar ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_recortar.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_recortar.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img, imgGenerada);

    unsigned int countPixelX = 1;
    unsigned int countFilaY = 1;
    unsigned int maxEjeX = heder->ancho >> 1;
    unsigned int maxEjeY = heder->alto >> 1;

    t_pixel pixel;
    t_pixel pixelBlanco;
    pixelBlanco.pixel[0]=0xff;
    pixelBlanco.pixel[1]=0xff;
    pixelBlanco.pixel[2]=0xff;

    while(fread(&pixel, PIXEL_24, 1l, img) > 0)
    {
        if(countPixelX > maxEjeX ||  countFilaY < maxEjeY)
        {
            if(countPixelX == heder->ancho )
            {
                countPixelX = 0;
                countFilaY++;
            }
            fwrite(&pixelBlanco, PIXEL_24, 1l, imgGenerada);
        }
        else
        {
            fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
        }
        countPixelX++;
    }

    //fseek(img, 0l, SEEK_SET) ;
    //fseek(imgGenerada, 18l, SEEK_SET) ;
    //fwrite(&maxEjeX, 4l, 1l, imgGenerada);
    //fwrite(&maxEjeY, 4l, 1l, imgGenerada);

    fclose(imgGenerada);
    logString(" Recortar finalizado.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
long int calculoOffsetRotarDerecha(int fila, int columna, int alto, int ancho)
{
    int newFila = ancho - 1 - columna;
    int newColumna = fila;
    int offset = newFila * alto + newColumna;

    return offset;
}

int rotarDerecha(t_metadata* heder, FILE* img)
{
    logString("\n Rotar derecha ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_rotar-derecha.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_rotar-derecha.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder, img, imgGenerada);

    int fila = 0;
    int columna = 0;
    long int offset;

    t_pixel pixel;

    while(fread(&pixel,PIXEL_24,1,img) > 0)
    {
        offset = calculoOffsetRotarDerecha(fila,columna,heder->alto,heder->ancho);
        fseek(imgGenerada, offset*3 + heder->comienzoImagen, SEEK_SET);
        fwrite(&pixel,PIXEL_24,1,imgGenerada);

        columna++;
        if(columna == heder->ancho)
        {
            fila++;
            columna = 0;
        }
    }

    fseek(imgGenerada, 18l, SEEK_SET) ;
    fwrite(&heder->alto, 4l, 1l, imgGenerada);
    fwrite(&heder->ancho, 4l, 1l, imgGenerada);

    fclose(imgGenerada);

    logString(" Rotar derecha finalizado.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
long int calculoOffsetRotarIzquierda(int fila, int columna, int alto, int ancho)
{
    int newFila = columna;
    int newColumna = alto - (fila+1);
    int offset = newFila * alto + newColumna;

    return offset;
}

int rotarIzquierda(t_metadata* heder, FILE* img)
{
    logString("\n Rotar izquierda ejecutando...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_rotar-izquierda.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_rotar-izquierda.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder, img, imgGenerada);

    int fila = 0;
    int columna = 0;

    long int offset;

    t_pixel pixel;

    while(fread(&pixel,PIXEL_24,1,img) > 0)
    {
        offset = calculoOffsetRotarIzquierda(fila,columna,heder->alto,heder->ancho);

        fseek(imgGenerada, offset*PIXEL_24 + heder->comienzoImagen, SEEK_SET);

        fwrite(&pixel,PIXEL_24,1,imgGenerada);

        columna++;
        if(columna == heder->ancho)
        {
            fila++;
            columna=0;
        }
    }

    fseek(imgGenerada, 18l, SEEK_SET) ;
    fwrite(&heder->alto, 4l, 1l, imgGenerada);
    fwrite(&heder->ancho, 4l, 1l, imgGenerada);

    fclose(imgGenerada);
    logString(" Rotar izquierda finalizado.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////
int comodin(t_metadata* heder, FILE* img)
{
    logString("\n Comodin Ejecutando...\n Tonalidad sepia...");

    FILE* imgGenerada;
    imgGenerada = fopen("estudiante_comodin.bmp","w+b");

    if(!imgGenerada)
    {
        logString("Error creando archivo nuevo estudiante_comodin.bmp");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    copiarHeder(heder,img, imgGenerada);

    t_pixel pixel;
    //Sepia rojo = 0.393 x Original rojo + 0.769 x Original verde + 0.189 x Original azul
    //Sepia verde = 0.349 x Original rojo + 0.686 x Original verde + 0.168 x Original azul
    //Sepia azul = 0.272 x Original rojo + 0.534 x Original verde + 0.131 x Original azul

    while(fread(&pixel, PIXEL_24, 1l, img) > 0)
    {
        pixel.pixel[BLUE] = (pixel.pixel[RED] * 0.272 + pixel.pixel[GREEN] * 0.534 + pixel.pixel[BLUE] * 0.131) > 255 ? 255 : (pixel.pixel[RED] * 0.272 + pixel.pixel[GREEN] * 0.534 + pixel.pixel[BLUE] * 0.131);
        pixel.pixel[GREEN] = (pixel.pixel[RED] * 0.349 + pixel.pixel[GREEN] * 0.686 + pixel.pixel[BLUE] * 0.168) > 255 ? 255 : (pixel.pixel[RED] * 0.349 + pixel.pixel[GREEN] * 0.686 + pixel.pixel[BLUE] * 0.168);
        pixel.pixel[RED] = (pixel.pixel[RED] * 0.393 + pixel.pixel[GREEN] * 0.769 + pixel.pixel[BLUE] * 0.189) > 255 ? 255 : (pixel.pixel[RED] * 0.393 + pixel.pixel[GREEN] * 0.769 + pixel.pixel[BLUE] * 0.189);

        fwrite(&pixel, PIXEL_24, 1l, imgGenerada);
    }
    fclose(imgGenerada);

    logString(" Comodin finalizado.\n");
    return TODO_OK;
}
////////////////////////////////////////////////////////////////////////////////

/* compartidas */
void copiarHeder(t_metadata* heder,FILE* img, FILE* newImg)
{
    char buffer[TAM_MAXIMO_HEDER_BMP];
    fseek(img, INICIO_ARCHIVO, SEEK_SET) ;
    fread(buffer, heder->comienzoImagen, 1l, img);
    fwrite(buffer, heder->comienzoImagen, 1l, newImg);
}
// fin


/// UTILITARIAS
////////////////////////////////////////////////////////////////////////////////
//                                log Print
////////////////////////////////////////////////////////////////////////////////
void logString(const char* msj)
{
    if(LOG) printf("%s\n", msj);
}

////////////////////////////////////////////////////////////////////////////////
//                                auxiliares test
////////////////////////////////////////////////////////////////////////////////
void mostrarHeder(t_metadata* heder);
void mostrarHeder(t_metadata* heder)
{
    if(!LOG) return;
    printf("Tamaño archivo: %d \n", heder->tamArchivo);
    printf("Tamaño encabecera: %d\n", heder->tamEncabezado);
    printf("Comienso imagen: %d\n", heder->comienzoImagen);
    printf("Alto: %d\n", heder->alto);
    printf("Ancho: %d\n", heder->ancho);
    printf("Profundidad: %d\n", heder->profundidad);

}

/*

unlam.bmp  --negativo --tonalidad-roja  --escala-de-grises --tonalidad-verde  pepito.Bmp --tonalidad-azul --reducir-contraste  --help  --aumentar-contraste --recortar --rotar-derecha --rotar-izquierda --comodin
*/


