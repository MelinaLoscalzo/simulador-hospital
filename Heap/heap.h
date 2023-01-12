#ifndef __HEAP__H__
#define __HEAP__H__

#include <stdbool.h>
#include <stdlib.h>

typedef int (*heap_comparador)(void*, void*);

typedef struct heap{
  void** vector;
  int tope;
  int capacidad_max;
  heap_comparador comparador;
} heap_t;


//pre: Capacidad maxima del heap, funcion comparadora a usar
//post: Crea un heap, sin elementos
heap_t *heap_crear(int capacidad_max, heap_comparador funcion_comparadora);


//pre: Dos elementos del heap
//post: swap de los elementos
void swap(heap_t *heap, int posicion_1, int posicion_2);


//pre: Puntero valido a heap, Posicion inicial de un elemento a flotar
//post: Hace flotar al elemento hasta quedar con un heap valido
void sift_up(heap_t *heap, int posicion_inicial);


//pre: Puntero valido a heap, Posicion inicial de un elemento que debe caer
//post: Hace caer al elemento hasta quedar con un heap valido
void sift_down(heap_t *heap, int posicion_inicial);


//pre: Puntero valido a heap
//post: Extrae la raiz del heap y lo reordena dejandolo valido
void *heap_extraer_raiz(heap_t *heap);


//pre: Puntero valido a heap, elemento a insertar
//post: Inserta el elemento y reordena el heap dejandolo valido
void heap_insertar(heap_t *heap, void *elemento);


//pre: Puntero valido a heap
//post: Destruye el heap
void heap_destruir(heap_t *heap);


//pre: Puntero a heap, puntero a funcion
//post: Aplica una funcion a todos los elementos del heap
void heap_con_cada_elemento(heap_t *heap, bool (*funcion)(void*));


#endif /* __HEAP__H__ */
