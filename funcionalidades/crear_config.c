#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  "../constantes.h"
#include  "../utiles/pedir_datos.h"

static const char STRING_INVALIDO [3]= "-1";

static const int MIN_INT = -1;
static const int MAX_INT = 1000000000;


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
void crear_config( nombre_archivo_t config ){

	configuracion_t configuracion = CONFIGURACION_STANDAR;
	pedir_config( &configuracion);


	nombre_archivo_t ruta;
	strcpy(ruta, RUTA_CONFIGURACIONES);
	strcat(ruta, config);

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
	if( aux > MIN_INT )
		configuracion->resistencia_torres[0] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Resistencia Torre 2" );
	if( aux > MIN_INT )
		configuracion->resistencia_torres[1] = aux;
}
void pedir_defensores_inicio(configuracion_t* configuracion){

	int aux, i;
	char mensaje[MAX_MENSAJE];

	printf("\n-- Defensores Inicio --\n");

	for( i=0; i<CANTIDAD_NIVELES;i++ ){

		sprintf(mensaje,"- Enanos Nivel %i", i+1);
		pedir_int( &aux, MIN_INT, MAX_INT, mensaje );
		if( aux > MIN_INT )
			configuracion->enanos_inicio[i] = aux;
	}

	for( i=0; i<CANTIDAD_NIVELES;i++ ){

		sprintf(mensaje,"- Elfos Nivel %i", i+1);
		pedir_int( &aux, MIN_INT, MAX_INT, mensaje );
		if( aux > MIN_INT )
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
	if( aux > MIN_INT )
		configuracion->enanos_extra[0] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Costo Torre 1" );
	if( aux > MIN_INT )
		configuracion->enanos_extra[1] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Costo Torre 1" );
	if( aux > MIN_INT )
		configuracion->enanos_extra[2] = aux;

	//ELFOS

	printf("\n- Elfos -\n");

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Cantidad" );
	if( aux > MIN_INT )
		configuracion->elfos_extra[0] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Costo Torre 1" );
	if( aux > MIN_INT )
		configuracion->elfos_extra[1] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Costo Torre 1" );
	if( aux > MIN_INT )
		configuracion->elfos_extra[2] = aux;
}
void pedir_defensores_animos(configuracion_t* configuracion){

	int aux;
	printf("\n-- Defensores Animos --\n");

	//ENANOS

	printf("\n- Enanos -\n");

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Probabilidad de Fallo" );
	if( aux > MIN_INT )
		configuracion->enanos_animo[0] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Probabilidad de Critico" );
	if( aux > MIN_INT )
		configuracion->enanos_animo[1] = aux;

	//ELFOS

	printf("\n- Elfos -\n");

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Probabilidad de Fallo" );
	if( aux > MIN_INT )
		configuracion->elfos_animo[0] = aux;

	pedir_int( &aux, MIN_INT, MAX_INT, 
		"- Probabilidad de Critico" );
	if( aux > MIN_INT )
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

	printf("- Caminos \n> ");
	scanf("%s",archivo);
	if( strcmp(archivo,STRING_INVALIDO)!=0 )
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