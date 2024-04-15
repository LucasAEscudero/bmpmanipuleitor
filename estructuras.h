#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct
{
    unsigned char pixel[2];
    unsigned int profundidad;  // Esta estructura admite formatos de distinta profundidad de color, a priori utilizaremos solo 24 bits.
}t_pixel;

typedef struct
{
    unsigned int tamArchivo;
    unsigned int tamEncabezado;    // El tamanio del encabezado no siempre coincide con el comienzo de la imagen
    unsigned int comienzoImagen;   // Por eso dejo espacio para ambas cosas
    unsigned int ancho;
    unsigned int alto;
    unsigned short profundidad;
}t_metadata;

typedef struct
{
  char* comandos[6];
  int cantComandos;
  char* nombreArchivo[1];
  int sinNombreDeArchivo;
  char* errores[6];
  int cantErrores;
} t_parametrosConsola;


#endif // ESTRUCTURAS_H_INCLUDED
