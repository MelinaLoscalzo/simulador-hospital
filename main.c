#include "src/hospital.h"
#include "src/simulador.h"
#include "src/estructuras.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LIGHT_BLUE "\x1b[94m"
#define YELLOW     "\x1b[33m"

#define GREEN "\x1b[32m"
#define LIGHT_GREEN "\x1b[92m"
#define LIGHT_RED "\x1b[91m"
#define BLUE "\x1B[34m"
#define fondo "\x1B[40m"

#define FIN   "\x1b[0m"
#define LETRA "\x1b[1m"

#define MINI_MARGEN     "			"

typedef struct{
    simulador_t* simulador;
    bool jugando;
}Juego;


char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}



unsigned int calcular_puntaje_juego(unsigned cantidad_intentos){

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
int verificar_nivel_juego(unsigned nivel_adivinado, unsigned nivel_pokemon){
    
    return (int)nivel_pokemon-(int) nivel_adivinado;
}
const char* verificacion_a_string_juego(int resultado_verificacion){
    
    if(resultado_verificacion == 0)
        return  "¬°Acertaste el nivel!";
    return  "¬°Fallaste no adivinaste el nivel!";
}


void mostrar_logo(Juego* juego){

        system("clear");
        printf(("\n"));

        printf(LETRA YELLOW MINI_MARGEN"                         /$$   /$$ /$$$$$$$  /$$$$$$$$ /$$$$$$  /$$                        \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$  | $$| $$__  $$|__  $$__//$$__  $$| $$                         \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$  | $$| $$  | $$   | $$  | $$  | $$| $$       /$$                \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$$$$$$$| $$$$$$$/   | $$  | $$$$$$$$| $$      |__/                \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$__  $$| $$____/    | $$  | $$__  $$| $$                          \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$  | $$| $$         | $$  | $$  | $$| $$       /$$                \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        | $$  | $$| $$         | $$  | $$  | $$| $$$$$$$$|__/                \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                        |__/  |__/|__/         |__/  |__/  |__/|________/                     \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                                                                                              \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                                                                                               \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"                                                                                               \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"  /$$$$$$  /$$                         /$$                 /$$                                 \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN" /$$__  $$|__/                        | $$                | $$                                  \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"| $$  |__/ /$$ /$$$$$$/$$$$  /$$   /$$| $$  /$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$              \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"|  $$$$$$ | $$| $$_  $$_  $$| $$  | $$| $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$             \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"$____  $$| $$| $$ / $$ / $$| $$  | $$| $$  /$$$$$$$| $$  | $$| $$  / $$| $$  /__/              \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"    /$$  / $$| $$| $$ | $$ | $$| $$  | $$| $$ /$$__  $$| $$  | $$| $$  | $$| $$                \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"   |  $$$$$$/| $$| $$ | $$ | $$|  $$$$$$/| $$|  $$$$$$$|  $$$$$$$|  $$$$$$/| $$                 \n"FIN);
        printf(LETRA YELLOW MINI_MARGEN"   |______/ |__/|__/ |__/ |__/ /______/ |__/ /_______/ /_______/ /______/ |__/                  \n\n"FIN);
                                                                                                                                            

        
        printf(("\n"));
        printf((MINI_MARGEN LETRA YELLOW  "‚ēĒ‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēó\n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                      |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                     /|                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                   ,' |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                  .   |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                    | |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                 ' '| |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                / / | |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ       _,.-\"\"--._              / /  | |                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ     ,'          `.           j '   ' '                                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ   ,'              `.         ||   / ,                         ___..--,               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ  /                  \\        ' `.'`.-.,-\".  .       _..---\"\"'' __, ,'                ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ /                    \\        \\` .\"`      `\"'\\   ,'\"_..--''\"\"\"'.'.'                  ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ.                      .      .'-'             \\,' ,'         ,','                    ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ|                      |      ,`               ' .`         .' /                      ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ|                      |     /          ,\"`.  ' `-. _____.-' .'                       ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ'                      |..---.|,\".      | | .  .-'\"\"   __.,-'                         ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ .                   ,'       ||,|      |.' |    |\"\"`'\"                               ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ  `-._   `._.._____  |        || |      `._,'    |                                    ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ      `.   .       `\".     ,'\"| \"  `'           ,+.                                   ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ        \\  '         |    '   |   .....        .'  `.                                 ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ         .'          '     7  \".              ,'     \\                                ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                   ,'      |    `..        _,'      F                                 ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                  .        |,      `'----''         |                                 ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                  |      ,\"j  /                   | '                                 ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                  `     |  | .                 | `,'                                  ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                   .    |  `.|                 |/                                     ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                    `-..'   ,'                .'                                      ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                            | \\             ,''                                       ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                            |  `,'.      _,' /                                        ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                            |    | ^.  .'   /                                         ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                             `-'.' j` V    /                                          ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                   |      /                                           ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                   |     /                                            ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                   |   ,'                                             ‚ēĎ \n"FIN));
        
        printf(( MINI_MARGEN LETRA YELLOW  "‚ēĎ                                   `\"\"                                                ‚ēĎ \n"FIN));
        
        printf((MINI_MARGEN LETRA YELLOW "‚ēö‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēĚ\n\n"FIN));
        printf("\n");
    fflush(stdout);
   
}


                                                                                                                               


void mostrar_estadisticas(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);
    printf(LETRA LIGHT_BLUE" ‚ě£ Estadisticas" FIN);
    printf("\n");
    printf(LETRA"‚ú¶ Entrenadores Atendidos:%u\n"FIN,e.entrenadores_atendidos);
    printf(LETRA"‚ú¶ Entrenadores Totales:%u\n"FIN,e.entrenadores_totales);
    printf(LETRA"‚ú¶ Pokemones Atendidos:%u\n"FIN,e.pokemon_atendidos);
    printf(LETRA"‚ú¶ Poemones en espera:%u\n"FIN,e.pokemon_en_espera);
    printf(LETRA"‚ú¶ Pokemones Totales:%u\n"FIN,e.pokemon_totales);
    printf(LETRA"‚ú¶ Puntos:%u\n"FIN,e.puntos);
    printf(LETRA"‚ú¶ Cantidad de eventos simulados:%u\n"FIN,e.cantidad_eventos_simulados);


}


void atender_proximo_entrenador(Juego* juego){
    ResultadoSimulacion atendido;
    simulador_simular_evento(juego->simulador,AtenderProximoEntrenador,&atendido);
}

void Obtener_Informacion_Pokemon_En_Tratamiento(Juego* juego){

    InformacionPokemon info;
    simulador_simular_evento(juego->simulador,ObtenerInformacionPokemonEnTratamiento,&info);
    printf(LETRA LIGHT_BLUE"‚ě£ Ficha Medica" FIN);
    printf("\n");

    printf(LETRA"‚úÖ Nombre Pokemon:%s\n"FIN,info.nombre_pokemon );
    printf(LETRA"‚úÖ Nombre Entrenador:%s\n"FIN,info.nombre_entrenador);
}

void Adivinar_Nivel_Pokemon(Juego* juego){
    Intento intento;
    intento.nivel_adivinado = 0;

    printf(LETRA LIGHT_GREEN"Nivel Pokemon:"FIN);
    scanf("%d",&intento.nivel_adivinado);
     
    simulador_simular_evento(juego->simulador,AdivinarNivelPokemon,&intento);
   
    printf(LETRA LIGHT_GREEN "Mensaje:%s\n"FIN,intento.resultado_string);
  

    
    

}

void Agregar_Dificultad(Juego* juego){
   
    DatosDificultad dificultad_nueva;
    dificultad_nueva.nombre = "Pikachu";
    dificultad_nueva.calcular_puntaje = calcular_puntaje_juego;
    dificultad_nueva.verificar_nivel = verificar_nivel_juego;
    dificultad_nueva.verificacion_a_string = verificacion_a_string_juego;

    simulador_simular_evento(juego->simulador,AgregarDificultad,&dificultad_nueva);
    printf(LETRA LIGHT_RED "Dificultad Agregada:%s\n"FIN,dificultad_nueva.nombre);


    DatosDificultad dificultad_nueva_2;
    dificultad_nueva_2.nombre = "Profesional";
    dificultad_nueva_2.calcular_puntaje = calcular_puntaje_juego;
    dificultad_nueva_2.verificar_nivel = verificar_nivel_juego;
    dificultad_nueva_2.verificacion_a_string = verificacion_a_string_juego;

    simulador_simular_evento(juego->simulador,AgregarDificultad,&dificultad_nueva_2);
    printf(LETRA LIGHT_RED"Dificultad Agregada:%s\n"FIN,dificultad_nueva_2.nombre);
 


   
}

void Seleccionar_Dificultad(Juego* juego){

    int id_dificultad_activa = 0;
   
    printf(LETRA"Id dificultad:"FIN);
    scanf("%d",&id_dificultad_activa);

    simulador_simular_evento(juego->simulador,SeleccionarDificultad,&id_dificultad_activa);
    printf(LETRA"Dificultad seleccionada:%s\n"FIN,((InformacionDificultad*)lista_elemento_en_posicion(juego->simulador->dificultades,(size_t)id_dificultad_activa))->nombre_dificultad);
}

bool imprimir_dificultad_lista(void* dificultad, void* contexto){

    if(!dificultad || !contexto) return false;
  
    InformacionDificultad* dificultad_activa = (InformacionDificultad*)dificultad;
   
    printf(LETRA"\t Id:%lu - Modo: %s\n"FIN, (*(size_t*)contexto)++,dificultad_activa->nombre_dificultad);
   
    
      
    return true;
}

void Obtener_Informacion_Dificultad(Juego* juego){

    InformacionDificultad info;   
    info.id =juego->simulador->actual_id;
    
    simulador_simular_evento(juego->simulador,ObtenerInformacionDificultad,&info);
    printf(LETRA LIGHT_BLUE "‚ě£ Dificultades"FIN);
    printf(("\n"));
    printf( "‚ēĒ‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēó\n");
    for(size_t i=0;i<lista_tamanio(juego->simulador->dificultades);i++){
        lista_con_cada_elemento(juego->simulador->dificultades,imprimir_dificultad_lista,&i);
        printf(LETRA GREEN"Esta en uso Modo:%s\n"FIN,info.nombre_dificultad);
        
    }
    printf("‚ēö‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēź‚ēĚ\n");
    

}


void Finalizar_simulacion(Juego* juego){
    simulador_simular_evento(juego->simulador,FinalizarSimulacion,NULL);
    juego->jugando = false;
    
   
}


void mostrar_ayuda(){


    printf(LETRA YELLOW"‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ Ayuda ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ"FIN);
    printf("\n");

    printf(LETRA"ūüĒĻ [e] Mostrar Estadisticas:" FIN" Muestra el estado actual de la partida,con los datos del Hospital \n");
    printf(LETRA"ūüĒĻ [p] Atender Proximo Entrenador:"FIN "Carga los pokemones del siguiente entrenador que se encuentra en espera al consultorio.\n");
    printf(LETRA"ūüĒĻ [i] Mostrar Pokemon en Tratamiento:" FIN "Muestra la ficha medica del pokemon que esta en el consultorio.\n");
    printf(LETRA"ūüĒĻ [a] Adivinar Nivel Pokemon:"FIN "Intenta ver si puedes lograr  adivinar el nivel del pokemon con la dificultad activa.\n");
    printf(LETRA"ūüĒĻ [n] Agregar Dificultades:" FIN "Carga al juego dos dificultades para super un mejor nivel.\n");
    printf(LETRA"ūüĒĻ [o] Mostrar Dificultades Disponibles:" FIN "Muestra un listado de todas las dificultades disponibles.\n");
    printf(LETRA"ūüĒĻ [d] Cambiar Dificultad:"FIN "Cambia la dificultad activa, a alguna dificultad del listado disponible.\n");
    printf(LETRA"ūüĒĻ [m] Ayuda:" FIN "Indica como funciona cada comando.\n");
    printf(LETRA"ūüĒĻ [q] Salir\n");



}
void mostrar_menu(){

    printf(MINI_MARGEN LETRA YELLOW"‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ Menu ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ ‚ėÖ"FIN);
    printf("\n");

    printf(MINI_MARGEN LETRA"ūüĒĻ [e] Mostrar Estadisticas\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [p] Atender Proximo Entrenador\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [i] Mostrar Pokemon en Tratamiento\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [a] Adivinar Nivel Pokemon\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [n] Agregar Dificultades\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [o] Mostrar Dificultades Disponibles\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [d] Cambiar Dificultad\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [m] Ayuda\n"FIN);
    printf(MINI_MARGEN LETRA"ūüĒĻ [q] Salir\n"FIN);

}



void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case 'q': //Salir del juego
            Finalizar_simulacion(juego);
            break;
        case 'e':
            mostrar_estadisticas(juego);
            break;
        case 'p':
            atender_proximo_entrenador(juego);
            break;
        case 'i':
            Obtener_Informacion_Pokemon_En_Tratamiento(juego);
            break;
        case 'a':
            Adivinar_Nivel_Pokemon(juego);
            break;
        case 'd':
            Seleccionar_Dificultad(juego);
            break;
        case 'n':
           Agregar_Dificultad(juego);
           break;
        case 'o':
           Obtener_Informacion_Dificultad(juego);
           break;
        case 'm':
           mostrar_ayuda(juego);
           break;
        default: 
            break;
    }
}

Juego* inicializar_juego(Juego* juego,hospital_t* hospital){

    if(!juego || !hospital) return NULL;
    
   
    juego->simulador = simulador_crear(hospital);
    if(!juego->simulador){
        free(juego->simulador);
        return NULL;
    }
    
    return juego;
}

void destruir_juego(Juego* juego){

    if(!juego) return;
       
    simulador_destruir(juego->simulador);

}


int main(int argc, char *argv[]) {

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    inicializar_juego(&juego, hospital);
  
    mostrar_logo(&juego);
    
    mostrar_menu();
    juego.jugando = true;

    do{
        printf("> ");
        char c = leer_comando();
        ejecutar_comando(&juego, c);   
        
    }while(juego.jugando);
    
    destruir_juego(&juego);
    
    return 0;
}
