#ifndef __ANIMOS_H__
#define __ANIMOS_H__

	// TP1: hace preguntas para determinar el estado ambiental y animos
	void animos(int* viento , int* humedad , char* animo_legolas , char* animo_gimli);

	// Calculo de probabilidades en funcion de el estado ambiental y animos
	int prob_critico( char animo );
	int prob_fallo( int parametro_ambiental );

#endif // __ANIMOS_H__