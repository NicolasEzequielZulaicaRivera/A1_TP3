#ifndef CONSTANTES_H
#define CONSTANTES_H

	#include <stdbool.h>

	#define MAX_NOMBRE 20
	#define MAX_ARCHIVO 40
	#define CANTIDAD_NIVELES  4
	#define CANTIDAD_TORRES  2
	#define INVALIDO -1
	#define STRING_INVALIDO "-1"
	
	const bool SOBREESCRIBIR;

	typedef char opcion_t[MAX_NOMBRE];
	typedef char nombre_archivo_t[MAX_ARCHIVO];

	//----- CONSTANTES COMUNES ----- (!)
		const char ANIMO_BUENO   ;
		const char ANIMO_REGULAR ;
		const char ANIMO_MALO    ;
		const char VACIO    ;
		const char FIL_PAR  ;
		const char COL_PAR  ;
		const char CAMINO   ;
		const char CAMINO_1 ;
		const char CAMINO_2 ;
		const char CAMINO_1Y2 ;
		const char TORRE    ;
		const char TORRE_1  ;
		const char TORRE_2  ;
		const char ENTRADA  ;
		const char ORCO     ;
		const char ORCO_SANO;
		const char ORCO_DEBIL;
		const char ORCO_MORIBUNDO;
		const char ELFO  ;
		const char ENANO ;
		//const char SIN_TIPO = 'X';
	//----- CONSTANTES COMUNES ----- (¡)

#endif