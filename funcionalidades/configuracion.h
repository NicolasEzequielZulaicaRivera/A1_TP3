#ifndef CONFIGURACION_H
#define CONFIGURACION_H

	#include "../constantes.h"	

	const nombre_archivo_t RUTA_CONFIGURACIONES;
    
	/*
     * Tipo de dato que almacena parametros de configuracion
     *  de un juego
     */
    typedef struct configuracion {

        // RESISTENCIA al comenzar el juego de las torres 1 y 2
        int resistencia_torres [2];

        // CANTIDAD de enanos y elfos al principio de cada nivel
        int enanos_inicio [CANTIDAD_NIVELES];
        int elfos_inicio [CANTIDAD_NIVELES];

        // CANTIDAD | COSTO T1 | COSTO T2 : de enanos y elfos extra
        int enanos_extra [3];
        int elfos_extra [3];

        // FALLO | CRITICO : de enanos y elfos
        int enanos_animo [2];
        int elfos_animo [2];

        // tiempo entre turnos 
        float velocidad;

        // archivo que guarda los caminos del juego
        nombre_archivo_t caminos;

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

	// Le permitirá al jugador crear una configuración inicial para utilizar luego.
	void crear_config( nombre_archivo_t config );

#endif