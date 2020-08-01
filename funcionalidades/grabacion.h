#ifndef GRABACION_H
#define GRABACION_H

	#include "../constantes.h"

	const nombre_archivo_t RUTA_GRABACION;

	// convierte el nombre de un archivo de grabacion en su ruta
	void cargar_ruta_grabacion( nombre_archivo_t nombre_archivo );

	// Muestra la grabacion correspondiente
	void grabacion( int argc , char *argv [] );

#endif