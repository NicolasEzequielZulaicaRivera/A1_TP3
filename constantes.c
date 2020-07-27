#include "constantes.h"
	

    const nombre_archivo_t RUTA_CONFIGURACIONES = "configuraciones/";
    const nombre_archivo_t RUTA_CAMINOS = "caminos/";

	/*
		// RESISTENCIA al comenzar el juego de las torres 1 y 2
        int resistencia_torres [2];

        // CANTIDAD de enanos y elfos al principio de cada nivel
        int enanos_inicio [CANTIDAD_NIVELES];
        int elfos_inicio [CANTIDAD_NIVELES];

        // CANTIDAD | COSTO T1 | COSTO T2 : de enanos y elfos extra
        int elfos_extra [3];
        int enanos_extra [3];

        // FALLO | CRITICO : de enanos y elfos
        int enanos_animo [2];
        int elfos_animo [2];

        // tiempo entre turnos 
        float velocidad;

        // archivo que guarda los caminos del juego
        nombre_archivo_t caminos;
    */

	const configuracion_t CONFIGURACION_STANDAR ={

        .resistencia_torres = {5000,5000},
        .enanos_inicio = {5,0,3,4},
		.elfos_inicio = {0,5,3,4},
		.elfos_extra = {10,50,0},
		.enanos_extra = {10,0,50},
		.enanos_animo = {50,50},
		.elfos_animo = {50,50},
        .velocidad = 1,
        .caminos = "_caminos.txt",


        .bonus_resistencia = 0,
        .saltear_niveles = false, .invencible = false,
        .auto_defensores = false,
        .complejidad = 2, .rareza_cruzado = 2
    };

    const configuracion_t CONFIGURACION_DEBUG ={
        .velocidad = 0.03f,
        .bonus_resistencia = 3000,
        .saltear_niveles = true, .invencible = true,
        .auto_defensores = true,
        .complejidad = 1, .rareza_cruzado = 1
    };

    const configuracion_t CONFIGURACION_MIN ={
        .velocidad = 0.01f,
        .bonus_resistencia = 0,
        .saltear_niveles = false, .invencible = false,
        .auto_defensores = false,
        .complejidad = 1, .rareza_cruzado = 1
    };

    const configuracion_t CONFIGURACION_MAX ={
        .velocidad = 1.0f,
        .bonus_resistencia = 15000,
        .saltear_niveles = true, .invencible = true,
        .auto_defensores = true,
        .complejidad = 5, .rareza_cruzado = 50
    };