#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "constantes.h"
#include "funcionalidades/configuracion.h"
#include "funcionalidades/caminos.h"
#include "juego/juego.h"

// Constantes
	
	static const opcion_t OPCION_JUGAR = "jugar";
	static const opcion_t OPCION_JUGAR_CONFIG = "config";
	static const opcion_t OPCION_JUGAR_GRABACION = "grabacion";

	static const opcion_t OPCION_CREAR_CONFIG = "crear_configuracion";

	static const opcion_t OPCION_CREAR_CAMINOS = "crear_camino";
// Constantes

// Declaraciones

	// muestra los argumentos que puede recibir el programa 
	void mostrar_opciones();
// Declaraciones

int main(int argc , char *argv []){

	srand( (unsigned int) time(NULL));

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

	if( argc > 1 && strcmp(argv[1],"p" )==0 ){
		//prueba();
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


		printf("\n\n");
	}
// Implementaciones

