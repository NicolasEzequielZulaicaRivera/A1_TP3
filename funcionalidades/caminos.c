#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  "../constantes.h"
#include  "caminos.h"
#include  "../utiles/pedir_datos.h"
#include  "../juego/defendiendo_torres.h"

// CONSTANTES

	const nombre_archivo_t RUTA_CAMINOS = "caminos/";

	const coordenada_t EXTREMOS_CAMINOS [CANTIDAD_NIVELES][CANTIDAD_TORRES][2] = 
	{
		{ // NIVEL 1 (Este)
			{{.fil=7, .col=14},{.fil=7, .col=0}},// ENTRADA 1, TORRE 1
			{{.fil=INVALIDO, .col=INVALIDO},{.fil=INVALIDO, .col=INVALIDO}}// ENTRADA 2, TORRE 2
		},
		{ // NIVEL 2 (Oeste)
			{{.fil=INVALIDO, .col=INVALIDO},{.fil=INVALIDO, .col=INVALIDO}},// ENTRADA 1, TORRE 1
			{{.fil=7, .col=0},{.fil=7, .col=14}}// ENTRADA 2, TORRE 2
		},
		{ // NIVEL 3 (Norte)
			{{.fil=0, .col=6 },{.fil=19, .col=6 }},// ENTRADA 1, TORRE 1
			{{.fil=0, .col=14},{.fil=19, .col=14}}// ENTRADA 2, TORRE 2
		},
		{ // NIVEL 4 (Sur)
			{{.fil=19, .col=6 },{.fil=0, .col=6 }},// ENTRADA 1, TORRE 1
			{{.fil=19, .col=14},{.fil=0, .col=14}}// ENTRADA 2, TORRE 2
		}
	};

// CONSTANTES

// CREAR CAMINOS

	// Subrutina para pedir al usuario los diferentes caminos
	void pedir_caminos(caminos_t* caminos);

	// Carga los extremos de los caminos segun la constante EXTREMOS_CAMINOS
	void cargar_extremos_caminos(caminos_t* caminos);
	
	// Subrutina para pedir al usuario los caminos de un nivel
	void pedir_caminos_nivel(camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES]);

	// Subrutina para pedir al usuario un camino
	// Mostrara por pantalla el camino mientras se introduce
	void pedir_camino(camino_t camino, int* tope, 
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension);

	// Subrutina que guarda caminos en cierta ruta
	void guardar_caminos(caminos_t caminos, nombre_archivo_t ruta );

	// Subrutina que añade las coordenadas de un camino en cierto archivo abierto para escritura
	void guardar_camino(caminos_t caminos, FILE* archivo );

	//Recibe un camino por torre con entrada y torre, y devuelve la dimension de su mapa 
	int dimension_caminos( camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES] );

	
	void crear_caminos( nombre_archivo_t nombre_archivo ){

		caminos_t caminos;

		pedir_caminos( &caminos );

		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CAMINOS);
		strcat(ruta, nombre_archivo);

		guardar_caminos( caminos, ruta );

	}

	void pedir_caminos(caminos_t* caminos){

		int i;

		cargar_extremos_caminos( caminos );

		for(i=0; i<CANTIDAD_NIVELES; i++){

			pedir_caminos_nivel( caminos->caminos[i], caminos->topes[i] );
		}

	}

	void cargar_extremos_caminos(caminos_t* caminos){

		for( int i = 0 ; i < CANTIDAD_NIVELES ; i++ ){

			for( int j = 0; j < CANTIDAD_TORRES; j++ ){

				if( 
					coordenada_valida( EXTREMOS_CAMINOS[i][j][0] ) &&
					coordenada_valida( EXTREMOS_CAMINOS[i][j][1] )
				)
					(caminos->topes[i][j]) = 2;
				else
					(caminos->topes[i][j]) = 0;

				(caminos->caminos[i][j])[0] = EXTREMOS_CAMINOS[i][j][0];
				(caminos->caminos[i][j])[1] = EXTREMOS_CAMINOS[i][j][1];
			}
		}
	}

	void pedir_caminos_nivel(camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES]){

		int i,j;
		int dimension = dimension_caminos(caminos,topes);

		char mapa[MAX_FILAS][MAX_COLUMNAS];//[MAX_FILAS][MAX_COLUMNAS]

		for(i = 0; i < dimension; i++)
			for( j = 0; j < dimension; j++ )
				mapa[i][j] = VACIO;

		for( i=0 ; i<CANTIDAD_TORRES ; i++)
			pedir_camino( caminos[i], &topes[i], mapa, dimension );
		
		system("clear");
		mostrar_mapa( mapa , dimension);
		tocar_para_continuar();

	}


	void pedir_camino(camino_t camino, int* tope, 
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension){

		if( *tope > 0 ){
			mapa[ camino[0].fil ][ camino[0].col ] = ENTRADA;
			mapa[ camino[*tope-1].fil ][ camino[*tope-1].col ] = TORRE;
		}
		
		system("clear");
		mostrar_mapa( mapa , dimension);
		printf("\n %i,%i-%i,%i",camino[0].fil,camino[0].col,camino[*tope-1].fil,camino[*tope-1].col);
		printf("\n %i",*tope);
		tocar_para_continuar();
	}

	void guardar_caminos(caminos_t caminos, nombre_archivo_t ruta ){
		
	}

	void guardar_camino(caminos_t caminos, FILE* archivo ){

	}

	int dimension_caminos( camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES]  ){

		int i,j, max=0;

		for(i=0; i<CANTIDAD_TORRES; i++){
			for(j=0; j<topes[i]; j++){

				if( caminos[i][j].fil > max )
					max = (caminos[i])[j].fil;

				if( caminos[i][j].col > max )
					max = caminos[i][j].col;
			}
		}

		return max+1;
	}

// CREAR CAMINOS
