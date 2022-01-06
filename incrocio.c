#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "DBGpthread.h"
#include "printerror.h"

#define STRADE 4
#define MAX_AUTO_IN_INCROCIO 2
#define AUTO_OGNI_15_SEC 6

typedef struct {
    int indice;
    int strada;
} InfoAuto;

/* auto in attesa di entrare nell'incrocio per ogni strada */
int autoInAttesa[STRADE];

/* distributore di biglietti per ciascuna strada */
int TurnoGlobale[STRADE];

/* turno corrente */
int Turno[STRADE];

int autoInIncrocio = 0;

pthread_mutex_t mutexBiglietto;
pthread_mutex_t mutexAttraversamento;
pthread_cond_t condAttraversamento;

int precedenza( int indicestrada ) {
    int i;
    for( i=indicestrada-1; i>=0; i-- ) {
        if( autoInAttesa[i] > 0 ) {
            return 0;
        }
    }
    return 1;
}

void *automobile( void *arg ) {

    char Alabel[128];
    int mioTurno;
    int mioIndice = (( InfoAuto *)arg)->indice;
    int miaStrada = (( InfoAuto *)arg)->strada;
    free( arg );
    sprintf( Alabel, "Auto %d proveniente dalla corsia %d", mioIndice, miaStrada );

    DBGpthread_mutex_lock( &mutexBiglietto, Alabel );
    mioTurno = TurnoGlobale[miaStrada]++;
    printf( "%s: prende biglietto %d\n", Alabel, mioTurno );
    DBGpthread_mutex_unlock( &mutexBiglietto, Alabel );

    DBGpthread_mutex_lock( &mutexAttraversamento, Alabel );
    while( autoInIncrocio >= MAX_AUTO_IN_INCROCIO
        || !precedenza( miaStrada )
        || mioTurno != Turno[miaStrada] ) {
        DBGpthread_cond_wait( &condAttraversamento, &mutexAttraversamento, Alabel );
    }
    printf( "%s: ( con biglietto %d ) può attraversare\n", Alabel, mioTurno );
    /* aumento le auto nell'incrocio */
    autoInIncrocio++;
    /* rimuovo un auto dalla lista di quelle in attesa */
    autoInAttesa[miaStrada]--;
    /* Aumento il ticket della mia strada */
    Turno[miaStrada]++;

    sleep( 2 );

    printf( "%s: ( con biglietto %d ) ha attraversato\n", Alabel, mioTurno );
    autoInIncrocio--;
    DBGpthread_cond_broadcast( &condAttraversamento, Alabel );
    DBGpthread_mutex_unlock( &mutexAttraversamento, Alabel );

    pthread_exit( NULL );
}

int main( void ) {

    pthread_t th;
    int rc, i, indiceAuto = 0;
    InfoAuto *info;
    srand( time( NULL ) );

    DBGpthread_mutex_init( &mutexBiglietto, NULL, "main" );
    DBGpthread_mutex_init( &mutexAttraversamento, NULL, "main" );
    DBGpthread_cond_init( &condAttraversamento, NULL, "main" );

    for( i=0; i<STRADE; i++ ) {
        autoInAttesa[i] = 0;
        TurnoGlobale[i] = 0;
        Turno[i] = 0;
    }

    while( 1 ) {
        for( i=0; i<AUTO_OGNI_15_SEC; i++ ) {
            info = ( InfoAuto *)malloc( sizeof( InfoAuto ));
            if( info == NULL ) {
                PrintErrnoAndExit( "Creazione intero" );
            }
            info->indice = indiceAuto++;
            info->strada = rand() % 4;
            rc = pthread_create( &th, NULL, automobile, ( void *)info );
            if( rc ) {
                PrintERROR_andExit( rc, "Creazione auto" );
            }
        }
        sleep( 15 );
        printf( "Main: altre 15 auto arrivate dioca\n" );
    }
    return 0;
}
