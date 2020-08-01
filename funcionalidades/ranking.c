#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ranking.h"
#include "../utiles/etiquetas.h"

// CONSTANTES

	const nombre_archivo_t RUTA_RANKING = "ranking/";
	const nombre_archivo_t PREFIJO_RANKING = "ranking_";

	static const char FORMATO_LECTURA [11] = "%[^;];%i\n";
	static const char FORMATO_ESRITURA [11] = "%s;%i\n";
	static const char EXTENSION [5] = "csv";

	static void cargar_ruta( nombre_archivo_t ruta, configuracion_t configuracion );
// CONSTANTES

// ETIQUETAS

    // Declaracion de funciones para cargar componentes del ranking
        static void cargar_ranking
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//1
        static void cargar_listar
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] );//2
    // Declaracion de funciones para cargar componentes del ranking


    #define CANT_ETQ_JUGAR 2
    static const etiqueta_t ETIQUETAS [CANT_ETQ_JUGAR] = {
        {
            .indice = 0,
            .etiqueta = "config",
            .cargar = cargar_ranking
        },
        {
            .indice = 1,
            .etiqueta = "listar",
            .cargar = cargar_listar
        }
    };
// ETIQUETAS

// MOSTRAR RANKING

	// Muestra el ranking correspondiente
	static void mostrar_ranking ( configuracion_t configuracion );

	void ranking( int argc , char *argv [] ){

		configuracion_t configuracion = CONFIGURACION_STANDAR;
	    etiqueta_t etiqueta_de_juego;
	    char etiqueta [MAX_NOMBRE], lectura [MAX_NOMBRE];

	    for( int i = 2; i < argc; i++ ){

	        sscanf( argv[i], "%[^=]=%s", etiqueta, lectura );

	        etiqueta_de_juego = buscar_etiqueta( etiqueta,
	                ETIQUETAS, CANT_ETQ_CONFIG );

	        etiqueta_de_juego.cargar(&configuracion,lectura);

	    }

	    mostrar_ranking( configuracion );

	    return;
	}

	void mostrar_ranking ( configuracion_t configuracion ){

		nombre_archivo_t ruta;
		cargar_ruta( ruta, configuracion );

		FILE* archivo = fopen( ruta, "r" );

		if( !archivo ){
			printf("\nNo existe el ranking\n");
			return;
		}

		int listados = 0, listar = configuracion.listar;
		int puntaje;
		nombre_t nombre;

		while( 
			( listar == INVALIDO || listados < listar) &&
			fscanf( archivo, FORMATO_LECTURA, nombre, &puntaje ) != EOF
		){

			listados ++;
			printf("%i - %s \t %i puntos\n",listados, nombre, puntaje );
		}

		printf( "\n" );

		fclose(archivo);
	}
// MOSTRAR RANKING

// GUARDAR PUNTAJE

	// Crea un ranking cuando no existe
	static void crear_ranking( int puntaje, nombre_archivo_t ruta );

	// Anade un puntaje a un ranking que existe
	static void anadir_a_ranking( int puntaje, nombre_archivo_t ruta );

	void guardar_puntaje( int puntaje, configuracion_t configuracion ){

		nombre_archivo_t ruta;
		cargar_ruta( ruta, configuracion );

		if( access( ruta, F_OK ) == INVALIDO ){
			crear_ranking( puntaje, ruta );
			return;
		}
		
		anadir_a_ranking( puntaje, ruta );
	}

	void crear_ranking( int puntaje, nombre_archivo_t ruta ){

		nombre_t nombre;
		printf("\n\nIntroduzca su nombre:" );
		scanf("%s",nombre);

		FILE* archivo = fopen( ruta, "w" );

			fprintf( archivo, FORMATO_ESRITURA, nombre, puntaje );

		fclose(archivo);
	}

	void anadir_a_ranking( int puntaje, nombre_archivo_t ruta ){

		nombre_t nombre;
		printf("\n\nIntroduzca su nombre:" );
		scanf("%s",nombre);
		bool anadido = false;

		nombre_archivo_t puntajes_viejos;
		strcpy( puntajes_viejos, RUTA_RANKING );
		strcat( puntajes_viejos, PREFIJO_RANKING );
		strcat( puntajes_viejos, ARCHIVO_TEMPORAL );

		rename( ruta, puntajes_viejos );

		FILE* puntajes = fopen( puntajes_viejos, "r" );
		if( !puntajes ){
			printf("\nNo se ha podido guardar el puntaje\n");
		}
		FILE* archivo = fopen( ruta, "w" );
		nombre_t nombre_i;
		int puntaje_i;

		while( fscanf( puntajes, FORMATO_LECTURA, nombre_i, &puntaje_i ) != EOF ){
			
			if( 
				!anadido && 
				( 
					puntaje > puntaje_i || 
					( puntaje == puntaje_i && strcmp(nombre,nombre_i) < 0 )
				) 
			){
				fprintf( archivo, FORMATO_ESRITURA, nombre, puntaje );
				anadido = true;
			}
			
			fprintf( archivo, FORMATO_ESRITURA, nombre_i, puntaje_i );
		}
		if( !anadido )
			fprintf( archivo, FORMATO_ESRITURA, nombre, puntaje );

		fclose(puntajes);
		fclose(archivo);

		remove( puntajes_viejos );
	}
// GUARDAR PUNTAJE

// ETIQUETAS

    // Implementacion de funciones para cargar componentes del ranking
        void cargar_ranking
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            strcpy(configuracion->ranking, lectura);
        }
        void cargar_listar
            ( configuracion_t* configuracion, char lectura [MAX_NOMBRE] ){

            configuracion->listar = atoi(lectura);
        }
    // Implementacion de funciones para cargar componentes del ranking
// ETIQUETAS

// CONSTANTES

    static void cargar_ruta( nombre_archivo_t ruta, configuracion_t configuracion ){

    	strcpy( ruta, RUTA_RANKING );
		strcat( ruta, PREFIJO_RANKING );
		strcat( ruta, configuracion.ranking );

		strcat( ruta, "." );
		char* ext;
		ext = strchr(ruta,'.');
		strcpy( (++ext),EXTENSION);
	}
// CONSTANTES