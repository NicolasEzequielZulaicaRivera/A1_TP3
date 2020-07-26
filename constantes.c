#include "constantes.h"

	const configuracion_t CONFIGURACION_STANDAR ={
        .velocidad = 0.4f,
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