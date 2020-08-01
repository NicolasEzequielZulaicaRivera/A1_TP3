#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include "constantes.h"
#include "funcionalidades/configuracion.h"
#include "funcionalidades/grabacion.h"
#include "funcionalidades/caminos.h"
#include "funcionalidades/ranking.h"
#include "juego/juego.h"

// Constantes
	
	static const opcion_t OPCION_JUGAR = "jugar";
	static const opcion_t OPCION_JUGAR_CONFIG = "config";
	static const opcion_t OPCION_JUGAR_GRABACION = "grabacion";

	static const opcion_t OPCION_CREAR_CONFIG = "crear_configuracion";

	static const opcion_t OPCION_CREAR_CAMINOS = "crear_camino";

	static const opcion_t OPCION_RANKING = "ranking";
	static const opcion_t OPCION_RANKING_CONFIG = "config";
	static const opcion_t OPCION_RANKING_LISTAR = "listar";

	static const opcion_t OPCION_GRABACION = "poneme_la_repe";
	static const opcion_t OPCION_GRABACION_ARCHIVO = "grabacion";
	static const opcion_t OPCION_GRABACION_VELOCIDAD = "velocidad";
// Constantes

// Declaraciones

	// muestra los argumentos que puede recibir el programa 
	void mostrar_opciones();

	// crea los directorios necesarios para el funcionamiento del programa 
	void iniciar_directorios();

	// funcion para debuggear
	void prueba();
// Declaraciones

int main(int argc , char *argv []){

	srand( (unsigned int) time(NULL));

	iniciar_directorios();

	if( argc == 3 && strcmp(argv[1],OPCION_CREAR_CONFIG)==0 ){
		crear_config( argv[2] );
		return 0;
	}

	if( argc == 3 && strcmp(argv[1],OPCION_CREAR_CAMINOS)==0 ){
		crear_caminos( argv[2] );
		return 0;
	}

	if( argc > 1 && strcmp(argv[1],OPCION_JUGAR )==0 ){

		jugar( argc , argv );
		return 0;
	}

	if( argc > 1 && strcmp(argv[1],OPCION_RANKING )==0 ){

		ranking( argc , argv );
		return 0;
	}

	if( argc > 1 && strcmp(argv[1],OPCION_GRABACION )==0 ){

		grabacion( argc , argv );
		return 0;
	}

	if( argc > 1 && strcmp(argv[1],"p" )==0 ){
		prueba();
		return 0;
	}

	mostrar_opciones();
	return INVALIDO;
}

// Implementaciones

	void mostrar_opciones(){

		printf("\n\n");

		printf(" PARAMETROS \n");

		printf("\n %s \n", OPCION_JUGAR);
		printf("\t %s=<config.txt> (opcional)\n", OPCION_JUGAR_CONFIG);
		printf("\t %s=<grabacion.dat> (opcional)\n", OPCION_JUGAR_GRABACION);

		printf("\n %s \n", OPCION_CREAR_CONFIG);
		printf("\t <nombre.txt> \n");

		printf("\n %s \n", OPCION_CREAR_CAMINOS);
		printf("\t <nombre.txt> \n");

		printf("\n %s \n", OPCION_RANKING);
		printf("\t %s=<config.txt> (opcional)\n", OPCION_RANKING_CONFIG);
		printf("\t %s=<int> (opcional)\n", OPCION_RANKING_LISTAR);

		printf("\n %s \n", OPCION_GRABACION);
		printf("\t %s=<grabacion.dat> \n", OPCION_GRABACION_ARCHIVO);
		printf("\t %s=<float> (opcional)\n", OPCION_GRABACION_VELOCIDAD);


		printf("\n\n");
	}

void iniciar_directorios(){

	nombre_archivo_t ruta;

	strcpy(ruta,RUTA_CONFIGURACIONES);
	if( access( ruta, F_OK ) == INVALIDO ){
		mkdir(RUTA_CONFIGURACIONES, 0700);
	}

	strcpy(ruta,RUTA_CAMINOS);
	if( access( ruta, F_OK ) == INVALIDO ){
		mkdir(RUTA_CAMINOS, 0700);
	}
	
	strcpy(ruta,RUTA_RANKING);
	if( access( ruta, F_OK ) == INVALIDO ){
		mkdir(RUTA_RANKING, 0700);
	}
	
	strcpy(ruta,RUTA_GRABACION);
	if( access( ruta, F_OK ) == INVALIDO ){
		mkdir(RUTA_GRABACION, 0700);
	}

	return;
}

void prueba(){

	printf("\nPrueba\n");

	return;
}

// Implementaciones