#ifndef CAMINOS_H
#define CAMINOS_H

	#include "../constantes.h"	
	#include "../juego/defendiendo_torres.h"

	const nombre_archivo_t RUTA_CAMINOS;	

	typedef coordenada_t camino_t [MAX_LONGITUD_CAMINO];

	typedef struct caminos {

    	camino_t caminos[CANTIDAD_NIVELES][CANTIDAD_TORRES];

    	int topes[CANTIDAD_NIVELES][CANTIDAD_TORRES];

    } caminos_t;

	// Le permitirá al jugador crear los caminos de un juego para utilizar luego.
	void crear_caminos( nombre_archivo_t nombre_archivo );

#endif