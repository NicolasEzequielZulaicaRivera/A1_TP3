#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grabacion.h"

#include "../juego/defendiendo_torres.h"
#include "../utiles/etiquetas.h"
#include "../utiles/utiles.h"

// CONSTANTES
	
	const nombre_archivo_t RUTA_GRABACION = "grabaciones/";
	static const float VELOCIDAD_REPRODUCCION_STD = 1.0f;

// CONSTANTES

// ETIQUETAS

    // Declaracion de funciones para cargar componentes del ranking
        static void cargar_grabacion
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//1
        static void cargar_velocidad
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//2
    // Declaracion de funciones para cargar componentes del ranking


    #define CANT_ETQ_GRABACION 2
    static const etiqueta_t ETIQUETAS [CANT_ETQ_GRABACION] = {
        {
            .indice = 0,
            .etiqueta = "grabacion",
            .cargar = cargar_grabacion
        },
        {
            .indice = 1,
            .etiqueta = "velocidad",
            .cargar = cargar_velocidad
        }
    };
// ETIQUETAS

// VER REPE

	// Muestra la grabacion correspondiente
	void ver_repe ( configuracion_t configuracion );

	void grabacion( int argc , char *argv [] ){

		configuracion_t configuracion = CONFIGURACION_STANDAR;
		configuracion.velocidad = VELOCIDAD_REPRODUCCION_STD;
	    etiqueta_t etiqueta_de_juego;
	    char etiqueta [MAX_NOMBRE], lectura [MAX_NOMBRE];

	    for( int i = 2; i < argc; i++ ){

	        sscanf( argv[i], "%[^=]=%s", etiqueta, lectura );

	        etiqueta_de_juego = buscar_etiqueta( etiqueta,
	                ETIQUETAS, CANT_ETQ_CONFIG );

	        etiqueta_de_juego.cargar(&configuracion,lectura);

	    }

	    ver_repe( configuracion );

	    return;
	}

	void ver_repe ( configuracion_t configuracion ){

		if( !strcmp(configuracion.grabacion,"") ){
			printf("\nNo se introdujo el archivo de grabacion\n");
			return;
		}

		nombre_archivo_t ruta;
		strcpy(ruta,configuracion.grabacion);
		cargar_ruta_grabacion(ruta);

		FILE* archivo = fopen( ruta, "rb" );

		if( !archivo ){
			printf("\nNo existe el archivo de grabacion %s\n",
				configuracion.grabacion);
			return;
		}

		juego_t juego;

		while( !feof(archivo) ){

			fread( &juego, sizeof(juego_t), 1,archivo );
			mostrar_juego( juego );
			detener_el_tiempo( configuracion.velocidad );
		}

		fclose(archivo);
	}
// VER REPE

// ETIQUETAS

    // Implementacion de funciones para cargar componentes del ranking
        void cargar_grabacion
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            strcpy(configuracion->grabacion, lectura);
        }
        void cargar_velocidad
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            configuracion->velocidad = (float) atof(lectura);
        }
    // Implementacion de funciones para cargar componentes del ranking
// ETIQUETAS

// CONSTANTES

	void cargar_ruta_grabacion( nombre_archivo_t nombre_archivo ){

		nombre_archivo_t aux;
		strcpy(aux,nombre_archivo);

		strcpy( nombre_archivo, RUTA_GRABACION );
		strcat( nombre_archivo, aux);
	}
// CONSTANTES