#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include "src/estructuras.h"
#include "src/heap.h"
#include "src/abb.h"
#include "src/lista.h"

#include "string.h"
#include <stdbool.h>



bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


/*..........................PRUEBAS NUEVAS CON LA IMPLEMENTACION ACTUALIZADA..........................................*/

/*----------------FUNCIONES AUXILIARES PARA HEAP--------------------------------------------------------------*/
int comparar_heap_minimal(void *a,void *b){
	
    int* poke_primero = a;
    int* poke_segundo = b;

     return (*poke_primero > *poke_segundo) ? 1:0;
}

void destructor_heap(void* elemento){
    free(elemento);
}
void insertar_heap_a_vector(heap_t* heap, int* vector_heap,size_t tope_vector){
    
    if(!heap)
      return;
    
    for(size_t i=0;i<tope_vector ; i++){
        heap_insertar(heap,(void*)&vector_heap[i]);
    }
    return;
    
}

void quitar_elemento_del_vector(heap_t* heap,size_t* tope,int* vector_heap){
        
    while(!heap_vacio(heap) ){
        vector_heap[(*tope)] = *(int*)heap_extraer_raiz(heap);
        (*tope)++;
        heap_extraer_raiz(heap);
    }
    return;
}
/*----------------------------------------------------------------------------------------------------------*/
int comparador_pokemones_nombre(void* a,void* b){
    if(!a || !b)
      return 0;
    pokemon_t* poke_primero = a;
    pokemon_t* poke_segundo = b;

    if(strcmp(poke_primero->nombre,poke_segundo->nombre) >= 0)
      return 1;
    else return 0;
}
int comparador_pokemones_nivel(void* a,void* b){
    if(!a || !b)
      return 0;
    pokemon_t* poke_primero = a;
    pokemon_t* poke_segundo = b;

    return (poke_primero->nivel > poke_segundo->nivel) ? 1:0;
}

bool mostrar_elemento_nombre(void* pok){
    if(!pok) return false;
    if(pok)
      printf("Pokemon:%s\n Nivel:%lu\n",((pokemon_t*)pok)->nombre,((pokemon_t*)pok)->nivel);
    return true;
}
void destruir_pokemon_pruebas(void* pokemon){
  if(pokemon)
    free(pokemon);
}

void Pruebas_Creacion_Heap_Minimal(){


    heap_t* heap = heap_crear(5,comparar_heap_minimal);
    int numero = 15;
    void* elemento = &numero;
  
    heap_insertar(heap, elemento);
    pa2m_afirmar(!heap_vacio(heap), "El heap no se encuentra vacio exitosamente");
    pa2m_afirmar(heap_cantidad(heap) == 1, "El heap contiene un elemento");
    pa2m_afirmar(heap_extraer_raiz(heap) == elemento, "El elemento insertado es la raiz actualmente exitosamente");
 
    heap_extraer_raiz(heap);
    pa2m_afirmar(heap_vacio(heap), "El heap queda vacio");
    pa2m_afirmar(heap_cantidad(heap) == 0, "El heap queda con 0 elementos");
    pa2m_afirmar(heap_extraer_raiz(heap) == NULL, "El heap no tiene raíz");
    
   
    heap_destruir(heap);
}


void Prueba_Pokemones_Heap(){

    pa2m_nuevo_grupo("PRUEBAS HEAP CON COMPARADOR DE NOMBRE");

    heap_t* heap_poke = heap_crear(10,comparador_pokemones_nivel);

    pokemon_t* poke_1= calloc(1,sizeof(pokemon_t));
    if(!poke_1)
      return;
    poke_1->nombre = "Pikachu";
    poke_1->nivel = 50;

    pokemon_t* poke_2= calloc(1,sizeof(pokemon_t));
    if(!poke_2)
      return;
    poke_2->nombre = "Mew";
    poke_2->nivel = 30;


    pokemon_t* poke_3= calloc(1,sizeof(pokemon_t));
    if(!poke_3)
      return;
    poke_3->nombre = "Savaj";
    poke_3->nivel = 34;


    pokemon_t* poke_4= calloc(1,sizeof(pokemon_t));
    if(!poke_4)
      return;
    poke_4->nombre = "Venusaur";
    poke_4->nivel = 95;


    pokemon_t* poke_5= calloc(1,sizeof(pokemon_t));
    if(!poke_5)
      return;
    poke_5->nombre = "Charmeleon";
    poke_5->nivel = 15;


    pokemon_t* poke_6= calloc(1,sizeof(pokemon_t));
    if(!poke_6)
      return;
    poke_6->nombre = "Squirtle";
    poke_6->nivel = 4;

    pokemon_t* poke_7= calloc(1,sizeof(pokemon_t));
    if(!poke_7)
      return;
    poke_7->nombre = "Wartortle";
    poke_7->nivel = 48;

    pokemon_t* poke_8= calloc(1,sizeof(pokemon_t));
    if(!poke_8)
      return;
    poke_8->nombre = "Bulbasaur";
    poke_8->nivel = 24;

    pokemon_t* poke_9= calloc(1,sizeof(pokemon_t));
    if(!poke_9)
      return;
    poke_9->nombre = "Chartie";
    poke_9->nivel = 8;

    pokemon_t* poke_10= calloc(1,sizeof(pokemon_t));
    if(!poke_10)
      return;
    poke_10->nombre = "Metapod";
    poke_10->nivel = 1;

    heap_insertar(heap_poke,poke_1);
    heap_insertar(heap_poke,poke_2);
    heap_insertar(heap_poke,poke_3);
    heap_insertar(heap_poke,poke_4);
    heap_insertar(heap_poke,poke_5);
    heap_insertar(heap_poke,poke_6);
    heap_insertar(heap_poke,poke_7);
    heap_insertar(heap_poke,poke_8);
    heap_insertar(heap_poke,poke_9);
    heap_insertar(heap_poke,poke_10);


    pa2m_afirmar(heap_vacio(heap_poke)== false, "El heap no se encuentra vacio exitosamente");
    pa2m_afirmar(heap_cantidad(heap_poke)==10,"El heap tiene 10 elementos insertados exitosamente");

    heap_con_cada_elemento(heap_poke,mostrar_elemento_nombre);
    heap_destruir(heap_poke);
    destruir_pokemon_pruebas(poke_1);
    destruir_pokemon_pruebas(poke_2);
    destruir_pokemon_pruebas(poke_3);
    destruir_pokemon_pruebas(poke_4);
    destruir_pokemon_pruebas(poke_5);
    destruir_pokemon_pruebas(poke_6);
    destruir_pokemon_pruebas(poke_7);
    destruir_pokemon_pruebas(poke_8);
    destruir_pokemon_pruebas(poke_9);
    destruir_pokemon_pruebas(poke_10);
    
    
    


    
}
/*--------------------------------------------------PRUEBAS SIMULADOR -----------------------------------------------------------------------------------*/
void simulador_evento_atender_entrendador(){

    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,"ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);

    simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
    simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
    pa2m_afirmar(simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL),"Atiende a varios entrenadores");
   
    simulador_destruir(simulador);

}

void simulador_obtener_estadisticas(){

    hospital_t* h = hospital_crear();
    simulador_t* simulador = simulador_crear(h);

    EstadisticasSimulacion estadisticas;
    ResultadoSimulacion res = simulador_simular_evento(simulador,ObtenerEstadisticas,&estadisticas);

    pa2m_afirmar((res = ExitoSimulacion) && (estadisticas.cantidad_eventos_simulados ==1),"Obtengo las estadisticas del simulador correctamente");
    pa2m_afirmar((estadisticas.entrenadores_totales == 0) && (estadisticas.entrenadores_totales == 0),"Al obtener las estadisticas los resultados son 0 exitosamente");
    
    simulador_destruir(simulador);

}

void simulador_simular_Varios_Eventos_Exitosamente(){

  hospital_t* h= hospital_crear();
  hospital_leer_archivo(h,"ejemplos/varios_entrenadores.hospital");

  simulador_t* simulador = simulador_crear(h);

  ResultadoSimulacion res = simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  pa2m_afirmar(res == ExitoSimulacion,"Atiendo 1 entrenador en el hospital devuelvo Exito");

  simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);

  EstadisticasSimulacion estadisticas;
  simulador_simular_evento(simulador,ObtenerEstadisticas,&estadisticas);

  pa2m_afirmar(estadisticas.entrenadores_atendidos == 2,"Atiendo 1 entrenador la cantidad de entrenadores atendidos aumenta");
  pa2m_afirmar(estadisticas.pokemon_atendidos == 0,"No se atendio a ningun pokemon todavia");

  simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);

  simulador_simular_evento(simulador,ObtenerEstadisticas,&estadisticas);

   
  pa2m_afirmar(estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)-1,"La cantidad de pokemones es igual al total de pokemones menos el pokemon atendido exitosamente");


  InformacionPokemon informacion;
  simulador_simular_evento(simulador,ObtenerInformacionPokemonEnTratamiento,&informacion);
 
  pa2m_afirmar(!strcmp(informacion.nombre_pokemon,"rampardos"),"Coincide el primer pokemon a atender exitosamente");
  pa2m_afirmar(!strcmp(informacion.nombre_entrenador,"lucas"),"Coincide el entrenador del pokemon atendido");
  

  Intento intento;
  intento.nivel_adivinado =10;
  
  pa2m_afirmar(simulador_simular_evento(simulador,AdivinarNivelPokemon,&intento)==ExitoSimulacion,"Adivino el nivel");
  pa2m_afirmar(!strcmp(intento.resultado_string,"¡Acertaste el nivel!"),"Coincide con el mensaje al usuario dado que adivino el nivel");

  
  pa2m_afirmar(estadisticas.entrenadores_totales == 5,"Son 5 entrenadores totales exitosamente");
  pa2m_afirmar(estadisticas.pokemon_en_espera == 23, "Se encuentran 23 pokemones en espera exitosamente");
  pa2m_afirmar(estadisticas.pokemon_totales == 24,"La cantidad de pokemones en totales son 24 exitosamente");
  pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 7,"Se simulo exitosamente 7 eventos");
  
  
  InformacionDificultad info;
  info.id = 0;
  simulador_simular_evento(simulador,ObtenerInformacionDificultad,&info);
  pa2m_afirmar(info.id == 0,"El id de la dificultad es 0 exitosamente");
  pa2m_afirmar(!strcmp(info.nombre_dificultad,"Facil"),"Coincide el nombre de la dificultad respecto al id--> Facil");
  pa2m_afirmar(info.en_uso == false,"Se encuentra la dificultad en uso");
 

  simulador_destruir(simulador);

  

}


void simulador_simular_evento_Obtener_Informacion(){

  hospital_t* h= hospital_crear();
  hospital_leer_archivo(h,"ejemplos/un_entrenador.hospital");

  simulador_t* simulador = simulador_crear(h);

  ResultadoSimulacion res = simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  pa2m_afirmar(res == ExitoSimulacion,"Atiendo 1 entrenador en el hospital devuelvo Exito");

  InformacionPokemon informacion;
  simulador_simular_evento(simulador,ObtenerInformacionPokemonEnTratamiento,&informacion);
 
  pa2m_afirmar(!strcmp(informacion.nombre_pokemon,"lapras"),"Coincide el primer pokemon a atender exitosamente--> NOMBRE POKEMON: lapras");
  pa2m_afirmar(!strcmp(informacion.nombre_entrenador,"Mariano"),"Coincide el entrenador del pokemon atendido ---> NOMBRE ENTRENADOR: Mariano");

  Intento intento;
  intento.nivel_adivinado =20;
  
  pa2m_afirmar(simulador_simular_evento(simulador,AdivinarNivelPokemon,&intento)==ExitoSimulacion,"No, Adivino el nivel");
  pa2m_afirmar(!strcmp(intento.resultado_string,"¡Fallaste no adivinaste el nivel!"),"Coincide con el mensaje al usuario dado que no adivino el nivel");

  EstadisticasSimulacion estadisticas;
  simulador_simular_evento(simulador,ObtenerEstadisticas,&estadisticas);

  printf("Puntos pokemon:%d\n",estadisticas.puntos);
  printf("POkemon atendido:%d\n",estadisticas.pokemon_atendidos);

  Intento intento_2;
  intento_2.nivel_adivinado =40;
  
  pa2m_afirmar(simulador_simular_evento(simulador,AdivinarNivelPokemon,&intento_2)==ExitoSimulacion,"Adivino el nivel");
  pa2m_afirmar(!strcmp(intento_2.resultado_string,"¡Acertaste el nivel!"),"Coincide con el mensaje al usuario dado que adivino el nivel");

  EstadisticasSimulacion estadisticas_2;
  simulador_simular_evento(simulador,ObtenerEstadisticas,&estadisticas_2);

  printf("Puntos pokemon:%d\n",estadisticas_2.puntos);
  printf("Pokemon atendido:%d\n",estadisticas_2.pokemon_atendidos);

  /* Atiendo otro pokemon*/
  simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  pa2m_afirmar(res == ExitoSimulacion,"Atiendo 1 entrenador en el hospital devuelvo Exito");

  InformacionPokemon informacion_2;
  simulador_simular_evento(simulador,ObtenerInformacionPokemonEnTratamiento,&informacion_2);
 
  pa2m_afirmar(!strcmp(informacion_2.nombre_pokemon,"pikachu"),"Coincide el primer pokemon a atender exitosamente--> NOMBRE POKEMON: pikachu");
  pa2m_afirmar(!strcmp(informacion_2.nombre_entrenador,"Mariano"),"Coincide el entrenador del pokemon atendido ---> NOMBRE ENTRENADOR: Mariano");

  
  
  simulador_destruir(simulador);
  


  

}

void simulador_simular_evento_agregar_dificultad(){

  hospital_t* h= hospital_crear();
  hospital_leer_archivo(h,"ejemplos/varios_entrenadores.hospital");

  simulador_t* simulador = simulador_crear(h);

  ResultadoSimulacion res = simulador_simular_evento(simulador,AtenderProximoEntrenador,NULL);
  pa2m_afirmar(res == ExitoSimulacion,"Atiendo 1 entrenador en el hospital devuelvo Exito");

  InformacionDificultad dificultad_nueva;
  dificultad_nueva.nombre_dificultad = "Nivel Profesional";
  dificultad_nueva.id = 3;
  dificultad_nueva.en_uso = false;
  simulador_simular_evento(simulador,AgregarDificultad,&dificultad_nueva);

  InformacionDificultad info_nueva;
  info_nueva.id = 3;
  simulador_simular_evento(simulador,ObtenerInformacionDificultad,&info_nueva);
  pa2m_afirmar(info_nueva.id == 3,"El id de la dificultad es 3 exitosamente");
  pa2m_afirmar(!strcmp(info_nueva.nombre_dificultad,"Nivel Profesional"),"Coincide el nombre de la dificultad respecto al id--> Nivel Profesional");
 
  
  Intento intento;
  intento.nivel_adivinado =20;
  intento.resultado_string = "No adivinaste el nivel profesional";
  
  pa2m_afirmar(simulador_simular_evento(simulador,AdivinarNivelPokemon,&intento)==ExitoSimulacion,"NO,Adivino el nivel");

  int id_dificultad_activa = 3;
  simulador_simular_evento(simulador,SeleccionarDificultad,&id_dificultad_activa);

  pa2m_afirmar(simulador->actual_id == 3,"La dificultad seleccionada es el id 3");
  
  simulador_destruir(simulador);


}



int main(){

    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();

    pa2m_nuevo_grupo("PRUEBAS ACTUALIZADAS PARA EL NUEVO HOSPITAL-------------------------------");
    
    pa2m_nuevo_grupo("PRUEBAS DE HEAP CREACION");
    Pruebas_Creacion_Heap_Minimal();

    pa2m_nuevo_grupo("PRUEBAS DE DISTINTAS OPERACIONES-HEAP MINIMAL");
    Prueba_Pokemones_Heap();

    pa2m_nuevo_grupo("PRUEBAS DE EVENTO ATENDER ENTRENADOR");
    simulador_evento_atender_entrendador();

    pa2m_nuevo_grupo("PRUEBAS DE EVENTO OBTENER ESTADISTICAS");
    simulador_obtener_estadisticas();

    pa2m_nuevo_grupo("PRUEBAS DE EVENTO OBTENER INFORMACION POKEMON");
    simulador_simular_evento_Obtener_Informacion();

    pa2m_nuevo_grupo("PRUEBAS DE EVENTO AGREGAR DIFICULTAD");
    simulador_simular_evento_agregar_dificultad();

    pa2m_nuevo_grupo("PRUEBAS DE VARIOS EVENTOS");
    simulador_simular_Varios_Eventos_Exitosamente();

    
   
   

    return pa2m_mostrar_reporte();
}
