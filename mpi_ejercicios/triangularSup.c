#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3

int main(int argc, char* argv[]){

    int  rank, i, *matriz;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank==0){
        //inicializar la matriz
        matriz= malloc(N*N*sizeof(int));
        int contador=0;
        for(i=0;i<N*N;i++)
            matriz[i]=contador++;

        //definir el tipo derivado
        MPI_Datatype triangular_sup;
        int blocklens[N];
        int indices[N];
        for(i=0;i<N;i++){
            blocklens[i]=N-i;
            indices[i]=i+i*N;
        }
        MPI_Type_indexed(N,blocklens,indices,MPI_INT,&triangular_sup);
        MPI_Type_commit(&triangular_sup);

        MPI_Bcast(matriz,1,triangular_sup,0,MPI_COMM_WORLD);
    }else{
        const int M=N*(N+1)/2;
        matriz=malloc(M*sizeof(int));
        MPI_Bcast(matriz,M,MPI_INT,0,MPI_COMM_WORLD);

        //imprimir matriz
        printf("Soy el hijo %d\n",rank);
        printf(" Matriz recibida: [");
        for(i=0;i<M;i++)
            printf(" %d",matriz[i]);
        printf(" ]\n");
    }

    MPI_Finalize();
    return 0;
}