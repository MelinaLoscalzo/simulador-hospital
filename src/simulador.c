#include "simulador.h"
#include "hospital.h"
#include "lista.h"
#include "estructuras.h"
#include "abb.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>


/*
*Pre: Direccion de dos pokemones validos.
* Post: Funcion comparadora para el Heap, devuelve 1 si el nombre 
* del primer pokemon es alfabeticamente mayor y 0 dado otro caso.
*/
int comparador_nivel(void* a,void* b){
    if(!a || !b)
      return 0;
    pokemon_t* poke_primero = a;
    pokemon_t* poke_segundo = b;

    return (poke_primero->nivel > poke_segundo->nivel) ? 1:0;
}

/*
*Pre: Direccion de un pokemon y direccion del heap
* Post: Funcion que inserta el pokemon en el heap ordenado de menor
* a mayor nivel.
*/
bool cargar_pokemon_a_heap(void* pokemon, void* heap){

    pokemon_t* pokemon_insertado = pokemon;
    if(!pokemon_insertado) return false;    
    heap_t* heap_pokemones_ordenados = heap;        
    heap_insertar(heap_pokemones_ordenados,pokemon);
    
    return true;
 
}

/*
*Funcion que se encarga de calcular el puntaje segun los intentos ingresados
* Si se logra adivinar en un intento obtenes el puntaje maximo 150 puntos y 
* los puntos van disminuyendo a medida que se haga mas intentos en caso de
* hacer mas de 4 intentos siempre se van a sumar 5 puntos como minimo puntaje
*/
unsigned calcular_puntaje(unsigned cantidad_intentos){

    unsigned int puntos_acumulados =0;
  
    if(cantidad_intentos == 1)
        puntos_acumulados =150;
    else if(cantidad_intentos == 2)
        puntos_acumulados =100;
    else if(cantidad_intentos == 3)
        puntos_acumulados = 50;
    else if(cantidad_intentos == 4)
        puntos_acumulados = 20;
    else 
       puntos_acumulados = 5;
    return puntos_acumulados;
}

/*
* Funcion que se encarga de verificar el nivel en caso de que 
* la diferencia entre el nivel de pokemon y el nivel adivinado de 0,
* significa que fue el nivel adivinado.
*/
int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon){
    
    return (int)nivel_pokemon-(int) nivel_adivinado;
}

/*
* Funcion que se encarga de verificar el string, dado al resultado de 
la funcion verificar nivel, en caso de ser 0 devuelve el mensaje exitoso
* y en caso de no adivinar devuelve el mensaje de fallo.Esta funcion es utilizada
* para cargar el mensaje cuando se crea una nueva dificultad.
*/
const char* verificacion_a_string(int resultado_verificacion){
    
    if(resultado_verificacion == 0)
        return  "¡Acertaste el nivel!";
    return  "¡Fallaste no adivinaste el nivel!";
}

/*
* Funcion que se encarga de verificar el string, dado al resultado de 
la funcion verificar nivel, en caso de ser 0 devuelve el mensaje exitoso
* y en caso de no adivinar devuelve el mensaje de fallo.Esta funcion es utilizada
* para cargar el mensaje para la dificultad Modo Facil.
*/
const char* verificacion_a_string_facil(int resultado_verificacion){
    
    if(resultado_verificacion == 0)
        return  "¡Acertaste el nivel!. Estuvo muy facil";
    return  "¡Fallaste no adivinaste el nivel.Vamos logra superar el nivel facil!";
}

/*
* Funcion que se encarga de verificar el string, dado al resultado de 
la funcion verificar nivel, en caso de ser 0 devuelve el mensaje exitoso
* y en caso de no adivinar devuelve el mensaje de fallo.Esta funcion es utilizada
* para cargar el mensaje para la dificultad Modo Normal.
*/
const char* verificacion_a_string_normal(int resultado_verificacion){
    
    if(resultado_verificacion == 0)
         return  "¡Acertaste el nivel! Superaste el nivel normal";
    return  "¡Fallaste no adivinaste el nivel.Vamos logra superar el nivel normal!";
}

/*
* Funcion que se encarga de verificar el string, dado al resultado de 
la funcion verificar nivel, en caso de ser 0 devuelve el mensaje exitoso
* y en caso de no adivinar devuelve el mensaje de fallo.Esta funcion es utilizada
* para cargar el mensaje para la dificultad Modo Dificil.
*/
const char* verificacion_a_string_dificil(int resultado_verificacion){
    
    if(resultado_verificacion == 0)
         return  "¡Acertaste el nivel!. Pasaste el nivel dificil";
    return  "¡Fallaste no adivinaste el nivel.Vamos logra superar el nivel dficil!";
}

/*
*Pre:Recibe el simulador como parametro
*Post: Busca la dificultad en uso en la lista de dificultades,segun su posicion
*/
void* dificultad_en_uso(simulador_t* simulador){
    if(!simulador)
      return NULL;
    return lista_elemento_en_posicion(simulador->dificultades,(size_t)simulador->actual_id);
     
}

/*
* Funcion que se encarga de duplicar el nombre de la dificultad y poder agregarla a la lista.
*/
char* duplicar_nombre(const char* nombre_copiar){
    
    char* copia_nombre = calloc(strlen(nombre_copiar)+1,sizeof(char));
    if(!copia_nombre){
        free(copia_nombre);
        return NULL;
    }
    strcpy(copia_nombre,nombre_copiar);

    return copia_nombre;
    
}

/*
* Pre: Recibe las tres funciones dadas en la estructura DatosDificultad
*Post:Crea la dificultad con el nombre y sus tres funciones cargadas
*/
DatosDificultad* crear_dificultad(const char* nombre,unsigned (*calcular_puntaje)(unsigned cantidad_intentos),int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon),const char* (*verificacion_a_string)(int resultado_verificacion)){
   

   DatosDificultad* dificultad_creada = malloc(sizeof(DatosDificultad));
   if(!dificultad_creada){
       free(dificultad_creada);
       return NULL;
   }
   
   dificultad_creada->nombre = duplicar_nombre(nombre);
   dificultad_creada->verificar_nivel = verificar_nivel;
   dificultad_creada->verificacion_a_string = verificacion_a_string;
   dificultad_creada->calcular_puntaje = calcular_puntaje;  
   return dificultad_creada;
}

/*
*Funcion que se encarga de insertar e inicializar a la estructura lista las tres dificultades:
-Modo Facil
-Modo Normal
-Moodo Dificil
*/
lista_t* insertar_y_inicializar_dificultades(){

    lista_t* lista_dificultades = lista_crear();
    if(!lista_dificultades){
        return NULL;
    }
      
    DatosDificultad* modo_facil = crear_dificultad("Facil",calcular_puntaje,verificar_nivel,verificacion_a_string_facil);
    if(!modo_facil){
        return ErrorSimulacion;
    }
    lista_dificultades = lista_insertar(lista_dificultades,modo_facil);


    DatosDificultad* modo_normal = crear_dificultad("Normal",calcular_puntaje,verificar_nivel,verificacion_a_string_normal);
    if(!modo_normal){
        return ErrorSimulacion;
    }
    lista_dificultades = lista_insertar(lista_dificultades,modo_normal);



    DatosDificultad* modo_dificil = crear_dificultad("Dificil",calcular_puntaje,verificar_nivel,verificacion_a_string_dificil);
    if(!modo_dificil){
        return ErrorSimulacion;
    }
    lista_dificultades = lista_insertar(lista_dificultades,modo_dificil);  


    return lista_dificultades;
    


}

/*
* Pre: Recibe la direccion de la dificultad insertada y la dificultad que va a ser insertada
*Post: Devuelve false si el nombre de las dificultades es igual
*/
bool verificacion_dificultad_repetida(void* dificultad,void* nueva_dificultad){

    return (strcmp(((DatosDificultad*)dificultad)->nombre,((DatosDificultad*)nueva_dificultad)->nombre));

}

/*
*Funcion que se encarga de asignar memoria al puntero
* Estadisticas de Simulacion
*/
EstadisticasSimulacion* crear_estadisticas(){
    return calloc(1,sizeof(EstadisticasSimulacion));
}

/*
*Pre:Puntero valido al simulador y un puntero valido a la informacion de EstadisticasSimulacion
*Post:Inicializa las estructura pasada por los parametros.
*/
ResultadoSimulacion Obtener_Estadisticas(simulador_t* simulador,void* informacion){

    if(!simulador || !informacion) return ErrorSimulacion;
  
    EstadisticasSimulacion* estadisticas = informacion;
    estadisticas->pokemon_atendidos = simulador->estadisticas.pokemon_atendidos;
    estadisticas->entrenadores_atendidos = simulador->estadisticas.entrenadores_atendidos;
    estadisticas->entrenadores_totales = (unsigned)lista_tamanio(simulador->hospital_sala_de_espera->lista);
    estadisticas->pokemon_en_espera = (unsigned) simulador->turno_pokemon->tope;
    estadisticas->pokemon_totales = (unsigned)hospital_cantidad_pokemon(simulador->hospital);
    estadisticas->puntos = simulador->estadisticas.puntos;
    estadisticas->cantidad_eventos_simulados = simulador->estadisticas.cantidad_eventos_simulados;
 
    

    return ExitoSimulacion;

}


/*
*Pre: Puntero valido al simulador
*Post: Atiende los entrenadores si quedan entrenadores en la sala de espera lo pasa a estado de atendido y sus pokemon quedan en espera.
*En caso de error que no haya entrenadores devuelve ErrorSimulacion
*/
ResultadoSimulacion Atender_Proximo_Entrenador(simulador_t* simulador){
    if(!simulador || !simulador->turno_pokemon || !simulador->hospital_sala_de_espera || !hospital_cantidad_entrenadores(simulador->hospital)) 
       return ErrorSimulacion;
    entrenador_t* entrenador_siguiente = lista_iterador_elemento_actual(simulador->hospital_sala_de_espera);
    if(!entrenador_siguiente || !entrenador_siguiente->pokemones) return ErrorSimulacion;
        
    if(entrenador_siguiente){
        lista_con_cada_elemento(entrenador_siguiente->pokemones,cargar_pokemon_a_heap,simulador->turno_pokemon);
        simulador->estadisticas.entrenadores_atendidos++;        
    }
    if(!simulador->atender_pokemon){
        simulador->atender_pokemon = heap_extraer_raiz(simulador->turno_pokemon);      
       
    }
    
    lista_iterador_avanzar(simulador->hospital_sala_de_espera);
    
   
   
    return ExitoSimulacion;   

}


/*
*Pre: Puntero a simulador y un puntero para la informacion del pokemon
*Post:Actualiza los campos de la InformacionPokemon.En caso de no atender un pokemon
* el nommbre del entrenador y el nombre del pokemon van a estar en NULL.
*/
ResultadoSimulacion informacion_pokemon(simulador_t* simulador,void* info){

    if(!simulador || !info ) return ErrorSimulacion;

    InformacionPokemon* info_poke = info;

     if(!simulador->atender_pokemon){
        info_poke->nombre_entrenador = NULL;
        info_poke->nombre_pokemon = NULL;
        return ErrorSimulacion;
     }
   
    info_poke->nombre_entrenador = simulador->atender_pokemon->entrenador->nombre_entrenador; 
    info_poke->nombre_pokemon = simulador->atender_pokemon->nombre;

    return ExitoSimulacion;

}

/*
*Pre:Recibe un puntero valido al simulador y un puntero valido Intento*
*Post:Intento tiene el campo nivel_adivinado inicializado al nivel que se propone
* Funcion que verifica si el nivel pasado es el correcto del pokemon atendido (usando las funciones 
* que corresponden según la dificultad activa) y llena la estructura con el resultado.
* El campo es_correcto es true si se acerta el nivel. El campo resultado_string contiene 
* la salida de la función verificacion_a_string para esa dificultad.
* En caso de un puntero nulo y si no hay pokemon siendo atendido devuelve ErrorSimulacion.
*/

ResultadoSimulacion adivinar_nivel_pokemon(simulador_t* simulador,Intento* intento){
    if(!simulador || !intento ) return ErrorSimulacion;
 
    if(!simulador->atender_pokemon){
        intento->es_correcto = false;
        return ErrorSimulacion;
    }
    simulador->intentos++;
    DatosDificultad* dificultad_activa = dificultad_en_uso(simulador);
  
    unsigned int puntaje =dificultad_activa->calcular_puntaje(simulador->intentos);
  
   
    int verificar = dificultad_activa->verificar_nivel(intento->nivel_adivinado,(unsigned int)pokemon_nivel(simulador->atender_pokemon));
    intento->nivel_adivinado = (unsigned int) verificar;

    const char* resultado_string = dificultad_activa->verificacion_a_string(verificar);

    if(verificar == 0 ){
        intento->es_correcto = true;
        intento->resultado_string = resultado_string;
        simulador->estadisticas.pokemon_atendidos++;
        
        void* pokemon = heap_extraer_raiz(simulador->turno_pokemon);
        pokemon_t* poke_siguiente = pokemon;
        simulador->atender_pokemon = poke_siguiente;
        if(!poke_siguiente) return ExitoSimulacion;
        simulador->estadisticas.puntos += puntaje;
        simulador->intentos = 0;
        
        return ExitoSimulacion;
    }
    else{
        intento->es_correcto = false;
        simulador->intentos++;
        intento->resultado_string = resultado_string;
    }
    intento->resultado_string = resultado_string;
    
    return ExitoSimulacion;
}

/*
*Pre: Puntero a simulador y un puntero para la informacion dificultad
*Post:Actualiza los campos de la InformacionDificultad.En caso de no existir
* devuelve false si el id de la dificultad no existe.
*/
ResultadoSimulacion obtener_informacion_dificultad(simulador_t* simulador,void* info_dificultad){

    if(!simulador || !info_dificultad ) 
        return ErrorSimulacion;
    
    InformacionDificultad* informacion = info_dificultad;
    
    DatosDificultad* dificultad_elegida = lista_elemento_en_posicion(simulador->dificultades,(size_t)informacion->id);

    if(!dificultad_elegida){
        informacion->nombre_dificultad = NULL;
        informacion->en_uso = false;
        informacion->id = -1;
        return ErrorSimulacion;
    }
   
    informacion->nombre_dificultad = dificultad_elegida->nombre;
    informacion->en_uso = simulador->actual_id == informacion->id;
    
    
    

    return ExitoSimulacion;
}

/*
*Pre:Puntero valido al simulador y a un entero id
*Post: Acttualiza la dificultad actual al id seleccionado 
*/
ResultadoSimulacion seleccionar_dificultad(simulador_t* simulador,void* informacion){
    if(!simulador || !informacion){
        return ErrorSimulacion;
    }
    int id_dificultad_seleccionada = (*(int*)informacion);
    if(id_dificultad_seleccionada >= lista_tamanio(simulador->dificultades)){
        return ErrorSimulacion;
    }
    simulador->actual_id = id_dificultad_seleccionada;
    
   
    return ExitoSimulacion;
}

/*
*Pre:Puntero a simulador y un puntero a una nueva diifcultad para agregar
*Post:Agrega la dificultad nueva a la lista de dificultades.
*/
ResultadoSimulacion agregar_dificultad(simulador_t* simulador,void* informacion_nueva_dificultad){

   
    if(!simulador || !informacion_nueva_dificultad){
        return ErrorSimulacion;
    }
    DatosDificultad* info_dificultad = informacion_nueva_dificultad;
    size_t nombre = lista_con_cada_elemento(simulador->dificultades,verificacion_dificultad_repetida,informacion_nueva_dificultad);
    size_t tamanio_actual_lista = lista_tamanio(simulador->dificultades);

    if(nombre < tamanio_actual_lista ){
        return ErrorSimulacion;
    } 

   
    DatosDificultad* dificultad_agregada = crear_dificultad(info_dificultad->nombre,info_dificultad->calcular_puntaje,info_dificultad->verificar_nivel,info_dificultad->verificacion_a_string);
    if(!dificultad_agregada){
        return ErrorSimulacion;
    }
    lista_t* nuevas_dificultades_agregar = lista_insertar(simulador->dificultades,dificultad_agregada);
    if(!nuevas_dificultades_agregar){
        return ErrorSimulacion;
    }

    
    return ExitoSimulacion;

}

/*
*Pre: Recibe un puntero a simulador y un puntero valido a los datos
*Post:Marca la simulación como finalizada, no se podrán procesar mas eventos. Intentar simular mas eventos devuelve error.
*/
ResultadoSimulacion finalizar_simulacion(simulador_t* simulador,void* datos){

    simulador->finalizar = true;
    return ExitoSimulacion;
}



/*
* Funcion del simulador.h
*/
simulador_t* simulador_crear(hospital_t* hospital){

    if(!hospital) return NULL;

    simulador_t* simulador = calloc(1,sizeof(simulador_t));
    if(!simulador){
        free(simulador);
        return NULL;
    }
    simulador->hospital = hospital;
    simulador->turno_pokemon = heap_crear((int)hospital->cantidad_pokemon,comparador_nivel);
    simulador->hospital_sala_de_espera = lista_iterador_crear(hospital->entrenadores);
    simulador->dificultades = insertar_y_inicializar_dificultades();
    simulador->actual_id = 1;
    
    return simulador;
}

/*
* Funcion del simulador.h
*/
ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador) return ErrorSimulacion;
    simulador->estadisticas.cantidad_eventos_simulados++;
    if(simulador->finalizar == true) return ErrorSimulacion;

    
    if(evento == AtenderProximoEntrenador){
       return Atender_Proximo_Entrenador(simulador);                
    }
    else if(evento == ObtenerEstadisticas){
       return Obtener_Estadisticas(simulador,datos);
    }
    else if(evento == ObtenerInformacionPokemonEnTratamiento){
       return informacion_pokemon(simulador,datos);
    }
    else if(evento == AdivinarNivelPokemon){
       return adivinar_nivel_pokemon(simulador,datos);
    }
    else if(evento == ObtenerInformacionDificultad){
       return obtener_informacion_dificultad(simulador,datos);
    }
    else if(evento == AgregarDificultad){
        return agregar_dificultad(simulador,datos);
    }
    else if(evento == SeleccionarDificultad){
        return seleccionar_dificultad(simulador,datos);
    }
    else if(evento == FinalizarSimulacion){
        return finalizar_simulacion(simulador,datos);
    }
    else{
        return ErrorSimulacion;
    }
   return ExitoSimulacion;

}

/*
*Pre: Recibe la lista donde se guardan las dificultades.
*Post: Libera la memoria utilizada para las dificultades
*/
void destruir_lista_dificultades(lista_t* lista){

    while(!lista_vacia(lista)){

        DatosDificultad* dificultad = (DatosDificultad*)lista_ultimo(lista);
        free((char*)dificultad->nombre);
        free(dificultad);
        lista_quitar(lista);
    }
    lista_destruir(lista);
    return;
}


/*
*Funcion del simulador.h
*/
void simulador_destruir(simulador_t* simulador){
    if(!simulador) return;

    
    heap_destruir(simulador->turno_pokemon);
    lista_iterador_destruir(simulador->hospital_sala_de_espera);  
    destruir_lista_dificultades(simulador->dificultades);
    hospital_destruir(simulador->hospital);
    free(simulador);
    
    
    return;
}

