//Compilar: mpicc ejercicio.c -o ejercicio -lm
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 12000

int sum(int* vector,int size){
    int suma=0;
    for(int i=0;i<size;i++)
        suma+=vector[i];
    return suma;
}

int main(int argc, char* argv[]){
    int rank, nrpocs, coords[2], *matriz, rank_maestro,i,j, suma, suma_total;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nrpocs);

    //Topología carteasiana
    const int P = sqrt(nrpocs);
    int dims[2]={P,P};
    int periods[2]={0,0};
    MPI_Comm comm_2d;
    MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,1,&comm_2d);
    MPI_Comm_rank(comm_2d,&rank);
    MPI_Cart_coords(comm_2d,rank,2,coords);

    //rank maestro
    int coords_maestro[2]={0,0};
    MPI_Cart_rank(comm_2d,coords_maestro,&rank_maestro);

    const int tam_dim=N/P;
    if(rank == rank_maestro){
        //Inicializar matriz
        int* matriz=malloc(N*N*sizeof(int));
        srand ( time ( NULL));
        for(int i=0;i<N*N;i++)
            matriz[i]=rand()%10;

        MPI_Datatype submatriz_t;
        MPI_Type_vector(tam_dim,tam_dim,N,MPI_INT,&submatriz_t);
        MPI_Type_commit(&submatriz_t);

        //Enviar las submatrices
        for(i=0;i<P;i++)
            for(j=0;j<P;j++){
                if(i == 0 && j == 0) continue;
                //Calcular el rango
                int rank_send;
                int coords_send[2]={i,j};
                MPI_Cart_rank(comm_2d,coords_send,&rank_send);
                //Enviar
                MPI_Send(matriz + j*tam_dim + i*tam_dim*N,1,submatriz_t ,rank_send,0,comm_2d);
            }

        //Suma parcial
        suma=0;
        for(i=0;i<N*tam_dim;i+=N)
            for(j=0;j<tam_dim;j++)
                suma+=matriz[i+j];

        printf("Maestro %2d, suma calculada: %6d \n",rank,suma);
    }else{
        matriz=malloc(tam_dim*tam_dim*sizeof(int));
        MPI_Recv(matriz,tam_dim*tam_dim,MPI_INT,rank_maestro,0,comm_2d,MPI_STATUS_IGNORE);
        suma=sum(matriz,tam_dim*tam_dim);
        printf("Esclavo %2d, suma calculada: %6d \n",rank,suma);
    }

    MPI_Reduce(&suma,&suma_total,1,MPI_INT,MPI_SUM,rank_maestro,comm_2d);

    if (rank == rank_maestro) printf("\nMaestro %2d, suma TOTAL calculada: %6d \n",rank,suma_total);

    free(matriz);
    MPI_Finalize();
    return 0;
}

