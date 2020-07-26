#ifndef PEDIR_DATOS_H
#define PEDIR_DATOS_H

    #define MAX_MENSAJE 100
    #define MAX_OPCIONES 10

    static const char OPCION_SI[MAX_MENSAJE]="SI";
    static const char OPCION_NO[MAX_MENSAJE]="NO";

    // Funciones para pedir datos de distintos tipos
    void pedir_int( int* dato, int min, int max, char mensaje[MAX_MENSAJE] );
    void pedir_float( float* dato, float min, float max, char mensaje[MAX_MENSAJE] );
    void pedir_char( char* dato, char opciones [MAX_OPCIONES], char nombre_opciones [MAX_OPCIONES][MAX_MENSAJE], int tope, char mensaje[MAX_MENSAJE] );
    bool pedir_bool( char mensaje[MAX_MENSAJE] );
    
    void tocar_para_continuar();

#endif