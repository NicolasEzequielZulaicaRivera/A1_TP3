#ifndef CONSTANTES_H
#define CONSTANTES_H

	#include <stdbool.h>

	#define MAX_NOMBRE 20
	#define MAX_ARCHIVO 40

	typedef char opcion_t[MAX_NOMBRE];
	typedef char nombre_archivo_t[MAX_ARCHIVO];

	/*
     * Tipo de dato que almacena parametros de configuracion
     *  de un juego
     */
    typedef struct configuracion {

        // tiempo entre turnos 
        float velocidad;

        // regeneracion de vida por nivel
        int bonus_resistencia;

        // utilidades 
        bool saltear_niveles, invencible;

        // colocar defensores automaticamente
        bool auto_defensores; 

        // parametros para los caminos
        int complejidad, rareza_cruzado; 
    } configuracion_t;

    const configuracion_t CONFIGURACION_STANDAR;

    const configuracion_t CONFIGURACION_DEBUG;

    const configuracion_t CONFIGURACION_MIN;

    const configuracion_t CONFIGURACION_MAX;

#endif