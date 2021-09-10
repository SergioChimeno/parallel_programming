#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

int main(int argc, char* argv[]){

    int  rank, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank==0){
        //Inicializar matriz
        int j, matriz[N][N];
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                matriz[i][j]=i*j;

        //Definir tipo diagonal
        MPI_Datatype tipo_diagonal;
        MPI_Type_vector(N,1,N+1,MPI_INT,&tipo_diagonal);
        MPI_Type_commit(&tipo_diagonal);

        MPI_Bcast(matriz,1,tipo_diagonal,0,MPI_COMM_WORLD);
    }else{
        int* vector[N];
        MPI_Bcast(vector,N,MPI_INT,0,MPI_COMM_WORLD);

        //Imprimir vector diagonal
        printf("Soy el proceso con rank %d\n",rank);
        printf(" He recibido el vector: [");
        for(i=0;i<N;i++)
            printf(" %d",vector[i]);
        printf("]\n");
    }

    MPI_Finalize();
    return 0;
}