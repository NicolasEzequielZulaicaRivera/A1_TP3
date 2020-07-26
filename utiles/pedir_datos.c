#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "pedir_datos.h"

static const char CONFIRMAR ='S';
static const char CANCELAR ='X';

// PEDIR DATOS (!)
    
    void pedir_int( int* dato, int min, int max, char mensaje[MAX_MENSAJE] ){

        printf("%s\n",mensaje );
        printf("[%i - %i] > ",min,max );

        do{
            scanf(" %i",dato);
        } while( (*dato < min) || (*dato > max) );

        return;    
    }

    void pedir_float( float* dato, float min, float max, char mensaje[MAX_MENSAJE] ){
        
        printf("%s\n",mensaje );
        printf("[%f - %f] > ",min,max );

        do{
            scanf(" %f",dato);
        } while( (*dato < min) || (*dato > max) );

        return;
    }

    void pedir_char( char* dato, char opciones [MAX_OPCIONES], char nombre_opciones [MAX_OPCIONES][MAX_MENSAJE], int tope, char mensaje[MAX_MENSAJE] ){

        int i;
        printf("%s\n",mensaje );
        for( i = 0 ; i < tope ; i++ )
            printf(" [%c : %s] ",opciones[i],nombre_opciones[i] );

        bool dato_valido = false;

        do{
            scanf(" %c",dato);
            *dato = (char) toupper((*dato));

            for( i = 0 ; i < tope ; i++ )
                if( *dato == toupper(opciones[i])  )
                    dato_valido = true;

        } while( !dato_valido );

        return;
    }

    bool pedir_bool( char mensaje[MAX_MENSAJE] ){

        char opciones [MAX_OPCIONES]; 
        char nombre_opciones [MAX_OPCIONES][MAX_MENSAJE];
        int tope = 2;
        opciones[0]= CONFIRMAR;
        opciones[1]= CANCELAR;
        strcpy(nombre_opciones[0],OPCION_SI);
        strcpy(nombre_opciones[1],OPCION_NO);

        char respuesta;
        pedir_char(&respuesta,opciones,nombre_opciones,tope,mensaje);

        if( respuesta == CONFIRMAR )
            return true;

        return false;
    }

    void tocar_para_continuar(){

        printf("\n\t presione para continuar \n" );
        char c;
        fflush(stdout);
        scanf("%c%c",&c,&c);
    }
// PEDIR DATOS (¡)
