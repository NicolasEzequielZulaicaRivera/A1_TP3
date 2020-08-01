#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include  "../constantes.h"
#include  "caminos.h"
#include  "../utiles/pedir_datos.h"
#include  "../juego/defendiendo_torres.h"

// CONSTANTES

	static const char FORMATO [MAX_NOMBRE] = "%i;%i\n";

	const nombre_archivo_t RUTA_CAMINOS = "caminos/";

	const coordenada_t COORDENADA_INVALIDA = {.fil=INVALIDO, .col=INVALIDO};

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

	enum movimiento { ARRIBA ='W', IZQUIERDA ='A' , ABAJO = 'S', DERECHA ='D' };
// CONSTANTES

// CREAR CAMINOS

	// Subrutina para pedir al usuario los diferentes caminos
	static void pedir_caminos(caminos_t* caminos);

	// Carga los extremos de los caminos segun la constante EXTREMOS_CAMINOS
	static void cargar_extremos_caminos(caminos_t* caminos);
	
	// Subrutina para pedir al usuario los caminos de un nivel
	static void pedir_caminos_nivel(camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES]);

	// Subrutina para pedir al usuario un camino al recibir un camino con extremos (tope >= 2)
	// Mostrara por pantalla el camino mientras se introduce
	static void pedir_camino(camino_t camino, int* tope, int numero, 
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension);

	// Subrutina de pedir camino
	static void iniciar_mapa_camino(camino_t camino, int tope, int numero, 
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension, char* senda);

	// Subrutina para pedir al usuario un movimiento
	static void pedir_movimiento(coordenada_t* pos);

	// Añade una posicion al final de un camino
	// No comprubea la adyacencia
	static void agregar_posicion(camino_t camino, int* tope, coordenada_t pos);

	// Subrutina que guarda caminos en cierta ruta
	static void guardar_caminos(caminos_t* caminos, nombre_archivo_t ruta );

	// Subrutina que añade las coordenadas de un camino en cierto archivo abierto para escritura
	static void guardar_camino(camino_t camino, int tope, FILE* archivo );

	//Recibe un camino por torre con entrada y torre, y devuelve la dimension de su mapa 
	static int dimension_caminos( camino_t caminos [CANTIDAD_TORRES], int topes [CANTIDAD_TORRES] );

	
	void crear_caminos( nombre_archivo_t nombre_archivo ){

		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CAMINOS);
		strcat(ruta, nombre_archivo);

		if( !SOBREESCRIBIR && access( ruta, F_OK ) != INVALIDO ){
			printf("\nYa existe el camino\n");
			return;
		}

		caminos_t caminos;

		pedir_caminos( &caminos );

		guardar_caminos( &caminos, ruta );
	}

	void pedir_caminos(caminos_t* caminos){

		int i;

		cargar_extremos_caminos( caminos );

		for(i=0; i<CANTIDAD_NIVELES; i++){

			pedir_caminos_nivel( caminos->caminos[i], caminos->topes[i] );

			printf("\n Nivel %i Terminado \n",i+1);
			tocar_para_continuar();
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

		for( i=0 ; i<CANTIDAD_TORRES ; i++){
			pedir_camino( caminos[i], &topes[i], i+1, mapa, dimension );

			if( topes[i] > 0 ){
				system("clear");
				mostrar_mapa( mapa , dimension);
				printf("\n Camino %i Terminado \n",i+1);
				tocar_para_continuar();
			}
		}
	}

	void pedir_camino(camino_t camino, int* tope, int numero,
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension){

		if( *tope < 2 ) return;

		coordenada_t entrada = camino[0];
		coordenada_t torre = camino[*tope-1];

		char senda = CAMINO;

		iniciar_mapa_camino( camino, *tope, numero, 
			mapa , dimension, &senda);
		

		coordenada_t pos_actual = entrada,
			pos_anterior = COORDENADA_INVALIDA,
			pos_aux = COORDENADA_INVALIDA;

		*tope = 0;
		agregar_posicion(camino, tope, pos_actual );

		while( !misma_coordenada(pos_actual,torre) ){

			system("clear");
			mostrar_mapa( mapa , dimension);

			pos_aux = pos_actual;
			pedir_movimiento(&pos_aux);

			if( 
				coordenada_valida( pos_aux ) &&
				pos_aux.fil < dimension && pos_aux.col < dimension &&
				!misma_coordenada( pos_aux, pos_anterior)
			){

				pos_anterior = pos_actual;
				pos_actual = pos_aux;
				mapa[ pos_actual.fil ][ pos_actual.col ] = senda;
				agregar_posicion(camino, tope, pos_actual );
			}
		}
	}

	void iniciar_mapa_camino(camino_t camino, int tope, int numero, 
		char mapa[MAX_FILAS][MAX_COLUMNAS] , int dimension, char* senda){

		mapa[ camino[0].fil ][ camino[0].col ] = ENTRADA;
		mapa[ camino[tope-1].fil ][ camino[tope-1].col ] = TORRE;
		if( numero == 1 ){
			mapa[ camino[tope-1].fil ][ camino[tope-1].col ] = TORRE_1;
			*senda = CAMINO_1;
		}
		if( numero == 2 ){
			mapa[ camino[tope-1].fil ][ camino[tope-1].col ] = TORRE_2;
			*senda = CAMINO_2;
		}
	}

	void pedir_movimiento(coordenada_t* pos){

		char movimiento;
		pedir_char( &movimiento, 
				(char[MAX_OPCIONES]){ARRIBA,IZQUIERDA,ABAJO,DERECHA}, 
				(char[MAX_OPCIONES][MAX_MENSAJE]){"ARRIBA","IZQUIERDA","ABAJO","DERECHA"}, 4, 
				"MOVIMIENTO" );

		if( movimiento == DERECHA )
			pos->col++;
		if( movimiento == IZQUIERDA )
			pos->col--;
		if( movimiento == ABAJO )
			pos->fil++;
		if( movimiento == ARRIBA )
			pos->fil--;
	}

	void agregar_posicion(camino_t camino, int* tope, coordenada_t pos){

		camino [*tope] = pos;
		(*tope)++;
	}

	void guardar_caminos(caminos_t* caminos, nombre_archivo_t ruta ){

		FILE* archivo = fopen(ruta, "w");

		if( !archivo ){
			printf("Error al guardar caminos\n");
			return;
		}

		for( int i=0; i < CANTIDAD_NIVELES; i++ ){
			fprintf(archivo,"NIVEL=%i\n",i+1);
			for( int j=0; j < CANTIDAD_TORRES; j++ ){
				if( caminos->topes[i][j] > 2 ){
					fprintf(archivo,"CAMINO=%i\n",j+1);

					guardar_camino(
						caminos->caminos[i][j], 
						caminos->topes[i][j], 
						archivo 
					);
				}
			}
		}

		fclose(archivo);
	}

	void guardar_camino(camino_t camino, int tope, FILE* archivo ){

		for( int i=0; i<tope; i++ )
			fprintf(
				archivo,FORMATO,
				camino[i].fil,
				camino[i].col
			);

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

// LEER CAMINOS

	static void inicializar_caminos( caminos_t* caminos );

	int obtener_caminos( caminos_t* caminos, 
        nombre_archivo_t nombre_archivo ){

		nombre_archivo_t ruta;
		strcpy(ruta, RUTA_CAMINOS);
		strcat(ruta, nombre_archivo);

		FILE* archivo = fopen( ruta, "r" );

		if( !archivo ){
			printf("\n No existen los caminos %s \n", nombre_archivo);
			return INVALIDO;
		}

		inicializar_caminos( caminos );

		int nivel = INVALIDO, camino = INVALIDO;
		int fila = INVALIDO, columna = INVALIDO, tope = INVALIDO;

		int hay_nivel = false, hay_camino = false, hay_coord = false;

		while( !feof(archivo) ){

			hay_nivel = fscanf( archivo, "NIVEL=%i\n", &nivel );
			hay_camino= fscanf( archivo, "CAMINO=%i\n", &camino );
			hay_coord = fscanf( archivo, FORMATO, &fila , &columna );

			if( hay_nivel ){
				//printf( "NIVEL=%i\n", nivel );
			}
			if( hay_camino ){
				//printf( "CAMINO=%i\n", camino );
			}
			if( hay_coord ){
				//printf( FORMATO, fila , columna );
				tope = caminos->topes[nivel-1][camino-1] ++;
				caminos->caminos[nivel-1][camino-1][tope].fil = fila;
				caminos->caminos[nivel-1][camino-1][tope].col = columna;
			}
		}

		fclose( archivo );

		return 0;
	}
	
	void inicializar_caminos( caminos_t* caminos ){

		for( int i=0 ; i<CANTIDAD_NIVELES ; i++ )
			for( int j=0 ; j<CANTIDAD_TORRES ; j++ )
				caminos->topes[i][j] = 0;
	}
// LEER CAMINOS
