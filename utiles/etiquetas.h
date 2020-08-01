#ifndef ETIQUETAS_H
#define ETIQUETAS_H

	#include "../constantes.h"
	#include "../funcionalidades/configuracion.h"

	typedef struct etiqueta {

        // indice de la etiqueta
        int indice;

        // la etiqueta que se encontrara en  
        char etiqueta [MAX_NOMBRE];

        /*
         * Funcion que recibiendo la lectura posterior a la etiqueta 
         *  la cargara en la configuracion
         * - El formato de lectura sera dependiente de la funcion
         */
        void (*cargar)
        ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );
    } etiqueta_t;

    const etiqueta_t ETIQUETA_INVALIDA;

    // Devuelve una etiqueta (obj) segun su etiqueta (string)
	etiqueta_t buscar_etiqueta( char etiqueta [MAX_NOMBRE],
		const etiqueta_t* vector_etiquetas, int cantidad_etiquetas );

#endif