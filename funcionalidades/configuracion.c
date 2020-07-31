#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include  "../constantes.h"
#include  "configuracion.h"
#include  "../utiles/pedir_datos.h"
#include  "../utiles/etiquetas.h"

// CONSTANTES

	const nombre_archivo_t RUTA_CONFIGURACIONES = "configuraciones/";

	static const char SEPARADOR = ',';
    
	const configuracion_t CONFIGURACION_STANDAR ={

        .resistencia_torres = {600,600},
        .enanos_inicio = {5,0,3,4},
		.elfos_inicio = {0,5,3,4},
		.enanos_extra = {10,50,0},
		.elfos_extra = {10,0,50},
		.enanos_animo = {10,50},
		.elfos_animo = {10,50},
        .velocidad = 0.1f,
        .caminos = "_caminos.txt",
        
        .ranking = "ranking.csv",
        .listar = INVALIDO,

        .grabacion = "",

        .caminos_random = false,
        .bonus_resistencia = 0,
        .saltear_niveles = false, .invencible = false,
        .auto_defensores = false,
        .complejidad = 2, .rareza_cruzado = 2
    };

    const configuracion_t CONFIGURACION_DEBUG ={
        .velocidad = 0.03f,
        .bonus_resistencia = 3000,
        .saltear_niveles = true, .invencible = true,
        .auto_defensores = true,
        .complejidad = 1, .rareza_cruzado = 1
    };

    const configuracion_t CONFIGURACION_MIN ={
        .velocidad = 0.01f,
        .bonus_resistencia = 0,
        .saltear_niveles = false, .invencible = false,
        .auto_defensores = false,
        .complejidad = 1, .rareza_cruzado = 1
    };

    const configuracion_t CONFIGURACION_MAX ={
        .velocidad = 1.0f,
        .bonus_resistencia = 15000,
        .saltear_niveles = true, .invencible = true,
        .auto_defensores = true,
        .complejidad = 5, .rareza_cruzado = 50
    };

	static const int MIN_INT = -1;
	static const int MAX_INT = 1000000000;
// CONSTANTES

// ETIQUETAS

   	// Declaracion de funciones para cargar componentes de la configuracion
	   	void cargar_resistencia_torres
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//1
	   	void cargar_enanos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//2
	   	void cargar_elfos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//3
	   	void cargar_enanos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//4
	   	void cargar_elfos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//5
	   	void cargar_enanos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//6
	   	void cargar_elfos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//7
	   	void cargar_velocidad
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//8
	   	void cargar_caminos
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//9

	   	// Funcion que carga enteros separados por un SEPARADOR
	   	// en un vector (que es apuntado por el puntero)
	   	// pre: la cantidad de elementos a leer es igual a los elementos en el vector
	   	void cargar_enteros_separados( int* puntero_vector, char lectura [MAX_NOMBRE] );
   	// Declaracion de funciones para cargar componentes de la configuracion

	static const etiqueta_t ETIQUETAS [CANT_ETQ_CONFIG] = {
		{
			.indice = 0,
			.etiqueta = "RESISTENCIA_TORRES",
			.cargar = cargar_resistencia_torres
		},
		{
			.indice = 1,
			.etiqueta = "ENANOS_INICIO",
			.cargar = cargar_enanos_inicio
		},
		{
			.indice = 2,
			.etiqueta = "ELFOS_INICIO",
			.cargar = cargar_elfos_inicio
		},
		{
			.indice = 3,
			.etiqueta = "ENANOS_EXTRA",
			.cargar = cargar_enanos_extra
		},
		{
			.indice = 4,
			.etiqueta = "ELFOS_EXTRA",
			.cargar = cargar_elfos_extra
		},
		{
			.indice = 5,
			.etiqueta = "ENANOS_ANIMO",
			.cargar = cargar_enanos_animo
		},
		{
			.indice = 6,
			.etiqueta = "ELFOS_ANIMO",
			.cargar = cargar_elfos_animo
		},
		{
			.indice = 7,
			.etiqueta = "VELOCIDAD",
			.cargar = cargar_velocidad
		},
		{
			.indice = 8,
			.etiqueta = "CAMINOS",
			.cargar = cargar_caminos
		},
	};
// ETIQUETAS

// CREAR CONFIG

	// Subrutinas para pedir al usuario los diferentes componentes de la configuracion
	void pedir_config(configuracion_t* configuracion);
	void pedir_torres(configuracion_t* configuracion);
	void pedir_defensores_inicio(configuracion_t* configuracion);
	void pedir_defensores_extra(configuracion_t* configuracion);
	void pedir_defensores_animos(configuracion_t* configuracion);
	void pedir_otros(configuracion_t* configuracion);

	// Subrutina que guarda una configuracion en cierta ruta
	void guardar_config( configuracion_t configuracion, nombre_archivo_t ruta );

	
	void crear_config( nombre_archivo_t nombre_archivo ){

		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CONFIGURACIONES);
		strcat(ruta, nombre_archivo);

		if( !SOBREESCRIBIR && access( ruta, F_OK ) != INVALIDO ){
			printf("\nYa existe la configuracion\n");
			return;
		}

		configuracion_t configuracion = CONFIGURACION_STANDAR;
		pedir_config( &configuracion);

		guardar_config( configuracion, ruta );

		printf("\nConfiguracion guardada correctamente\n");
	}

	void pedir_config(configuracion_t* configuracion){

		pedir_torres( configuracion);
		pedir_defensores_inicio( configuracion);
		pedir_defensores_extra( configuracion);
		pedir_defensores_animos( configuracion);
		pedir_otros( configuracion);
	}

	void pedir_torres(configuracion_t* configuracion){
		
		int aux;
		printf("\n-- Resistencia Torres --\n");

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Resistencia Torre 1" );
		configuracion->resistencia_torres[0] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Resistencia Torre 2" );
		configuracion->resistencia_torres[1] = aux;
	}
	void pedir_defensores_inicio(configuracion_t* configuracion){

		int aux, i;
		char mensaje[MAX_MENSAJE];

		printf("\n-- Defensores Inicio --\n");

		for( i=0; i<CANTIDAD_NIVELES;i++ ){

			sprintf(mensaje,"- Enanos Nivel %i", i+1);
			pedir_int( &aux, MIN_INT, MAX_INT, mensaje );
			configuracion->enanos_inicio[i] = aux;
		}

		for( i=0; i<CANTIDAD_NIVELES;i++ ){

			sprintf(mensaje,"- Elfos Nivel %i", i+1);
			pedir_int( &aux, MIN_INT, MAX_INT, mensaje );
			configuracion->elfos_inicio[i] = aux;
		}
	}
	void pedir_defensores_extra(configuracion_t* configuracion){

		int aux;
		printf("\n-- Defensores Extra --\n");

		//ENANOS

		printf("\n- Enanos -\n");

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Cantidad" );
		configuracion->enanos_extra[0] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Costo Torre 1" );
		configuracion->enanos_extra[1] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Costo Torre 1" );
		configuracion->enanos_extra[2] = aux;

		//ELFOS

		printf("\n- Elfos -\n");

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Cantidad" );
		configuracion->elfos_extra[0] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Costo Torre 1" );
		configuracion->elfos_extra[1] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Costo Torre 1" );
		configuracion->elfos_extra[2] = aux;
	}
	void pedir_defensores_animos(configuracion_t* configuracion){

		int aux;
		printf("\n-- Defensores Animos --\n");

		//ENANOS

		printf("\n- Enanos -\n");

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Probabilidad de Fallo" );
		configuracion->enanos_animo[0] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Probabilidad de Critico" );
		configuracion->enanos_animo[1] = aux;

		//ELFOS

		printf("\n- Elfos -\n");

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Probabilidad de Fallo" );
		configuracion->elfos_animo[0] = aux;

		pedir_int( &aux, MIN_INT, MAX_INT, 
			"- Probabilidad de Critico" );
		configuracion->elfos_animo[1] = aux;
	}
	void pedir_otros(configuracion_t* configuracion){

		float aux;
		nombre_archivo_t archivo;

		printf("\n-- Otros --\n");

		pedir_float( &aux, (float)MIN_INT, (float)MAX_INT, 
			"- Velocidad" );
		if( aux > 0 )
			configuracion->velocidad = aux;
		else
			configuracion->velocidad = INVALIDO;

		printf("- Caminos \n> ");
		scanf("%s",archivo);
		strcpy(configuracion->caminos,archivo);
	}

	void guardar_config( configuracion_t configuracion, nombre_archivo_t ruta ){

		FILE* archivo = fopen(ruta,"w");
		int i = 0;

		fprintf(archivo,"%s=%i,%i\n", ETIQUETAS[ i++].etiqueta,
			configuracion.resistencia_torres[0], configuracion.resistencia_torres[1] );

		fprintf(archivo,"%s=%i,%i,%i,%i\n", ETIQUETAS[ i++ ].etiqueta,
			configuracion.enanos_inicio[0], configuracion.enanos_inicio[1],
			configuracion.enanos_inicio[2], configuracion.enanos_inicio[3] );

		fprintf(archivo,"%s=%i,%i,%i,%i\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.elfos_inicio[0], configuracion.elfos_inicio[1],
			configuracion.elfos_inicio[2], configuracion.elfos_inicio[3] );

		fprintf(archivo,"%s=%i,%i,%i\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.enanos_extra[0], configuracion.enanos_extra[1],
			configuracion.enanos_extra[2] );

		fprintf(archivo,"%s=%i,%i,%i\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.elfos_extra[0], configuracion.elfos_extra[1],
			configuracion.elfos_extra[2] );

		fprintf(archivo,"%s=%i,%i\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.enanos_animo[0], configuracion.enanos_animo[1]);

		fprintf(archivo,"%s=%i,%i\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.enanos_animo[0], configuracion.enanos_animo[1]);

		fprintf(archivo,"%s=%f\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.velocidad );

		fprintf(archivo,"%s=%s\n", ETIQUETAS[ i++ ].etiqueta, 
			configuracion.caminos );

		fclose(archivo);
	}
// CREAR CONFIG

// LEER CONFIG

	// Subrutina de cargar_config
	// cargara los datos de un archivo de configuracion en una configuracion sin verificar
	// pre: ruta correcta
    void cargar_config_archivo( configuracion_t* configuracion, 
        nombre_archivo_t ruta );

    void cargar_config( configuracion_t* configuracion, 
        nombre_archivo_t nombre_archivo ){

		configuracion_t aux = *configuracion;

		*configuracion = CONFIGURACION_STANDAR;

		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CONFIGURACIONES);
		strcat(ruta, nombre_archivo);


		cargar_config_archivo( configuracion, ruta ); 

		// Correcciones 
		strcpy(configuracion->grabacion, aux.grabacion);	
		strcpy(configuracion->ranking, aux.ranking);	
		configuracion->auto_defensores = aux.auto_defensores;
		configuracion->listar = aux.listar;
    }

    void cargar_config_archivo( configuracion_t* configuracion, 
        nombre_archivo_t ruta ){

    	char etiqueta [MAX_NOMBRE], lectura [MAX_NOMBRE] ;
    	etiqueta_t etiqueta_de_configuracion;


    	FILE* archivo = fopen( ruta, "r" );

		if( !archivo ){

			printf("\nNo existe el archivo de configuracion\nSe utilizara la configuracion standar\n");
			tocar_para_continuar();
			return;
		} 

    	while( 
    		fscanf( archivo, "%[^=]=%[^\n]\n", etiqueta, lectura  ) != EOF 
    	){

	    	etiqueta_de_configuracion = buscar_etiqueta( etiqueta,
	    		ETIQUETAS, CANT_ETQ_CONFIG );

	    	etiqueta_de_configuracion.cargar(configuracion,lectura);
    	}

    	fclose( archivo );
    }
// LEER CONFIG

// ETIQUETAS

    // Implementacion de funciones para cargar componentes de la configuracion
	   	void cargar_enteros_separados( int* puntero_vector, char lectura [MAX_NOMBRE] ){

	   		int aux;
	   		char lectura_aux [MAX_NOMBRE];
	   		do{
		   		sscanf( lectura, "%i%s", &aux, lectura_aux );

		   		if( aux != INVALIDO )
		   			*puntero_vector = aux;
		   		puntero_vector++;

		   		lectura=strchr(lectura,SEPARADOR);
		   		if( lectura )lectura ++;
	   		}while(lectura);

	   	}

	   	void cargar_resistencia_torres
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->resistencia_torres, lectura );
	   	}
	   	void cargar_enanos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->enanos_inicio, lectura );
	   	}
	   	void cargar_elfos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->elfos_inicio, lectura );
	   	}
	   	void cargar_enanos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->enanos_extra, lectura );
	   	}
	   	void cargar_elfos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->elfos_extra, lectura );
	   	}
	   	void cargar_enanos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->enanos_animo, lectura );
	   	}
	   	void cargar_elfos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		cargar_enteros_separados( configuracion->elfos_animo, lectura );
	   	}
	   	void cargar_velocidad
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		float aux;

	   		sscanf( lectura, "%f", &aux);

	   		if( (int)aux != INVALIDO )
	   			configuracion->velocidad = aux;
	   	}
	   	void cargar_caminos
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		if( strcmp(lectura,STRING_INVALIDO) )
	   			strcpy( configuracion->caminos, lectura);
	   	}
   	// Implementacion de funciones para cargar componentes de la configuracion
// ETIQUETAS