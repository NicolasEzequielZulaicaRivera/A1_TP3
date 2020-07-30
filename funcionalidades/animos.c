#include <stdio.h>
#include <stdbool.h>
#include "animos.h"
#include "../constantes.h"
///Constantes

    /// Dia - Viento
static const int primer_dia_del_mes = 1;
static const int ultimo_dia_del_mes = 30;

static const int dia_hasta_1 = 10;
static const int dia_hasta_2 = 20;
static const int dia_hasta_3 = 30;
static const int vel_viento_1 = 75;
static const int vel_viento_2 = 50;
static const int vel_viento_3 = 25;

    /// Hora - Humedad
static const char manana = 'M';
static const char tarde  = 'T';
static const char noche  = 'N';
static const int h_manana = 75;
static const int h_tarde  = 25;
static const int h_noche  = 50;

    /// Animo

        ///Pie
static const char pie_izq = 'I';
static const char pie_der = 'D';
static const int p_pie_der = 10;
static const int p_pie_izq =  0;

        /// Comida
static const char ensalada     = 'E';
static const char hamburguesa  = 'H';
static const char pizza        = 'P';
static const char guiso        = 'G';
static const int p_ensalada     = 20;
static const int p_hamburguesa  = 15;
static const int p_pizza        = 10;
static const int p_guiso        =  5;

        /// Dormimr
static const int dormir_max = 12;
static const int dormir_min = 0;
static const int durmio_hasta_1 = 4;
static const int durmio_hasta_2 = 8;
static const int p_durmio_hasta_1 = 0;
static const int p_durmio_hasta_2 = 10;
static const int p_durmio_hasta_3 = 20;


        ///Estados
static const char animo_bueno   = 'B';
static const char animo_regular = 'R';
static const char animo_malo    = 'M';
static const int p_hasta_bueno   = 50;
static const int p_hasta_regular = 35;
static const int p_hasta_malo    = 20;

static const int PROBABILIDAD_CRITICO_NULA =  0;
static const int PROBABILIDAD_CRITICO_MALA =  0;
static const int PROBABILIDAD_CRITICO_REGULAR = 10;
static const int PROBABILIDAD_CRITICO_BUENA = 25;


/// Declaracion de Funciones
bool c_en_arreglo(char dato,const char valores_validos[],const int n_valores_validos);
void c_pedir_dato(char* dato,const char valores_validos[],const int n_valores_validos,char mensaje[]);
bool i_en_rango(int dato,const int min,const int max);
void i_pedir_dato(int* dato,const int min,const int max,char mensaje[]);

int calcular_vel_viento(int dia);
int calcular_humedad(char hora);
char calcular_animo(char pie, char cena, int durmio);

void historia(int vel_viento, int humedad, char animo_legolas, char animo_gimli);

/// -- PROGRAMA PRINCIPAL --

void animos(int* viento , int* humedad , char* animo_legolas , char* animo_gimli){
    /// Constantes & Variables

    // no se puede declarar global
    const char horas[] ={manana,tarde,noche};
    const char pies[] = {pie_izq,pie_der};
    const char comidas[] = {ensalada,hamburguesa,pizza,guiso};

    int dia = -1;
    char hora =' ';

    *animo_legolas = 'x';
    char   pie_legolas = ' ';
    char  cena_legolas = ' ';
    int durmio_legolas = -1;

    *animo_gimli = 'x';
    char   pie_gimli = ' ';
    char  cena_gimli = ' ';
    int durmio_gimli = -1;

    *viento = 0;
    *humedad = 0;

    char mensaje[100]="";

    /// Pedir Datos

        /// Tiempo
    sprintf(mensaje, "- Cuando se librara la batalla ? Que dia del mes ?  [%i-%i] ",primer_dia_del_mes,ultimo_dia_del_mes);
    i_pedir_dato(&dia,primer_dia_del_mes,ultimo_dia_del_mes,mensaje);

    sprintf(mensaje, "- Cuando se librara la batalla ? Manana, Tarde o Noche ?  [%c/%c/%c] ",manana,tarde,noche);
    c_pedir_dato(&hora,horas,3,mensaje);

        /// Legolas
    sprintf(mensaje, "- Con que pie se levanto Legolas ? Izquierdo o Derecho ?  [%c/%c] ",pie_izq,pie_der);
    c_pedir_dato(&pie_legolas,pies,2,mensaje);

    sprintf(mensaje, "- Que ceno Legolas ? Ensalada, Hamburguesa, Pizza o Guiso ?  [%c/%c/%c/%c] ",ensalada,hamburguesa,pizza,guiso);
    c_pedir_dato(&cena_legolas,comidas,4,mensaje);

    sprintf(mensaje, "- Cuantas horas durmio Legolas ? [%i-%i] ",dormir_min,dormir_max);
    i_pedir_dato(&durmio_legolas,dormir_min,dormir_max,mensaje);

        /// Gimli
    sprintf(mensaje, "- Con que pie se levanto Gimli ? Izquierdo o Derecho ?  [%c/%c] ",pie_izq,pie_der);
    c_pedir_dato(&pie_gimli,pies,2,mensaje);

    sprintf(mensaje, "- Que ceno Gimli ? Ensalada, Hamburguesa, Pizza o Guiso ?  [%c/%c/%c/%c] ",ensalada,hamburguesa,pizza,guiso);
    c_pedir_dato(&cena_gimli,comidas,4,mensaje);

    sprintf(mensaje, "- Cuantas horas durmio Gimli ? [%i-%i] ",dormir_min,dormir_max);
    i_pedir_dato(&durmio_gimli,dormir_min,dormir_max,mensaje);

    /// Calcular Valores

    *viento    = calcular_vel_viento( dia );
    *humedad       = calcular_humedad( hora );
    *animo_legolas = calcular_animo( pie_legolas, cena_legolas, durmio_legolas );
    *animo_gimli   = calcular_animo( pie_gimli, cena_gimli, durmio_gimli );

    ///Fin
    //printf("\nResumen: %i %i %c %c.\n",*viento,*humedad,*animo_legolas,*animo_gimli);

    return;
}

int prob_critico( char animo ){

    int dano_critico = 
        (animo == ANIMO_BUENO)?   PROBABILIDAD_CRITICO_BUENA:
        (animo == ANIMO_REGULAR)? PROBABILIDAD_CRITICO_REGULAR:
        (animo == ANIMO_MALO)?    PROBABILIDAD_CRITICO_MALA:
        PROBABILIDAD_CRITICO_NULA;

    return dano_critico;
}

int prob_fallo( int parametro_ambiental ){

    return parametro_ambiental/2;
}

/// Definicion de Funciones

bool c_en_arreglo(char dato,const char valores_validos[],const int n_valores_validos){
    for(int i=0; i<n_valores_validos;i++){
        if( dato == valores_validos[i]) return true;
    }
    return false;
};
void c_pedir_dato(char* dato,const char valores_validos[],const int n_valores_validos,char mensaje[]){
    do{
        printf("%s\n",mensaje);
        scanf(" %c",dato);
    }while( !c_en_arreglo(*dato,valores_validos,n_valores_validos) );
    return;
};

bool i_en_rango(int dato,const int min,const int max){
    return ( dato >= min ) && ( dato <= max );
};
void i_pedir_dato(int* dato,const int min,const int max,char mensaje[]){
    do{
        printf("%s\n",mensaje);
        scanf(" %i",dato);
    }while( !i_en_rango(*dato,min,max) );
    return;
};

int calcular_vel_viento(int dia){

    if( dia <= dia_hasta_1 )
        return vel_viento_1;
    else if ( dia <= dia_hasta_2 )
        return vel_viento_2;
    else if ( dia <= dia_hasta_3 )
        return vel_viento_3;
    return -1;
};
int calcular_humedad(char hora){
    if( hora == manana )
        return h_manana;
    else if ( hora == tarde )
        return h_tarde;
    else if ( hora == noche )
        return h_noche;
    return -1;
};

char calcular_animo(char pie, char cena, int durmio){

    int puntos_de_animo = 0;

    if( pie == pie_der )
        puntos_de_animo += p_pie_der;
    else if( pie == pie_izq )
        puntos_de_animo += p_pie_izq;
    else
        return 'x';

    if( cena == ensalada )
        puntos_de_animo += p_ensalada;
    else if( cena == hamburguesa )
        puntos_de_animo += p_hamburguesa;
    else if( cena == pizza )
        puntos_de_animo += p_pizza;
    else if( cena == guiso )
        puntos_de_animo += p_guiso;
    else
        return 'x';

    if( durmio <= durmio_hasta_1 )
        puntos_de_animo += p_durmio_hasta_1;
    else if( durmio <= durmio_hasta_2 )
        puntos_de_animo += p_durmio_hasta_2;
    else if( durmio <= dormir_max )
        puntos_de_animo += p_durmio_hasta_3;
    else
        return 'x';

    if( puntos_de_animo <= p_hasta_malo )
        return animo_malo;
    else if( puntos_de_animo <= p_hasta_regular )
        return animo_regular;
    else if( puntos_de_animo <= p_hasta_bueno )
        return animo_bueno;
    else
        return 'x';

    return 'x';
};
