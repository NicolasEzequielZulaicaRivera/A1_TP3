#ifndef RANKING_H
#define RANKING_H

	#include "../constantes.h"
	#include "configuracion.h"

	const nombre_archivo_t RUTA_RANKING;
	const nombre_archivo_t PREFIJO_RANKING;

	// Muestra el ranking correspondiente
	void ranking( int argc , char *argv [] );

	// Guarda el puntaje tras una partida
	void guardar_puntaje( int puntaje, configuracion_t configuracion );

#endif