#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define TAM_DIMENSIONES_MATRIZ 10000
#define TAM_MATRIZ (TAM_DIMENSIONES_MATRIZ*TAM_DIMENSIONES_MATRIZ)

int main(int argc, char* argv[]){

    int rank, nprocs, i, tam_scatter;
    double tiempo_inicial,tiempo_total,media, *matriz, suma_parcial=.0, suma_total=.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    tam_scatter= TAM_MATRIZ / nprocs;

    //Paso 2 inicializar matriz
    if(rank==0){
        matriz = malloc(TAM_MATRIZ*sizeof(double));
        srand ( time ( NULL));
        for(i=0;i<TAM_MATRIZ;i++)
            matriz[i]=rand()%101;
    }else{
        matriz=malloc(tam_scatter*sizeof(double));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    tiempo_inicial=MPI_Wtime();
    //Paso 3 distribuir matriz
    MPI_Scatter(matriz,tam_scatter,MPI_DOUBLE,matriz,tam_scatter,MPI_DOUBLE,0,MPI_COMM_WORLD);

    //Paso 4 Calcular la suma
    for(i=0;i<tam_scatter;i++)
        suma_parcial+=matriz[i];

    //Paso 5 enviar la suma total a cada proceso
    MPI_Allreduce(&suma_parcial,&suma_total,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

    //Calcular media
    media=suma_total/TAM_MATRIZ;

    //Paso 6 dividir cada elemento por la media
    for(i=0;i<tam_scatter;i++)
        matriz[i]/=media;

    //Paso 7 enviar matriz actualizada al maestro
    MPI_Gather(matriz,tam_scatter,MPI_DOUBLE,matriz,tam_scatter,MPI_DOUBLE,0,MPI_COMM_WORLD);
    tiempo_total=MPI_Wtime()- tiempo_inicial;

    if(rank==0){
        printf("El tiempo total transcurrido es %lf, usando %d procesos\n",tiempo_total,nprocs);
        printf("La media de la matriz antes del paso 6 es %lf\n",media);
        printf("La suma total de la matriz antes del paso 6 es %lf\n",suma_total);
    }

    MPI_Finalize();
    return 0;
}