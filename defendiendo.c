#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constantes.h"
#include "juego/juego.h"
#include "funcionalidades/crear_config.h"

// Constantes
	
	static const opcion_t OPCION_JUGAR = "jugar";
	static const opcion_t OPCION_JUGAR_CONFIG = "config";
	static const opcion_t OPCION_JUGAR_GRABACION = "grabacion";

	static const opcion_t OPCION_CREAR_CONFIG = "crear_configuracion";
// Constantes

// Declaraciones

	// muestra los argumentos que puede recibir el programa 
	void mostrar_opciones();
// Declaraciones

int main(int argc , char *argv []){

	if( argc < 2 ){
		mostrar_opciones();
		return 0;
	}

	if( argc == 3 && strcmp(argv[1],OPCION_CREAR_CONFIG)==0 ){
		nombre_archivo_t config;
		strcpy(config,argv[2]);
	}

	if( strcmp(argv[1],OPCION_JUGAR )==0 ){
		jugar_juego();
	}

	return 0;
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

