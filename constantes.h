#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TODO_OK 0
#define ARCHIVO_NO_ENCONTRADO 10
#define NO_SE_PUEDE_CREAR_ARCHIVO 20

#define true 1
#define false 0

#define CANTIDAD_COMANDOS 11
#define TAMANIO_MAXIMO_COMANDO 25

#define NEGATIVO "--negativo"
// invertir los colores de la imagen

#define ESCALA_DE_GRISES "--escala-de-grises"
// deber� promediar los valores de cada color RGB y transformarlo a gris

#define AUMENTAR_CONTRASTE "--aumentar-contraste"
// aumenta el contraste en un 25%

#define REDUCIR_CONTRASTE "--reducir-contraste"
//reduce el contraste en un 25%

#define TONALIDAD_AZUL "--tonalidad-azul"
// aumenta en un 50% la intensidad del color azul

#define TONALIDAD_VERDE "--tonalidad-verde"
// aumenta en un 50% la intensidad del color verde

#define TONALIDAD_ROJO "--tonalidad-roja"
// aumenta en un 50% la intensidad del color rojo

#define RECORTAR "--recortar"
// reduce el tama�o de la imagen al 50%, sin cambiar sus proporciones,
//s�lamente descarta lo que exceda ese tama�o. Por ejemplo: una imagen de 1000px x
//500px, deber� mantener todos los p�xeles que est�n entre 0 y 499 en el eje X y entre 0 y
//249 el eje Y.

#define ROTAR_DERECHA "--rotar-derecha"
// gira la imagen 90 grados a la derecha

#define ROTAR_IZQUIERDA "--rotar-izquierda"
// gira la imagen 90 grados a la derecha

#define COMODIN "--comodin"

#define CANT_BYTES_BITMAP_HEADER 53


#endif // CONSTANTES_H_INCLUDED
