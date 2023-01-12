#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

heap_t *heap_crear(int capacidad_max, heap_comparador comparador){
	heap_t *heap = calloc(1, sizeof(heap_t));
	heap->vector = calloc((size_t)capacidad_max, sizeof(void*));
	if(!heap->vector) return NULL;
	heap->tope = 0;
	heap->capacidad_max = capacidad_max;
	heap->comparador = comparador;
	return heap;
}


void swap(heap_t *heap, int pos_1, int pos_2){
	void **vector = heap->vector;
	void *temp = vector[pos_2];
    vector[pos_2] = vector[pos_1];
    vector[pos_1] = temp;
    return;
}


void sift_up(heap_t* heap, int pos_actual){
    if(!pos_actual) return;
    int pos_padre = (pos_actual - 1) / 2;
    if(heap->comparador(heap->vector[pos_padre], heap->vector[pos_actual])){
        swap(heap, pos_actual, pos_padre);
        sift_up(heap, pos_padre);
    }
    return;
}


void sift_down(heap_t *heap, int pos_actual){
	int tope = heap->tope;
	if(pos_actual*2 + 1 >= tope) return;
	int pos_cambio = 2*pos_actual + 1;

	if(2*pos_actual + 2 < tope 
		&& heap->comparador(heap->vector[pos_cambio], heap->vector[2*pos_actual + 2])) 
		pos_cambio = 2*pos_actual + 2;

	
	if(heap->comparador(heap->vector[pos_actual], heap->vector[pos_cambio])){
		swap(heap, pos_cambio, pos_actual);
		sift_down(heap, pos_cambio);
	}

	return;
}


void *heap_extraer_raiz(heap_t *heap){
    if(!heap->tope) return 0;
    void *raiz = heap->vector[0];
    heap->vector[0] = heap->vector[heap->tope-1];
    sift_down(heap, 0);
    heap->tope--;
    return raiz;
}


int heap_insertar(heap_t *heap, void *elemento){
	if(heap->tope == heap->capacidad_max) return -1;
    heap->vector[heap->tope] = elemento;
    sift_up(heap, heap->tope);
    heap->tope++;
    return 0;
}



int heap_cantidad(heap_t* heap){
    if(heap)
        return heap->tope;
    return 0;
}

bool heap_vacio(heap_t* heap){
    if(heap) return !(heap->tope);
    return true;
}


void heap_destruir(heap_t *heap){
	free(heap->vector);
	free(heap);
}


void heap_con_cada_elemento(heap_t *heap, bool (*funcion)(void*)){
	if(!heap->tope) return;
	if(!funcion(heap_extraer_raiz(heap))) return;;
	heap_con_cada_elemento(heap, funcion);
}

