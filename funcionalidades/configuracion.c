#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  "../constantes.h"
#include  "configuracion.h"
#include  "../utiles/pedir_datos.h"

// CONSTANTES

	const nombre_archivo_t RUTA_CONFIGURACIONES = "configuraciones/";
    
	const configuracion_t CONFIGURACION_STANDAR ={

        .resistencia_torres = {5000,5000},
        .enanos_inicio = {5,0,3,4},
		.elfos_inicio = {0,5,3,4},
		.elfos_extra = {10,50,0},
		.enanos_extra = {10,0,50},
		.enanos_animo = {50,50},
		.elfos_animo = {50,50},
        .velocidad = 1,
        .caminos = "_caminos.txt",


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

	// Estrucura que describe las etiquetas que se encuentran en los archivos de configuracion
	// debido a las limitaciones de C se utilizaran funciones para relacionarlas
	typedef struct etiqueta_de_configuracion {

		// indice de la etiqueta
		int indice;

		// la etiqueta que se encontrara en  
		char etiqueta [MAX_NOMBRE];

		/*
		 * Funcion que recibiendo la lectura posterior a la etiqueta 
		 *  la cargara en la configuracion
		 */
		void (*cargar)
		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );
   	} etiqueta_de_configuracion_t;

   	// Declaracion de funciones para cargar componentes de la configuracion
	   	void cargar_resistencia_torres
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//1
	   	void enanos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//2
	   	void elfos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//3
	   	void enanos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//4
	   	void elfos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//5
	   	void enanos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//6
	   	void elfos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//7
	   	void velocidad
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//8
	   	void caminos
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//9
   	// Declaracion de funciones para cargar componentes de la configuracion

	const etiqueta_de_configuracion_t ETIQUETAS [1] = {
		{
			.indice = 1,
			.etiqueta = "RESISTENCIA_TORRES",
			.cargar = &cargar_resistencia_torres
		}
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

	// Guarda una configuracion segun datos ingresados por el usuario
	void crear_config( nombre_archivo_t nombre_archivo ){

		configuracion_t configuracion = CONFIGURACION_STANDAR;
		pedir_config( &configuracion);


		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CONFIGURACIONES);
		strcat(ruta, nombre_archivo);

		guardar_config( configuracion, ruta );

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

		fprintf(archivo,"RESISTENCIA_TORRES=%i,%i\n", 
			configuracion.resistencia_torres[0], configuracion.resistencia_torres[1] );

		fprintf(archivo,"ENANOS_INICIO=%i,%i,%i,%i\n", 
			configuracion.enanos_inicio[0], configuracion.enanos_inicio[1],
			configuracion.enanos_inicio[2], configuracion.enanos_inicio[3] );

		fprintf(archivo,"ELFOS_INICIO=%i,%i,%i,%i\n", 
			configuracion.elfos_inicio[0], configuracion.elfos_inicio[1],
			configuracion.elfos_inicio[2], configuracion.elfos_inicio[3] );

		fprintf(archivo,"ENANOS_EXTRA=%i,%i,%i\n", 
			configuracion.enanos_extra[0], configuracion.enanos_extra[1],
			configuracion.enanos_extra[2] );

		fprintf(archivo,"ELFOS_EXTRA=%i,%i,%i\n", 
			configuracion.elfos_extra[0], configuracion.elfos_extra[1],
			configuracion.elfos_extra[2] );

		fprintf(archivo,"ENANOS_ANIMO=%i,%i\n", 
			configuracion.enanos_animo[0], configuracion.enanos_animo[1]);

		fprintf(archivo,"ELFOS_ANIMO=%i,%i\n", 
			configuracion.enanos_animo[0], configuracion.enanos_animo[1]);

		fprintf(archivo,"VELOCIDAD=%f\n", 
			configuracion.velocidad );

		fprintf(archivo,"CAMINOS=%s\n", 
			configuracion.caminos );

		fclose(archivo);
	}
// CREAR CONFIG

// LEER CONFIG

	// Cargara una configuracion de un archivo
    void cargar_config( configuracion_t* configuracion, 
        nombre_archivo_t nombre_archivo );
// LEER CONFIG

// ETIQUETAS

    // Implementacion de funciones para cargar componentes de la configuracion
	   	void cargar_resistencia_torres
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%i;%i", 
	   			&(configuracion->resistencia_torres[0]),
	   			&(configuracion->resistencia_torres[1])
	   		);
	   	}
	   	void enanos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		// DEPENDE DE CANTIDAD DE NIVELES 
	   		sscanf( lectura, "%i;%i;%i;%i",
	   			&(configuracion->enanos_inicio[0]),
	   			&(configuracion->enanos_inicio[1]),
	   			&(configuracion->enanos_inicio[2]),
	   			&(configuracion->enanos_inicio[3])
	   		);
	   	}
	   	void elfos_inicio
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		// DEPENDE DE CANTIDAD DE NIVELES 
	   		sscanf( lectura, "%i;%i;%i;%i",
	   			&(configuracion->elfos_inicio[0]),
	   			&(configuracion->elfos_inicio[1]),
	   			&(configuracion->elfos_inicio[2]),
	   			&(configuracion->elfos_inicio[3])
	   		);
	   	}
	   	void enanos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%i;%i;%i",
	   			&(configuracion->enanos_extra[0]),
	   			&(configuracion->enanos_extra[1]),
	   			&(configuracion->enanos_extra[2])
	   		);
	   	}
	   	void elfos_extra
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%i;%i;%i",
	   			&(configuracion->elfos_extra[0]),
	   			&(configuracion->elfos_extra[1]),
	   			&(configuracion->elfos_extra[2])
	   		);
	   	}
	   	void enanos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%i;%i", 
	   			&(configuracion->enanos_animo[0]),
	   			&(configuracion->enanos_animo[1])
	   		);
	   	}
	   	void elfos_animo
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%i;%i", 
	   			&(configuracion->elfos_animo[0]),
	   			&(configuracion->elfos_animo[1])
	   		);
	   	}
	   	void velocidad
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		sscanf( lectura, "%f", 
	   			&(configuracion->velocidad)
	   		);
	   	}
	   	void caminos
	   		( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

	   		strcpy( configuracion->caminos, lectura);
	   	}
   	// Implementacion de funciones para cargar componentes de la configuracion
// ETIQUETAS

// PRUEBAS

void prueba(){

	etiqueta_de_configuracion_t et = ETIQUETAS [0];

	printf("%i, %s\n", et.indice, et.etiqueta );

	configuracion_t conf = CONFIGURACION_STANDAR;

	et.cargar( &conf, "2;2" );

	printf(
		"\n----\n %i;%i \n----\n",
		conf.resistencia_torres[0],conf.resistencia_torres[1]
	);

}
// PRUEBAS