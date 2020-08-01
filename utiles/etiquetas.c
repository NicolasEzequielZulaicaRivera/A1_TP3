#include <string.h>
#include "etiquetas.h"

const etiqueta_t ETIQUETA_INVALIDA = {
		.indice = INVALIDO,
		.etiqueta = STRING_INVALIDO,
		.cargar = NULL
	};

etiqueta_t buscar_etiqueta( char etiqueta [MAX_NOMBRE],
	const etiqueta_t* vector_etiquetas, int cantidad_etiquetas ){

   	for( int i = 0; i < cantidad_etiquetas; i++ ){

   		if( strcmp( etiqueta, vector_etiquetas[i].etiqueta )==0 )
   			return vector_etiquetas[i];
   	}

   	return ETIQUETA_INVALIDA;
}