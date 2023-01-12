#include "hospital.h"
#include "estructuras.h"
#include "split.h"
#include "heap.h"
#include "lista.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>

#define RECORRIDO_INORDEN 0



/*
*Pre: Direccion de dos pokemones validos.
* Post: Funcion comparadora para el ABB-Hospital, devuelve 1 si el nombre 
* del primer pokemon es alfabeticamente mayor y 0 dado otro caso.
*/

int comparador(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
	       return 0;

    pokemon_t* poke_primero = elemento1;
    pokemon_t* poke_segundo = elemento2;

    if(!strcmp(poke_primero->nombre,poke_segundo->nombre))
       return 0;
    return (strcmp(poke_primero->nombre,poke_segundo->nombre) > 0) ? 1:-1;
}

/*
* Funcion de hospital.h
*/

hospital_t* hospital_crear(){
    hospital_t* hospital = calloc(1,sizeof(hospital_t));
    if(!hospital){
        free(hospital);
        return NULL;
    }
    hospital->pokemones_hospitalizados = abb_crear(comparador);
    hospital->cantidad_pokemon =0;
    
    hospital->entrenadores = lista_crear();
    
    return hospital;
    
}

/*
* Funcion que se encarga de leer la linea del archivo
*Input: recibe un pointer a archivo
*Output: un string a una linea, en caso de falla devuelve NULL
*/
char* leer_linea(FILE* archivo) {
	size_t bytes_leidos = 0;
	size_t tope_caracteres = 1024;
	char* linea_a_leer = malloc(tope_caracteres * sizeof(char));

	if(!linea_a_leer ){
        free(linea_a_leer);
    }           
       
	while (fgets(linea_a_leer + bytes_leidos, (int)tope_caracteres - (int)bytes_leidos, archivo)) {
		size_t largo_linea = strlen(linea_a_leer + bytes_leidos);
		if (largo_linea > 0 && *(linea_a_leer + bytes_leidos + largo_linea - 1) == '\n') {
			*(linea_a_leer + bytes_leidos + largo_linea - 1) = 0;
			return linea_a_leer;
		} else {
			char* auxiliar = realloc(linea_a_leer, tope_caracteres * 2);
			if (!auxiliar) {
				free(linea_a_leer);
				return NULL;
			}

			linea_a_leer = auxiliar;
			tope_caracteres *= 2;
		}
		bytes_leidos += largo_linea;
	}


	if (!bytes_leidos) {
		free(linea_a_leer);
		return NULL;
	}
        
	return linea_a_leer;
}

/*
* Funcion que asigna memoria para la creacion de un entrenador.
*/
entrenador_t* inicializar_entrenador(){
    entrenador_t* nuevo_entrenador = calloc(1,sizeof(entrenador_t));
    if(!nuevo_entrenador){
        free(nuevo_entrenador);
        return NULL;
    }  
    return nuevo_entrenador;

}

/*
* Funcion que asigna memoria para la creacion de un pokemon.
*/

pokemon_t* inicializar_pokemon(){
    pokemon_t* nuevo_pokemon = calloc(1,sizeof(pokemon_t));
    if(!nuevo_pokemon){
        free(nuevo_pokemon);
        return NULL;
    }
    return nuevo_pokemon;           
               
}


/*
* Funcion comparadora de los niveles de los pokemones
*/
int comparador_nivel_poke(void* a,void* b){
    if(!a || !b)
      return 0;
    pokemon_t* poke_primero = a;
    pokemon_t* poke_segundo = b;

    return (poke_primero->nivel > poke_segundo->nivel) ? 1:0;
}

/*
*Pre: Recibe el hospital y la linea del archivo.
*Post: Agrega los campos del pokemon y se inserta en dos estructuras en la lista de pokemones
* y en el abb para ser ordenados alfabeticamente. Ademas agrega los campos del entrenador e inserta 
* su estructura en una lista de entrenadores.
*/
char* agregar_entrenador_y_pokemon(hospital_t* hospital, char* linea){

       
        char** campos_leidos = split(linea,';');
       
        entrenador_t* nuevo_entrenador = inicializar_entrenador();

        nuevo_entrenador->id = (size_t)atoi(campos_leidos[0]);               
        nuevo_entrenador->nombre_entrenador = calloc(strlen(campos_leidos[1])+1,sizeof(char));
        if(!nuevo_entrenador->nombre_entrenador){
            free(hospital->entrenadores);
            return NULL;
        }

        strcpy(nuevo_entrenador->nombre_entrenador, campos_leidos[1]); 
        hospital->entrenadores = lista_insertar(hospital->entrenadores,nuevo_entrenador);  
        if(!hospital->entrenadores) return NULL;

        hospital->cantidad_entrenadores++;      

        nuevo_entrenador->pokemones = lista_crear();  
        if(!nuevo_entrenador->pokemones){
            free(nuevo_entrenador->pokemones);
            free(hospital);
            return NULL;
        }       
       
        for(int i=2; campos_leidos[i] != NULL ;i+=2){  
            
            pokemon_t* nuevo_pokemon = inicializar_pokemon();      
                         
            nuevo_pokemon->nombre = calloc(strlen(campos_leidos[i])+1,sizeof(char));
            if(!nuevo_pokemon->nombre){
                free(nuevo_pokemon->nombre);
                return NULL;
            }
            strcpy(nuevo_pokemon->nombre, campos_leidos[i]);            
            nuevo_pokemon->nivel = (size_t)atoi((campos_leidos[i+1])); 
            nuevo_pokemon->entrenador = nuevo_entrenador;                    
           
            nuevo_entrenador->pokemones = lista_insertar(nuevo_entrenador->pokemones,nuevo_pokemon);  
            if(!nuevo_entrenador->pokemones) return NULL;
            

            hospital->pokemones_hospitalizados = abb_insertar(hospital->pokemones_hospitalizados,nuevo_pokemon);
            if(!hospital->pokemones_hospitalizados) return NULL;
            hospital->cantidad_pokemon++;
            
            
        }  
        
        
        char** auxiliar_campo = campos_leidos;
        for(int i = 0; auxiliar_campo[i]; i++){
            free(auxiliar_campo[i]);            
        }       
        free(campos_leidos);
        free(linea); 

        return linea;
}

/*
* Funcion de hospital.h
*/
bool hospital_leer_archivo(hospital_t* hospital,const char* nombre_archivo){
        
    if(!hospital)
       return false;

    FILE* archivo = fopen(nombre_archivo,"r");
    if(!archivo){
        return false;
    }    
    char* linea_leida =leer_linea(archivo);    
    
    while(linea_leida){ 
        agregar_entrenador_y_pokemon(hospital,linea_leida);      
        linea_leida = leer_linea(archivo);                 
    }  

    fclose(archivo);
    
    return true;

} 

/*
* Funcion de hospital.h
*/
size_t hospital_cantidad_pokemon(hospital_t* hospital) {
    
    if(!hospital)
        return 0;
    return hospital->cantidad_pokemon;
}

/*
* Funcion de hospital.h
*/
size_t hospital_cantidad_entrenadores(hospital_t* hospital){

    if(!hospital)
      return 0;    
    return hospital->cantidad_entrenadores;
}

/*
*Pre: Recibe como parametro la direccion del pokemon y un auxiliar
*Post: Aplica la funcion al pokemon. En caso de ser true aplica a los siguientes 
* pokemones restantes y si es false no puede continuar.
*/
bool auxiliar_a_cada_pokemon(void* poke, void* aux){
    pokemon_t* pokemon = poke;
    bool (*funcion)(pokemon_t* p) = aux;
    return (funcion(pokemon))? true:false;
}


/* 
* Funcion de hospital.h
*/
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    
    if(!hospital || !funcion || !hospital->cantidad_pokemon) 
        return 0;
    
    
    size_t recorrer = abb_con_cada_elemento(hospital->pokemones_hospitalizados,RECORRIDO_INORDEN,auxiliar_a_cada_pokemon,funcion);

        
    return recorrer;
   
       
}


/*
*Pre:Recibe la direccion de un pokemon y un auxiliar
*Post: Libera la memoria utilizada por el pokemon.
*/
bool destruir_pokemon(void* poke, void* aux){
    pokemon_t* poke_nuevo = poke;
    free(poke_nuevo->nombre);
    free(poke_nuevo);

    return true;
}

/*
*Pre:Recibe la direccion de un entrenador y un auxiliar
*Post: Libera la memoria utilizada por el entrenador.
*/

bool destruir_entrenador(void* entrenador, void* aux){
    entrenador_t* entrenador_nuevo = entrenador;
    
    free(entrenador_nuevo->nombre_entrenador);
    lista_con_cada_elemento(entrenador_nuevo->pokemones,destruir_pokemon,NULL);
    lista_destruir(entrenador_nuevo->pokemones);
    free(entrenador_nuevo);

    return true;
}


/*
*Funcion de hospital.h
*/
void hospital_destruir(hospital_t* hospital){
    
    if(!hospital)  return;

    lista_con_cada_elemento(hospital->entrenadores,destruir_entrenador,NULL);
    lista_destruir(hospital->entrenadores); 
    abb_destruir_todo(hospital->pokemones_hospitalizados,NULL);
  
    free(hospital);

    return;

}

/* 
*Funcion del hospital.h
*/
size_t pokemon_nivel(pokemon_t* pokemon) {
    if (!pokemon)
        return 0;
    return pokemon->nivel;

}
/*
* Funcion del hospital.h
*/
const char* pokemon_nombre(pokemon_t* pokemon){
    if(!pokemon)
        return NULL;
    return pokemon->nombre;
}

