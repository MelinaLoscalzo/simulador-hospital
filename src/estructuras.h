#ifndef __ESTRUCTURAS__H__
#define __ESTRUCTURAS__H__

#include <stdbool.h>
#include <stdlib.h>

#include "heap.h"
#include "simulador.h"
#include "abb.h"
#include "lista.h"



struct _simulador_t{
    hospital_t* hospital;

    lista_iterador_t* hospital_sala_de_espera;
    EstadisticasSimulacion estadisticas;


    pokemon_t* atender_pokemon;
    heap_t* turno_pokemon;  
    
    unsigned int intentos;   
    int actual_id;
    lista_t* dificultades;
    DatosDificultad dificultad_en_uso;
    bool finalizar;
    



};

struct _entrenador_t{
    int id;
    char* nombre_entrenador;
    lista_t* pokemones;
};

struct _hospital_pkm_t{    
    size_t cantidad_pokemon;    
    size_t cantidad_entrenadores;    
    lista_t* entrenadores;
    abb_t* pokemones_hospitalizados;    
    
};

struct _pkm_t{
    char* nombre;
    size_t nivel;    
    entrenador_t* entrenador; 
    
};


#endif /* __ESTRUCTURAS__H__ */
