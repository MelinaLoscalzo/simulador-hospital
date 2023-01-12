#include "split.h"
#include <stdlib.h>
#include <string.h>
#define CARACTER '\0'


size_t contar_separadores(const char* string, char separador) {

    size_t contador = 0;
    for (size_t pos = 0; string[pos] != CARACTER; pos++) {
        if (string[pos] == separador)
            contador++;

    }
    return contador;
}

size_t contar_separadores_siguientes(const char* string, char separador) {

    size_t posicion = 0;

    while (string[posicion] != separador && string[posicion] != CARACTER )
        posicion++;

    return posicion;
}
char* duplicar_string(const char* string, size_t cantidad){
    char* nuevo_string = malloc((1+cantidad)*sizeof(char));
    if(!nuevo_string) 
       return NULL;

    for(size_t i=0; i< cantidad;i++){
        nuevo_string[i] = string[i];
    }
    nuevo_string[cantidad] = 0;

    return nuevo_string;
}

void liberar_memoria(char** vector){
    int i=0;
    while(vector[i] != NULL){
        free(vector[i]);
        i++;
    }
}

char **split(const char *string, char separador) {

    if(!string){
        return NULL;
    }
    size_t separadores = contar_separadores(string, separador);
    size_t cantidad_substrings = separadores+1;
    char** vector_substrings = calloc(cantidad_substrings+ 1, sizeof(void*));
    if(!vector_substrings){
        return NULL;
    }
   
    for(size_t i=0; i< cantidad_substrings; i++) {
        size_t cantidad_separadores = contar_separadores_siguientes(string, separador);
        char* substring = duplicar_string(string,cantidad_separadores);
        
        if(!vector_substrings) {
            liberar_memoria(vector_substrings);
            return NULL;
        }
        vector_substrings[i] = substring;
        string += cantidad_separadores+1;

        
    }
   
    


    return vector_substrings;

}



