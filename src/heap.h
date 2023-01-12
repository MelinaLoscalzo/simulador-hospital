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


//pre: capacidad maxima del heap, funcion comparadora a usar
//post: crea un heap, sin elementos
heap_t *heap_crear(int capacidad_max, heap_comparador funcion_comparadora);


//pre: dos elementos del heap
//post: swap de los elementos
void swap(heap_t *heap, int posicion_1, int posicion_2);


//pre: puntero valido a heap, posicion inicial de un elemento a flotar
//post: hace flotar al elemento hasta quedar con un heap valido
void sift_up(heap_t *heap, int posicion_inicial);


//pre: puntero valido a heap, posicion inicial de un elemento que debe caer
//post: hace caer al elemento hasta quedar con un heap valido
void sift_down(heap_t *heap, int posicion_inicial);


//pre: puntero valido a heap
//post: extrae la raiz del heap y lo reordena dejandolo valido
void *heap_extraer_raiz(heap_t *heap);


//pre: puntero valido a heap, elemento a insertar
//post: inserta el elemento y reordena el heap dejandolo valido
int heap_insertar(heap_t *heap, void *elemento);


int heap_cantidad(heap_t* heap);

bool heap_vacio(heap_t* heap);


//pre: puntero valido a heap
//post: destruye el heap
void heap_destruir(heap_t *heap);


//pre: puntero a heap, puntero a funcion
//post: aplica una funcion a todos los elementos del heap
void heap_con_cada_elemento(heap_t *heap, bool (*funcion)(void*));


#endif /* __HEAP__H__ */