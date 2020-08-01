#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "../constantes.h"
#include "defendiendo_torres.h"

#include "../utiles/pedir_datos.h" 
#include "../utiles/etiquetas.h"
#include "../utiles/utiles.h"

#include "../funcionalidades/configuracion.h"
#include "../funcionalidades/grabacion.h"
#include "../funcionalidades/caminos.h"
#include "../funcionalidades/ranking.h"
#include "../funcionalidades/animos.h"


//  CONSTANTES DE JUEGO (!)
    static const char NADIE = 'N';

    static const int MULTIPLICADOR_PUNTAJE = 1000;

    static const int ESTADO_JUGANDO = 0;
    static const int ESTADO_GANADO  = 1;
    //static const int ESTADO_PERDIDO =-1;

    static const char CONFIRMAR ='S';
    static const char CANCELAR ='X';

    #define CANTIDAD_NIVELES  4

    static const int RESISTENCIA_ORCO  = 200;
    static const int RESISTENCIA_ORCO_RAND  = 100;

    static const int RESISTENICA_TORRE_INICIAL = 600;

    static const float FACTOR_ESPERA_NUEVO_NIVEL = 3;

    static const int VIENTO_INICIAL = 50;
    static const int HUMEDAD_INICIAL= 50;
    static const char ANIMO_INICIAL = 'B';

    /*
     * Tipo de dato que almacena las caracteristicas que se utilizan 
     *  generar un nivel
     */
    typedef struct caracteristicas_nivel{
        
        // numero de nivel
        int num;

        // dimension del tablero (cuadrado) del nivel
        int dimension;

        // direccion cardinal (Norte,Sur,Este,Oeste) de donde provienen los enemigos
        char entrada;

        // Indica que torres/caminos se hallaran en el nivel
        bool torre_1, torre_2;

        // Cantidades de enanos y elfos iniciales en el nivel
        // orcos a entrar en el nivel
        int enanos, elfos, orcos;

        int costo_enanos_extra[2];
        int costo_elfos_extra[2];

        // Turnos cada los cuales se podra agregar un defensor extra
        int turnos_bonus;
    } caracteristicas_nivel_t;

    #define ENTRADA_ESTE  'E'
    #define ENTRADA_OESTE 'O'
    #define ENTRADA_NORTE 'N'
    #define ENTRADA_SUR   'S'

    const caracteristicas_nivel_t PRUEBA ={ 
        .num = 1,
        .dimension = 15,
        .entrada = ENTRADA_ESTE,
        .torre_1 = true, .torre_2 = false,
        .enanos = 1, .elfos = 0, 
        .orcos = 1,
        .turnos_bonus = 25
    };

    const caracteristicas_nivel_t NIVEL_1 ={ 
        .num = 1,
        .dimension = 15,
        .entrada = ENTRADA_ESTE,
        .torre_1 = true, .torre_2 = false,
        .enanos = 5, .elfos = 0, 
        .orcos = 100,
        .turnos_bonus = 25
    };

    const caracteristicas_nivel_t NIVEL_2 ={ 
        .num = 2,
        .dimension = 15,
        .entrada = ENTRADA_OESTE,
        .torre_1 = false, .torre_2 = true,
        .enanos = 0, .elfos = 5, 
        .orcos = 200,
        .turnos_bonus = 50
    };

    const caracteristicas_nivel_t NIVEL_3 ={ 
        .num = 3,
        .dimension = 20,
        .entrada = ENTRADA_NORTE,
        .torre_1 = true, .torre_2 = true,
        .enanos = 3, .elfos = 3, 
        .orcos = 300,
        .turnos_bonus = 50/2
    };

    const caracteristicas_nivel_t NIVEL_4 ={ 
        .num = 4,
        .dimension = 20,
        .entrada = ENTRADA_SUR,
        .torre_1 = true, .torre_2 = true,
        .enanos = 4, .elfos = 4, 
        .orcos = 500,
        .turnos_bonus = 50/2
    };

    const int MAX_INTENTOS = 20;
//  CONSTANTES DE JUEGO (¡)

// ETIQUETAS

    // Declaracion de funciones para cargar componentes del juego
        void cargar_configuracion
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//1
        void cargar_grabar
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//2
    // Declaracion de funciones para cargar componentes del juego


    #define CANT_ETQ_JUGAR 2
    static const etiqueta_t ETIQUETAS [CANT_ETQ_JUGAR] = {
        {
            .indice = 0,
            .etiqueta = "config",
            .cargar = cargar_configuracion
        },
        {
            .indice = 1,
            .etiqueta = "grabacion",
            .cargar = cargar_grabar
        }
    };
// ETIQUETAS

// HEADER JUEGO (!)
    /* 
     * Comienza un nuevo juego
     * pre: recibe un juego inicializado
     */
    static void nuevo_juego( juego_t* juego , configuracion_t configuracion );  

    /*
     * Devuelve un nivel_t segun el nivel(numero)
     *  0 > Entrada Este
     *  1 > Entrada Oeste
     *  2 > Entrada Norte
     *  3 > Entrada Sur
     *  Default > Nivel vacio
     */
    static nivel_t nuevo_nivel( int nivel, configuracion_t configuracion, caminos_t* caminos );

    // Generan los caminos del nivel segun sus caracteristicas y la coniguracion
    static void generar_caminos( nivel_t* nivel, caminos_t* caminos );
    static void generar_camino_1( nivel_t* nivel, caracteristicas_nivel_t caracteristicas_nivel , configuracion_t configuracion, bool cruzado );
    static void generar_extremos_camino_1( coordenada_t* entrada, coordenada_t* torre,
        caracteristicas_nivel_t caracteristicas_nivel, bool cruzado );
    static void generar_camino_2( nivel_t* nivel, caracteristicas_nivel_t caracteristicas_nivel , configuracion_t configuracion, bool cruzado );
    static void generar_extremos_camino_2( coordenada_t* entrada, coordenada_t* torre,
        caracteristicas_nivel_t caracteristicas_nivel, bool cruzado );
    
    // Genera un camino segun los parametros
    static void generar_camino(coordenada_t camino[MAX_LONGITUD_CAMINO], 
        int* tope_camino, coordenada_t entrada, coordenada_t torre, 
        int dimension, configuracion_t configuracion );

    // Muestra un mensaje al pasar de nivel
    static void mensaje_nuevo_nivel( int nivel );

    // Muestra un mensaje al finalizar un juego (ganar o perder)
    static void mensaje_terminar_juego( int estado );

    // Pide al usuario que coloque defensores segun las especificaciones del nivel
    static void agregar_defensores( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  );

    // Coloqua defensores segun las especificaciones del nivel
    static void auto_agregar_defensores( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  );

    // Pide al usuario que coloque un defensor segun las especificaciones del nivel
    static void pedir_un_defensor( juego_t* juego, 
        caracteristicas_nivel_t caracteristicas_nivel, char tipo, int* k );
    
    // Pide al usuario que coloque un defensor extra segun las especificaciones del nivel
    static void agregar_defensores_bonus( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  );

    // Subrutinas de agregar_defensores_bonus()
    static void obtener_tipo_defensor_bonus( juego_t juego, char* tipo,
        caracteristicas_nivel_t caracteristicas_nivel );
    static void colocar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel );
    
    // Coloca automaticamente un defensor extra segun las especificaciones del nivel
    static void auto_agregar_defensores_bonus( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  );

    // Subrutinas de auto_agregar_defensores_bonus()
    static void auto_obtener_tipo_defensor_bonus( juego_t* juego, char* tipo,
        caracteristicas_nivel_t caracteristicas_nivel );
    static void auto_colocar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel );

    // Decrementa la resistencia de las torres segun las caracteristicas del nivel
    //  y el defensor colocado
    static void cobrar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel );

    // Devuelve las especificaciones del nivel pedido
    static caracteristicas_nivel_t buscar_caracteristicas_nivel( int nivel );

    // Aplica acciones extra al comenzar un nivel segun la configuracion
    static void bonus_nuevo_nivel( juego_t* juego , configuracion_t configuracion );

    // pasa el juego de nivel, 
    // aplicando los bonus correspoindientes a la configuracion
    static void pasar_de_nivel( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t* caracteristicas_nivel, caminos_t* caminos  );

    // Carga las caracteristicas correspondientes de la configuracion en las caracteristicas del nivel
    void cargar_caracteristicas_nivel( configuracion_t configuracion, 
        caracteristicas_nivel_t* caracteristicas_nivel );

    // pasa el nivel de turno (se juega un turno), 
    // aplicando los bonus correspoindientes a la configuracion
    static void pasar_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  );

    // aplica al juego los bonus que corresponden previo a jugar un turno
    static void bonus_pre_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  );

    // aplica al juego los bonus que corresponden posterior a jugar un turno
    static void bonus_post_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  );

    // devuelve el puntaje del juego segun la configuracion
    static int puntaje( juego_t juego, configuracion_t configuracion );
// HEADER JUEGO (¡)

// jugar juego segun la configuracion
static void jugar_juego( configuracion_t configuracion ){

    int viento = VIENTO_INICIAL;
    int humedad = HUMEDAD_INICIAL;
    char animo_legolas = ANIMO_INICIAL;
    char animo_gimli = ANIMO_INICIAL;

    juego_t juego;

    if( 
        pedir_bool( " Desea iniciar animos? " ) 
    ){
        animos(&viento , &humedad , &animo_legolas , &animo_gimli);
        configuracion.enanos_animo[0] = prob_fallo(humedad);
        configuracion.enanos_animo[1] = prob_critico(animo_gimli);
        configuracion.elfos_animo[0] = prob_fallo(viento);
        configuracion.elfos_animo[1] = prob_critico(animo_legolas);
    }

    configuracion.auto_defensores = !pedir_bool( " Desea colocar los defensores manualmente ? " );

    inicializar_juego(&juego, configuracion);
    nuevo_juego( &juego , configuracion );

    return;
}

void jugar( int argc , char *argv [] ){

    configuracion_t configuracion = CONFIGURACION_STANDAR;
    etiqueta_t etiqueta_de_juego;
    char etiqueta [MAX_NOMBRE], lectura [MAX_NOMBRE];

    for( int i = 2; i < argc; i++ ){

        sscanf( argv[i], "%[^=]=%s", etiqueta, lectura );

        etiqueta_de_juego = buscar_etiqueta( etiqueta,
                ETIQUETAS, CANT_ETQ_CONFIG );

        etiqueta_de_juego.cargar(&configuracion,lectura);

    }

    jugar_juego( configuracion );

    return;
}

// JUEGO (!)

    void nuevo_juego( juego_t* juego , configuracion_t configuracion ){

        int turno = 0;
        caracteristicas_nivel_t caracteristicas_nivel;
        caminos_t caminos;
        if( obtener_caminos( &caminos, configuracion.caminos ) == INVALIDO )
            return;

        juego->nivel_actual = 0;
        juego->nivel.tope_enemigos = 0;
        juego->nivel = nuevo_nivel( juego->nivel_actual, configuracion, &caminos );

        bool grabar = strcmp(configuracion.grabacion,"");
        nombre_archivo_t grabacion;
        strcpy(grabacion,configuracion.grabacion);
        cargar_ruta_grabacion(grabacion);
        FILE* archivo_grabacion = NULL;
        if( grabar ){
            archivo_grabacion = fopen(grabacion,"wb");
        }

        while( estado_juego( *juego) == ESTADO_JUGANDO ){

            if( estado_nivel( juego->nivel ) == ESTADO_GANADO  ){
                
                turno = 0;
                pasar_de_nivel( juego , configuracion, &caracteristicas_nivel , &caminos);
                
            }else{
                
                pasar_turno( juego , configuracion, 
                    caracteristicas_nivel, turno  );
                
                turno ++;

                if( configuracion.velocidad > 0 ){
                    
                    mostrar_juego( *juego );
                    detener_el_tiempo( configuracion.velocidad );
                }
                if( grabar ){
                    fwrite( juego, sizeof(juego_t), 1,archivo_grabacion );
                }
            }
        }

        fclose( archivo_grabacion );

        mensaje_terminar_juego( estado_juego( *juego) );

        guardar_puntaje( puntaje( *juego , configuracion ) ,configuracion);
    }

    void pasar_de_nivel( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t* caracteristicas_nivel, caminos_t* caminos  ){

        juego->nivel_actual ++;
        *caracteristicas_nivel = buscar_caracteristicas_nivel( juego->nivel_actual );
        cargar_caracteristicas_nivel( configuracion,caracteristicas_nivel );

        juego->nivel = nuevo_nivel( juego->nivel_actual, configuracion, caminos );
        mensaje_nuevo_nivel( juego->nivel_actual );

        bonus_nuevo_nivel( juego , configuracion );

        if( (juego->nivel_actual <= CANTIDAD_NIVELES) && (juego->nivel.max_enemigos_nivel > 0) ){

                if( configuracion.auto_defensores )
                auto_agregar_defensores( juego, *caracteristicas_nivel );
            else
                agregar_defensores( juego, *caracteristicas_nivel );
        }

        mostrar_juego( *juego );
        detener_el_tiempo( configuracion.velocidad * FACTOR_ESPERA_NUEVO_NIVEL );
    }

    void cargar_caracteristicas_nivel( configuracion_t configuracion, 
        caracteristicas_nivel_t* caracteristicas_nivel ){

        caracteristicas_nivel->costo_enanos_extra[0] = configuracion.enanos_extra[1];
        caracteristicas_nivel->costo_enanos_extra[1] = configuracion.enanos_extra[2];
        caracteristicas_nivel->costo_elfos_extra[0] = configuracion.elfos_extra[1];
        caracteristicas_nivel->costo_elfos_extra[1] = configuracion.elfos_extra[2];

        caracteristicas_nivel->enanos = 
            configuracion.enanos_inicio[ caracteristicas_nivel->num -1 ];

        caracteristicas_nivel->elfos = 
            configuracion.elfos_inicio[ caracteristicas_nivel->num -1 ];
    }

    void pasar_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  ){

        bonus_pre_turno( juego , configuracion, 
            caracteristicas_nivel, turno  );

        jugar_turno( juego );

        bonus_post_turno( juego , configuracion, 
            caracteristicas_nivel, turno  );
    }

    void bonus_pre_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  ){

        if( (turno > 0) && 
            (turno <= (caracteristicas_nivel.orcos / 
                (caracteristicas_nivel.torre_1*1+caracteristicas_nivel.torre_2*1) )) &&
            (turno % caracteristicas_nivel.turnos_bonus == 0) 
        ){

            if( (configuracion.auto_defensores) )
                auto_agregar_defensores_bonus( juego, caracteristicas_nivel );
            else
                agregar_defensores_bonus( juego, caracteristicas_nivel );
        }
    }

    void bonus_post_turno( juego_t* juego , configuracion_t configuracion, 
        caracteristicas_nivel_t caracteristicas_nivel, int turno  ){

        if(configuracion.invencible){
            juego->torres.resistencia_torre_1 = 
            juego->torres.resistencia_torre_2 = 
            RESISTENICA_TORRE_INICIAL;
        }
    }

    void bonus_nuevo_nivel( juego_t* juego , configuracion_t configuracion ){

        juego->torres.resistencia_torre_1 += configuracion.bonus_resistencia;
        juego->torres.resistencia_torre_2 += configuracion.bonus_resistencia;

        if( configuracion.saltear_niveles ){
            char respuesta;
            printf("\n SALTEAR NIVEL [%c]\n", CONFIRMAR );
            scanf("%c",&respuesta);

            if( toupper(respuesta) == toupper(CONFIRMAR) )
                juego->nivel.max_enemigos_nivel = 0;
         
        }
    }

    void mensaje_nuevo_nivel( int nivel ){

        system("clear");

        if( nivel-1 > 0)
            printf("\n\t HAS COMPLETADO EL NIVEL %i \n", nivel-1 );

        if( nivel <= CANTIDAD_NIVELES )
            printf("\n\t COMIENZA EL NIVEL %i \n", nivel );

        tocar_para_continuar();
    }

    void mensaje_terminar_juego( int estado ){

        system("clear");
        if( estado == ESTADO_GANADO){

            printf("\n\n");
            printf("\t =================================== \n");
            printf("\t ||                               ||\n");
            printf("\t ||  ¡¡¡ HAS GANADO EL JUEGO !!!  ||\n");
            printf("\t ||                               ||\n");
            printf("\t =================================== \n");
        
        }else{

            printf("\n\n");
            printf("\t ==================================== \n");
            printf("\t ||                                ||\n");
            printf("\t ||      HAS PERDIDO EL JUEGO      ||\n");
            printf("\t ||                                ||\n");
            printf("\t ==================================== \n");

        }

        tocar_para_continuar();
    } 

    nivel_t nuevo_nivel( int nivel , configuracion_t configuracion , caminos_t* caminos){

        nivel_t nuevo_nivel;

        nuevo_nivel.tope_camino_1 = 0;
        nuevo_nivel.tope_camino_2 = 0;
        nuevo_nivel.tope_defensores = 0;
        nuevo_nivel.tope_enemigos = 0;
        nuevo_nivel.max_enemigos_nivel = 0;
        nuevo_nivel.numero = nivel;

        caracteristicas_nivel_t caracteristicas_nivel;

        caracteristicas_nivel = buscar_caracteristicas_nivel( nivel );

        // CAMINOS       
        if( nivel <= CANTIDAD_NIVELES ){

            

            if( configuracion.caminos_random ){// TP2
                bool cruzado = ( rand()%configuracion.rareza_cruzado == 0 );
                generar_camino_1( &nuevo_nivel, caracteristicas_nivel, configuracion, cruzado);
                generar_camino_2( &nuevo_nivel, caracteristicas_nivel, configuracion, cruzado);     
            }else{

                generar_caminos( &nuevo_nivel, caminos );   
            }

            nuevo_nivel.max_enemigos_nivel = caracteristicas_nivel.orcos;
        }

        
        // ORCOS
        for(int i = 0; i<nuevo_nivel.max_enemigos_nivel; i++){
            nuevo_nivel.enemigos[i].vida= RESISTENCIA_ORCO + rand() %(RESISTENCIA_ORCO_RAND+1) ;
            nuevo_nivel.enemigos[i].camino = INVALIDO;
            nuevo_nivel.enemigos[i].pos_en_camino = INVALIDO;
        }

        return nuevo_nivel;
    }

    void generar_caminos( nivel_t* nivel, caminos_t* caminos ){

        int i, _nivel = nivel->numero -1;

        nivel->tope_camino_1 = caminos->topes[_nivel][0];
        for( i=0; i < caminos->topes[_nivel][0]; i++ ){
            nivel->camino_1[i].fil = caminos->caminos[_nivel][0][i].fil;
            nivel->camino_1[i].col = caminos->caminos[_nivel][0][i].col;
        }

        nivel->tope_camino_2 = caminos->topes[_nivel][1] ;
        for( i=0; i < caminos->topes[_nivel][1]; i++ ){
            nivel->camino_2[i].fil = caminos->caminos[_nivel][1][i].fil;
            nivel->camino_2[i].col = caminos->caminos[_nivel][1][i].col;
        }
    }
    void generar_camino(coordenada_t camino[MAX_LONGITUD_CAMINO], 
        int* tope_camino, coordenada_t entrada, coordenada_t torre, 
        int dimension, configuracion_t configuracion ){

        int intentos = 0;
        *tope_camino = 0;

        while( (*tope_camino < configuracion.complejidad *dimension) 
            && (intentos < MAX_INTENTOS) ){

            obtener_camino( camino, tope_camino, entrada, torre);
        }
    }
    void generar_camino_1( nivel_t* nivel, 
        caracteristicas_nivel_t caracteristicas_nivel , configuracion_t configuracion, bool cruzado ){
        
        int dimension = caracteristicas_nivel.dimension ;
        coordenada_t entrada,torre;
        entrada.fil=entrada.col=0;
        torre.fil=torre.col=10;

        if( caracteristicas_nivel.torre_1 ){

            generar_extremos_camino_1( &entrada, &torre,
                caracteristicas_nivel, cruzado );

            generar_camino(nivel->camino_1, &(nivel->tope_camino_1),
                entrada, torre, dimension, configuracion );

        }else
            nivel->tope_camino_1 = 0;
    }
    void generar_extremos_camino_1( coordenada_t* entrada, coordenada_t* torre,
        caracteristicas_nivel_t caracteristicas_nivel, bool cruzado ){

        int dimension = caracteristicas_nivel.dimension;
        switch( caracteristicas_nivel.entrada ){

            case ENTRADA_ESTE:
                entrada->fil = rand()%dimension;
                entrada->col = dimension-1;
                torre->fil = rand()%dimension;
                torre->col = 0;
            break;

            case ENTRADA_NORTE:
                entrada->fil = 0;
                entrada->col = rand()%(dimension/2)+(dimension/2)*(cruzado);
                torre->fil = dimension-1;
                torre->col = rand()%(dimension/2);
            break;

            case ENTRADA_SUR:
                entrada->fil = dimension-1;
                entrada->col = rand()%(dimension/2)+(dimension/2)*(cruzado);
                torre->fil = 0;
                torre->col = rand()%(dimension/2);
            break;
        }
    }  
    void generar_camino_2( nivel_t* nivel, 
        caracteristicas_nivel_t caracteristicas_nivel , configuracion_t configuracion, bool cruzado ){
         
        int dimension = caracteristicas_nivel.dimension ;
        coordenada_t entrada,torre;
        entrada.fil=entrada.col=0;
        torre.fil=torre.col=10;

        if( caracteristicas_nivel.torre_2 ){

            generar_extremos_camino_2( &entrada, &torre,
                caracteristicas_nivel, cruzado );

            generar_camino(nivel->camino_2, &(nivel->tope_camino_2),
                entrada, torre, dimension, configuracion );

        }else
            nivel->tope_camino_2 = 0; 
    }
    void generar_extremos_camino_2( coordenada_t* entrada, coordenada_t* torre,
        caracteristicas_nivel_t caracteristicas_nivel, bool cruzado ){

        int dimension = caracteristicas_nivel.dimension;
        switch( caracteristicas_nivel.entrada ){

                case ENTRADA_OESTE:
                    entrada->fil = rand()%dimension;
                    entrada->col = 0;
                    torre->fil = rand()%dimension;
                    torre->col = dimension-1;
                break;

                case ENTRADA_NORTE:
                    entrada->fil = 0;
                    entrada->col = rand()%(dimension/2)+(dimension/2)*(!cruzado);
                    torre->fil = dimension-1;
                    torre->col = rand()%(dimension/2)+(dimension/2);

                break;

                case ENTRADA_SUR:
                    entrada->fil = dimension-1;
                    entrada->col = rand()%(dimension/2)+(dimension/2)*(!cruzado);
                    torre->fil = 0;
                    torre->col = rand()%(dimension/2)+(dimension/2);
                break;
            }
    }

    void agregar_defensores( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  ){

        int k;
        
        // ENANOS
        k=0;
        while( k<caracteristicas_nivel.enanos )
            pedir_un_defensor( juego, caracteristicas_nivel, ENANO, &k ) ;
    

        // ELFOS
        k=0;
        while( k<caracteristicas_nivel.elfos)
            pedir_un_defensor( juego, caracteristicas_nivel, ELFO, &k ) ;
    }

    void pedir_un_defensor( juego_t* juego, 
        caracteristicas_nivel_t caracteristicas_nivel, char tipo, int* k  ){
    
        int col, fil, i=*k;
        char mensaje [MAX_MENSAJE], nombre[MAX_MENSAJE];
        coordenada_t posicion;
        char estado;
    
        strcpy(nombre,"defensor");
        if( tipo == ENANO )
            strcpy(nombre,"enano");
        if( tipo == ELFO )
            strcpy(nombre,"elfo");
    
        do{
            mostrar_juego(*juego);
            printf("\n COLOCAR DEFENSORES \n");
            printf("\n Enanos: %i \t Elfos: %i \n",caracteristicas_nivel.enanos,caracteristicas_nivel.elfos);

            sprintf(mensaje, "Ingrese la fila del %s %i",nombre,i+1);
            pedir_int( &fil, 0, caracteristicas_nivel.dimension-1,mensaje);
            sprintf(mensaje, "Ingrese la columna del %s %i",nombre,i+1);
            pedir_int( &col, 0, caracteristicas_nivel.dimension-1,mensaje);

            posicion.fil = fil;
            posicion.col = col;

        } while ( agregar_defensor( &(juego->nivel), posicion, tipo) == INVALIDO );
        i++;
        mostrar_juego(*juego);
        
        printf(" \n CANCELAR > [%c] ",CANCELAR);
        scanf("%c%c",&estado,&estado);
        if(toupper(estado) == toupper(CANCELAR)){
            i--;
            juego->nivel.tope_defensores --;
        }
        *k=i;
    }

    void auto_agregar_defensores( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  ){

        int i;
        coordenada_t posicion, aux;
        
        // ENANOS
        i=0;
        if( juego->nivel.tope_camino_1 > 0 ){
            while( i<caracteristicas_nivel.enanos ){
                do{
                    aux = juego->nivel.camino_1[ rand()%(juego->nivel.tope_camino_1-2)+2 ];
                    posicion.fil = aux.fil-1+rand()%3;
                    posicion.col = aux.col-1+rand()%3;
                } while ( agregar_defensor( &(juego->nivel), posicion, ENANO) == INVALIDO );
                i++;
            }
        }else{
            while( i<caracteristicas_nivel.enanos ){
                do{
                    posicion.fil = rand()% caracteristicas_nivel.dimension ;
                    posicion.col = rand()% caracteristicas_nivel.dimension ;
                } while ( agregar_defensor( &(juego->nivel), posicion, ENANO) == INVALIDO );
                i++;
            }
        }

        // ELFOS
        i=0;
        if( juego->nivel.tope_camino_2 > 0 ){
            while( i<caracteristicas_nivel.elfos ){
                do{
                    aux = juego->nivel.camino_2[ rand()%(juego->nivel.tope_camino_2-2) ];
                    posicion.fil = aux.fil-1+rand()%3;
                    posicion.col = aux.col-1+rand()%3;
                } while ( agregar_defensor( &(juego->nivel), posicion, ELFO) == INVALIDO );
                i++;
                mostrar_juego(*juego);
            }  
        }else{
            while( i<caracteristicas_nivel.elfos ){
                do{
                    posicion.fil = rand()% caracteristicas_nivel.dimension ;
                    posicion.col = rand()% caracteristicas_nivel.dimension ;
                } while ( agregar_defensor( &(juego->nivel), posicion, ELFO) == INVALIDO );
                i++;
            }
        } 
    }

    void agregar_defensores_bonus( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  ){

        char tipo;

        obtener_tipo_defensor_bonus( *juego, &tipo, caracteristicas_nivel );

        if( tipo != ELFO && tipo!=ENANO )
            return;

        colocar_defensores_bonus( juego, tipo, caracteristicas_nivel );

        cobrar_defensores_bonus( juego, tipo, caracteristicas_nivel );
    }

    void obtener_tipo_defensor_bonus( juego_t juego, char* tipo,
        caracteristicas_nivel_t caracteristicas_nivel ){

        bool enanos,elfos;
        char opciones [MAX_OPCIONES];
        char nombre_opciones [MAX_OPCIONES][MAX_MENSAJE];
        int tope;
        char mensaje [MAX_MENSAJE];

        enanos = ( caracteristicas_nivel.torre_1 && juego.torres.enanos_extra > 0);
        elfos = ( caracteristicas_nivel.torre_2 && juego.torres.elfos_extra > 0);

        *tipo = NADIE;
        if( !enanos && !elfos ) return;

        mostrar_juego(juego);
        printf("\n PUEDE COLOCAR UN DEFENSOR EXTRA \n");
        printf(" COSTO: ");
        if(enanos) printf(" Enanos: %i hp T1, %i hp T2 .",
            juego.torres.costo_enanos_extra[0],juego.torres.costo_enanos_extra[1]);
        if(elfos) printf(" Elfo: %i hp T1, %i hp T2 .",
            juego.torres.costo_elfos_extra[0],juego.torres.costo_elfos_extra[1]);
        printf("\n\n");

        tope=0;
        opciones[tope] = CANCELAR;
        strcpy(nombre_opciones[tope], "CANCELAR");
        tope++;
        if( enanos ){
            opciones[tope] = ENANO;
            strcpy(nombre_opciones[tope],"ENANO");
            tope++;
        }
        if( elfos ){
            opciones[tope] = ELFO;
            strcpy(nombre_opciones[tope],"ELFO");
            tope++;
        }

        sprintf(mensaje," Que defensor desea colocar ?");
        pedir_char( tipo, opciones, nombre_opciones, tope, mensaje );
    }

    void colocar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel ){

        int col, fil;
        char estado;
        char mensaje [MAX_MENSAJE];
        coordenada_t posicion;

        do{
            do{
                mostrar_juego(*juego);
                printf("\n COLOCAR DEFENSOR \n");
                
                sprintf(mensaje, "Ingrese la fila del defensor ");
                pedir_int( &fil, 0, caracteristicas_nivel.dimension-1,mensaje);
                sprintf(mensaje, "Ingrese la columna del defensor");
                pedir_int( &col, 0, caracteristicas_nivel.dimension-1,mensaje);

                posicion.fil = fil;
                posicion.col = col;

            } while ( agregar_defensor( &(juego->nivel), posicion, tipo) == INVALIDO );
            mostrar_juego(*juego);
        
            printf(" \n CANCELAR > [%c] ",CANCELAR);
            scanf("%c%c",&estado,&estado);
            if(toupper(estado) == toupper(CANCELAR)){
                juego->nivel.tope_defensores --;
            }
        }while(toupper(estado) == toupper(CANCELAR));
    }

    void auto_agregar_defensores_bonus( juego_t* juego, caracteristicas_nivel_t caracteristicas_nivel  ){

        char tipo;
        
        auto_obtener_tipo_defensor_bonus( juego, &tipo, caracteristicas_nivel );
        
        auto_colocar_defensores_bonus( juego, tipo, caracteristicas_nivel );

        cobrar_defensores_bonus( juego, tipo, caracteristicas_nivel );
    }

    void auto_obtener_tipo_defensor_bonus( juego_t* juego, char* tipo,
        caracteristicas_nivel_t caracteristicas_nivel ){

        bool enanos,elfos;

        enanos = ( caracteristicas_nivel.torre_1 && 
            juego->torres.enanos_extra > 0);

        elfos = ( caracteristicas_nivel.torre_2 && 
            juego->torres.elfos_extra > 0);


        if( enanos && elfos ){
            if( rand()%2 ){
                *tipo = ENANO;
                juego->torres.enanos_extra --;
            }else{
                *tipo = ELFO;
                juego->torres.elfos_extra --;
            }
        }
        else if( enanos ){
            *tipo = ENANO;
            juego->torres.enanos_extra --;
        }
        else if( elfos ){
            *tipo = ELFO;
            juego->torres.elfos_extra --;
        }
        else
            *tipo = NADIE;
    }
    void auto_colocar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel ){

        coordenada_t posicion,aux;
        
        if( tipo == ENANO ){
            do{
                aux = juego->nivel.camino_1[ rand()%juego->nivel.tope_camino_1 ];
                posicion.fil = aux.fil-1+rand()%3;
                posicion.col = aux.col-1+rand()%3;

            } while ( agregar_defensor( &(juego->nivel), posicion, tipo) == INVALIDO );
        }

        if( tipo == ELFO ){
            do{
                aux = juego->nivel.camino_2[ rand()%juego->nivel.tope_camino_2 ];
                posicion.fil = aux.fil-1+rand()%3;
                posicion.col = aux.col-1+rand()%3;

            } while ( agregar_defensor( &(juego->nivel), posicion, tipo) == INVALIDO );
        }
    }

    void cobrar_defensores_bonus( juego_t* juego, char tipo,
        caracteristicas_nivel_t caracteristicas_nivel ){

        int dano[2]={0,0}, i;

        if( tipo == ENANO )
            for( i=0; i<CANTIDAD_TORRES; i++)
                dano[i] += caracteristicas_nivel.costo_enanos_extra[i];
        
        if( tipo == ELFO )
            for( i=0; i<CANTIDAD_TORRES; i++)
                dano[i] += caracteristicas_nivel.costo_elfos_extra[i];

        juego->torres.resistencia_torre_1 -= dano[0];
        juego->torres.resistencia_torre_2 -= dano[1];
    }

    caracteristicas_nivel_t buscar_caracteristicas_nivel( int nivel ){

        caracteristicas_nivel_t caracteristicas_nivel;
        caracteristicas_nivel.num = 0;
        caracteristicas_nivel.orcos = 0;
        caracteristicas_nivel.dimension = 0;
        caracteristicas_nivel.torre_1 = caracteristicas_nivel.torre_2 = false;

        caracteristicas_nivel_t niveles[CANTIDAD_NIVELES] = { NIVEL_1, NIVEL_2, NIVEL_3, NIVEL_4 };

        for( int i = 0; i < CANTIDAD_NIVELES; i++ )
            if( niveles[i].num == nivel )
                caracteristicas_nivel = niveles[i];

        return caracteristicas_nivel;
    }

    int puntaje( juego_t juego, configuracion_t configuracion ){

        return MULTIPLICADOR_PUNTAJE * juego.orcos_muertos /
        (
            1 + 

            (2*configuracion.resistencia_torres[0] - juego.torres.resistencia_torre_1)/3 +
            (2*configuracion.resistencia_torres[1] - juego.torres.resistencia_torre_2)/3 +

            (
            configuracion.enanos_inicio[0] +
            configuracion.enanos_inicio[1] +
            configuracion.enanos_inicio[2] +
            configuracion.enanos_inicio[3] +

            configuracion.elfos_inicio[0] +
            configuracion.elfos_inicio[1] +
            configuracion.elfos_inicio[2] +
            configuracion.elfos_inicio[3]
            ) *2 +

            configuracion.enanos_extra[0] - juego.torres.enanos_extra +
            configuracion.elfos_extra[0] - juego.torres.elfos_extra
        ) ;
    }
// JUEGO (¡)

// ETIQUETAS

    // Implementacion de funciones para cargar componentes de la configuracion
        void cargar_configuracion
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            cargar_config( configuracion, lectura );

            strcpy(configuracion->ranking, lectura);
        }
        void cargar_grabar
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            strcpy(configuracion->grabacion, lectura);
        }
    // Implementacion de funciones para cargar componentes de la configuracion
// ETIQUETAS
