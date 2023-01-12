#include "lista.h"
#include <stddef.h>
#include <stdlib.h>


/*****************************************************************/
 nodo_t* nodo_crear(void* elemento){
    nodo_t* nodo = calloc(1,sizeof(nodo_t));
    if(!nodo) return NULL;

    nodo->elemento = elemento;  
    nodo->siguiente=NULL;
            
    return nodo;
}

nodo_t* buscar_nodo(nodo_t* nodo,size_t posicion){

    for(int i=0;i<posicion;i++){
        nodo=nodo->siguiente;
    }
    return nodo;
}
/*****************************************************************/

lista_t* lista_crear(){
    return calloc(1,sizeof(lista_t));   
}

lista_t* lista_insertar(lista_t* lista, void* elemento){
       
    if(!lista)
       return NULL;

    if(lista_tamanio(lista) == 0){
       nodo_t* aux_nodo = nodo_crear(elemento);
       lista->nodo_inicio = aux_nodo;
       lista->nodo_fin = aux_nodo;
       lista->cantidad++;
       
    }
    else{
        nodo_t* nodo_nuevo = nodo_crear(elemento);
        lista->nodo_fin->siguiente = nodo_nuevo;
        lista->nodo_fin = nodo_nuevo;
        lista->cantidad++;
    }
    return lista; 
    

}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){

    if(!lista)
        return NULL;

    if(posicion >= lista->cantidad){
        return lista_insertar(lista,elemento);
    }
    nodo_t* insertar_nodo=nodo_crear(elemento);
    nodo_t* auxiliar=lista->nodo_inicio;

    if(posicion==0){
        insertar_nodo->siguiente=lista->nodo_inicio;
        lista->nodo_inicio=insertar_nodo;
        lista->cantidad++;
    }
    else{
        for(int i=0;i<posicion-1;i++){
            auxiliar=auxiliar->siguiente;
        }
        insertar_nodo->siguiente=auxiliar->siguiente;
        auxiliar->siguiente=insertar_nodo;
        lista->cantidad++;
    }

    return lista;
}
  
void* lista_quitar(lista_t* lista){
    
    if(!lista || lista_vacia(lista))
      return NULL;    

    void* nodo_eliminado;

    if(lista_tamanio(lista) == 1){
        nodo_eliminado = lista->nodo_inicio->elemento;
        free(lista->nodo_inicio);
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
        lista->cantidad--;
        return nodo_eliminado;
      
    }
    size_t posicion_para_borrar = lista_tamanio(lista) - 2;
    nodo_t* nodo = buscar_nodo(lista->nodo_inicio,posicion_para_borrar);    
    nodo_t* auxiliar = nodo->siguiente; 
    nodo_eliminado = auxiliar->elemento;
    
    lista->nodo_fin = nodo;
    
    free(auxiliar);

    lista->nodo_fin->siguiente = NULL;
    lista->cantidad--;

    return nodo_eliminado;
}
    
void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){

    if(!lista || lista_vacia(lista))
      return NULL;
    
    if(posicion >=lista_tamanio(lista)-1)
      return lista_quitar(lista);
        
    nodo_t* nodo = lista->nodo_inicio;
    void* nodo_borrado;
    if(posicion == 0){
        nodo_borrado=lista->nodo_inicio->elemento;
        lista->nodo_inicio =nodo->siguiente;        
        free(nodo);        
        
    }
    else{
        for(int i=0;i<posicion-1;i++){
            nodo=nodo->siguiente;
        }
                                  
        nodo_t* borrar_nodo =nodo->siguiente;   
        nodo_borrado=borrar_nodo->elemento;  
        nodo->siguiente= borrar_nodo->siguiente;  
             
        free(borrar_nodo);
        
    }
    
    lista->cantidad--;
    return nodo_borrado;
}
       
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista) ||posicion >lista_tamanio(lista)){
       return NULL;
    }
    nodo_t* nodo_posicion = buscar_nodo(lista->nodo_inicio,posicion);

    if(nodo_posicion != NULL){
        return nodo_posicion->elemento;
    }
    return NULL;
    
}

void* lista_primero(lista_t* lista){
    return lista_elemento_en_posicion(lista,0);
}

void* lista_ultimo(lista_t* lista){
    if(!lista || lista_tamanio(lista) == 0)
       return NULL;
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    
    if(!lista)
      return true;
    if(lista_tamanio(lista) == 0)
       return true;
    return false;
}

size_t lista_tamanio(lista_t* lista){
    if(!lista)
        return 0;    
    return lista->cantidad;

}

void lista_destruir(lista_t* lista){

    if(!lista) 
      return;
    while(lista->nodo_inicio){
        nodo_t* nodo = lista->nodo_inicio;
        lista->nodo_inicio = nodo->siguiente;
        free(nodo);
    }
    free(lista);    
    
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista)
      return NULL;
    
    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if(!iterador) return NULL;

    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente)
      return false;
    return true;
     
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente){
        return false;
    }
    iterador->corriente = iterador->corriente->siguiente;
    if (!iterador->corriente) 
       return false;
    return true;


}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente)
        return NULL;
    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    
    if(!funcion || lista_vacia(lista) || !lista)
       return 0;
    
    bool debe_cumplir_la_condicion = true;
    nodo_t* nodo_iterador = lista->nodo_inicio;
    size_t cantidad_de_elementos =0;
    

    while(debe_cumplir_la_condicion && nodo_iterador != NULL){
        debe_cumplir_la_condicion = funcion(nodo_iterador->elemento,contexto);
        nodo_iterador = nodo_iterador->siguiente;
        cantidad_de_elementos++;
    }
    return cantidad_de_elementos;
}
