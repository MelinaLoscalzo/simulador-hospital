#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


/***************************************/
nodo_abb_t* crear_nodo(){
  nodo_abb_t * nodo_nuevo = calloc(1, sizeof(nodo_abb_t));
  if (!nodo_nuevo)
    return NULL;  
  return nodo_nuevo;
}
/***************************************/



abb_t* abb_crear(abb_comparador comparador){
    
    abb_t* arbol_nuevo = calloc(1,sizeof(abb_t));
    if(!arbol_nuevo || !comparador)
       return NULL;
    
    arbol_nuevo->nodo_raiz = NULL;
    arbol_nuevo->comparador=comparador;

    return arbol_nuevo;
}

nodo_abb_t* buscar_nodo_en_el_arbol(nodo_abb_t* nodo, nodo_abb_t* nodo_padre, abb_comparador comparador, void* elemento){
  if(!nodo){
      return nodo_padre;
  }
  nodo_padre = nodo;
  int resultado_comparacion = comparador(nodo->elemento, elemento);

	if(resultado_comparacion >= 0)
		return buscar_nodo_en_el_arbol(nodo->izquierda, nodo_padre, comparador, elemento);  
	return buscar_nodo_en_el_arbol(nodo->derecha, nodo_padre, comparador, elemento);
}

abb_t* abb_insertar(abb_t* arbol, void* elemento){  
  
  if (!arbol) return NULL;  

  nodo_abb_t * nodo = crear_nodo();
  nodo->elemento = elemento;

  if(abb_vacio(arbol)){
      nodo->izquierda = NULL;
      nodo->derecha = NULL;

      arbol->nodo_raiz = nodo;
      arbol->tamanio++;
      return arbol;
  }
  nodo_abb_t* nodo_padre = NULL;
  nodo_padre = buscar_nodo_en_el_arbol(arbol->nodo_raiz, nodo_padre, arbol->comparador, elemento);

  int comparacion_resultado = arbol->comparador(nodo_padre->elemento, elemento);

  if(comparacion_resultado >= 0){
        nodo_padre->izquierda = nodo;
  }
  else{
      nodo_padre->derecha = nodo;
  }
  arbol->tamanio++;
    
  return arbol;

}

void* extraer_nodo_minimo(nodo_abb_t* nodo, void* borrar){
  
  while(nodo->derecha != NULL){
    nodo = nodo->derecha;
  }
  void* auxiliar = nodo->elemento;
  nodo->elemento = borrar;
  
  return auxiliar;
}


nodo_abb_t* quitar_nodos_aux(nodo_abb_t* nodo, void* elemento,void** guardar_elemento,abb_comparador comparador){
   
    if (!nodo || !comparador) return NULL;    
    
    int comparacion = comparador(elemento,nodo->elemento);       
    
    if(comparacion == 0){ 
      if (nodo->izquierda && nodo->derecha) {        
        // DOS HIJOS  
      void* auxiliar = nodo->elemento;
      nodo->elemento = extraer_nodo_minimo(nodo->izquierda, auxiliar);
      nodo->izquierda = quitar_nodos_aux(nodo->izquierda,auxiliar,guardar_elemento,comparador);      
      }
      else {
        nodo_abb_t* hijo_encontrado = nodo->derecha  ? nodo->derecha : nodo->izquierda;
        *guardar_elemento = nodo->elemento;
			  free(nodo);
        
        return hijo_encontrado;        
		  }     	
	  }     
    if(comparacion > 0){
      nodo->derecha = quitar_nodos_aux(nodo->derecha,elemento,guardar_elemento,comparador);      
    }      
    else {
       nodo->izquierda = quitar_nodos_aux(nodo->izquierda,elemento,guardar_elemento, comparador);       
    }       
    return nodo;  
  
}   

void* abb_quitar(abb_t* arbol,void* elemento){
    
 if(!arbol) return NULL;
 void* eliminado=NULL;
 
 arbol->nodo_raiz= quitar_nodos_aux(arbol->nodo_raiz,elemento,&eliminado,arbol->comparador);
 arbol->tamanio--; 

return eliminado;

 

}

void* abb_buscar(abb_t* arbol, void* elemento){
  if(!arbol || abb_vacio(arbol))
    return NULL;
  
  nodo_abb_t* nodo = arbol->nodo_raiz;
  void* elemento_encontrado = NULL;

  while(nodo){
    int resultado_comparacion = arbol->comparador(elemento,nodo->elemento);

    if(resultado_comparacion == 0){
      elemento_encontrado = nodo->elemento;
      return elemento_encontrado;
    }
    else if(resultado_comparacion < 0){
      nodo = nodo->izquierda;
    }
    else{
      nodo = nodo->derecha;
    }
  }
  return elemento_encontrado;
 
}


bool abb_vacio(abb_t* arbol){
  if(!arbol || !arbol->nodo_raiz)
     return true;
  return false;
}


size_t abb_tamanio(abb_t *arbol){
  if(!arbol)
    return 0;
  return arbol->tamanio;
}


void destruir_aux(nodo_abb_t* nodo_raiz, void (*destructor)(void*)){
    if (!nodo_raiz) return;
    destruir_aux(nodo_raiz->izquierda, destructor);
    destruir_aux(nodo_raiz->derecha, destructor);

    if (destructor)
        destructor(nodo_raiz->elemento);

    free(nodo_raiz);
}

void abb_destruir_todo(abb_t* arbol, void (*destructor)(void*)){
    if(!arbol)return;

    destruir_aux(arbol->nodo_raiz, destructor);
    free(arbol);
}
void abb_destruir(abb_t* arbol){
    abb_destruir_todo(arbol, NULL);
}


void iterador_interno_abb_recorrer_inorden(nodo_abb_t* arbol,abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux,bool* condicion_corte,size_t* cantidad_invocada){
 
  if (!arbol || !*(condicion_corte));

  if(arbol->izquierda != NULL){
    iterador_interno_abb_recorrer_inorden(arbol->izquierda,INORDEN,funcion,aux,condicion_corte,cantidad_invocada);
  } 
         
   if (!arbol || !*(condicion_corte )) return;
   (*cantidad_invocada)++; 
    *condicion_corte = funcion(arbol->elemento,aux);      
  
  
  if(arbol->derecha != NULL){
    iterador_interno_abb_recorrer_inorden(arbol->derecha,INORDEN,funcion,aux, condicion_corte,cantidad_invocada);
  }
  
  

}

void iterador_interno_abb_recorrer_preorden(nodo_abb_t* arbol,abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux,bool* condicion_corte,size_t* cantidad_invocada){
   
  if(!arbol) return;
  
  if (!arbol || !*(condicion_corte)) return;
  (*cantidad_invocada)++; 
  *condicion_corte = funcion(arbol->elemento,aux);
   
  
  if(arbol->izquierda != NULL){
    iterador_interno_abb_recorrer_preorden(arbol->izquierda,PREORDEN,funcion,aux, condicion_corte,cantidad_invocada);
  }         
  if(arbol->derecha != NULL){
    iterador_interno_abb_recorrer_preorden(arbol->derecha,PREORDEN,funcion,aux, condicion_corte,cantidad_invocada);
  }    
  
} 

void iterador_interno_abb_recorrer_postorden(nodo_abb_t* arbol,abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux,bool* condicion_corte,size_t* cantidad_invocada){
  
  if(!arbol) return;

  if(arbol->izquierda != NULL){
    iterador_interno_abb_recorrer_postorden(arbol->izquierda,POSTORDEN,funcion,aux, condicion_corte,cantidad_invocada);
  }     
  if(arbol->derecha != NULL){
    iterador_interno_abb_recorrer_postorden(arbol->derecha,POSTORDEN,funcion,aux, condicion_corte,cantidad_invocada);
  }  
  if (!arbol || !*(condicion_corte)) return;
  (*cantidad_invocada)++;
  *condicion_corte = funcion(arbol->elemento,aux);
  

  

}


size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux){
  
  size_t cantidad_invocada=0;
  bool condicion_corte = true;

  if (!arbol || !funcion || abb_vacio(arbol))
    return cantidad_invocada;
    
  if(recorrido == INORDEN )
    iterador_interno_abb_recorrer_inorden(arbol->nodo_raiz,INORDEN,funcion,aux,&condicion_corte,&cantidad_invocada);
    
  if(recorrido == POSTORDEN)
     iterador_interno_abb_recorrer_postorden(arbol->nodo_raiz,POSTORDEN,funcion,aux,&condicion_corte,&cantidad_invocada);
  
  if(recorrido == PREORDEN)
    iterador_interno_abb_recorrer_preorden(arbol->nodo_raiz,PREORDEN,funcion,aux,&condicion_corte,&cantidad_invocada);
  
  
  return cantidad_invocada;
} 



size_t abb_recorrer_inorden(nodo_abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array,size_t* nodos_guardados){
 
  if (!arbol || !array || tamanio_array <= 0) return 0;
    
  if(arbol->izquierda != NULL){
    abb_recorrer_inorden(arbol->izquierda,INORDEN,array,tamanio_array,nodos_guardados);
  }       
  
  if(*nodos_guardados < tamanio_array){
    array[*nodos_guardados] = arbol->elemento;
    *nodos_guardados += 1;
  }  
  if(arbol->derecha != NULL){
    abb_recorrer_inorden(arbol->derecha,INORDEN,array,tamanio_array,nodos_guardados);
  }  

  return *nodos_guardados;
}

size_t abb_recorrer_preorden(nodo_abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array,size_t* nodos_guardados){
  
  if (!arbol || !array || tamanio_array <= 0) return 0;

  if(*nodos_guardados < tamanio_array){
      array[*nodos_guardados] = arbol->elemento;
      *nodos_guardados += 1;
  }

  if(arbol->izquierda != NULL){
    abb_recorrer_preorden(arbol->izquierda,PREORDEN,array,tamanio_array,nodos_guardados);
  }
         
  if(arbol->derecha != NULL){
    abb_recorrer_preorden(arbol->derecha,PREORDEN,array,tamanio_array,nodos_guardados);
  }  
  
  return *nodos_guardados;
}

size_t abb_recorrer_postorden(nodo_abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array,size_t* nodos_guardados){
  
  if (!arbol || !array || tamanio_array <= 0) return 0;

  if(arbol->izquierda != NULL){
    abb_recorrer_postorden(arbol->izquierda,POSTORDEN,array,tamanio_array,nodos_guardados);
  }      
   
  if(arbol->derecha != NULL){
    abb_recorrer_postorden(arbol->derecha,POSTORDEN,array,tamanio_array,nodos_guardados);
  }      

  if(*nodos_guardados < tamanio_array){
      array[*nodos_guardados] = arbol->elemento;
      *nodos_guardados += 1;
  } 
  
  return *nodos_guardados;
}



size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array){
  
  
  size_t contador_recorridos=0;

  if (!arbol || !array || tamanio_array <= 0)
    return contador_recorridos;
  
  if(recorrido == INORDEN)
    return abb_recorrer_inorden(arbol->nodo_raiz,INORDEN,array,tamanio_array,&contador_recorridos);

  if(recorrido == PREORDEN)
    return abb_recorrer_preorden(arbol->nodo_raiz,PREORDEN,array,tamanio_array,&contador_recorridos);
    
  if(recorrido == POSTORDEN)
    return abb_recorrer_postorden(arbol->nodo_raiz,POSTORDEN,array,tamanio_array,&contador_recorridos);  
  

  return contador_recorridos;
    
  
}
