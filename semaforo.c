/*
Universidad del valle de Guatemala
Sistemas Operativos
Alexis Fernando Hengstenberg chocooj
carne 17699
Semaforo
*/


//importamos librerias
#include <stdio.h>
#include <unistd.h>


#include <sys/syscall.h>

#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

//compilación de partes de un archivo fuente. 
#ifndef THREAD_N
#define THREAD_N 4
#endif
//compilación de partes de un archivo fuente. 
#ifndef ITER_N
#define ITER_N 15
#endif
//compilación de partes de un archivo fuente. 
#ifndef gettid
#define gettid() syscall(SYS_gettid)
#endif

//ingresamos el contador
int contador = 0;
sem_t semaforoT;


void * thread_runner( void * args ) {


    //archivo de registro de args
    FILE * log = ( ( FILE * )args );

    
    //inicio.
    //obtenemos el id del thread.
    int idThread = gettid();

    //mostramos el inicio.
    fprintf( log, "Inicio, ID del thread: %d \n", idThread );

    //declaramos variable para el for
    int i;

    //comiemza el ciclo.
    for( i = 0; i < ITER_N; i++ ) {

        //printeamos el id del thread que obtiene acceso a los recursos 
        fprintf( log, "utilizando los recursos el thread: %d\n", i, idThread );
        //el semaforo cambia a estado de espera ya que esta siendo utilizados los recursos
        fprintf( log, "enviar senial al semaforo para ambiar a modo espera\n");

        //instruccion para poner en espera el semaforo
        sem_wait( &semaforoT );
        //confirmamos que el semaforo efectivamente este en espera.

        fprintf( log, "semaforo en espera\n");


        //aumentamos el contador.
        contador++;
        //mostramos el numero del contador.

        //el thread ya no utilizara mas los recursos por lo tanto sale del semforo y lo livera.
        fprintf( log, "los recursos son liberados por el thread> %d\n", idThread);
        //instruccion para liberar el semaforo.
        sem_post( &semaforoT );
        //mostramos que salga sin ningun problema

        fprintf( log, "salida correcta del thread\n");
        sleep( 1 );
    }

    fprintf( log, "Salida del thread\n");
    //salida del thread
    pthread_exit( NULL );

}

int main() {
    //abrimos el archivo en el que mostraremos lo que sucede con el programa.
    //escribiremos todo dentro del archivo.
    FILE * log_semaphore = fopen( "semaforoTest.txt", "w" );

    //creamos nuevos subprocesos
    printf(" nuevos subprocesos \n");
    //nuevo subproceso
    pthread_t thread_pool[ THREAD_N ];

    //primer proceso de ejecucion
    printf(" primer proceso \n");
    sem_init( &semaforoT, 0, 1 );

    //hilos creados
    printf(" thread nuevos creados. \n");

    //variable para el for
    int j;

    
    for( j = 0; j < THREAD_N; j++ ) {
        pthread_create( &thread_pool[ j ], NULL, &thread_runner, log_semaphore );   
    }

    printf(" Thread join \n");
    for( j = 0; j < THREAD_N; j++ ) {
        pthread_join( thread_pool[j], NULL );
    }

    printf(" File close \n");
    fclose( log_semaphore );
    return 0;

}
