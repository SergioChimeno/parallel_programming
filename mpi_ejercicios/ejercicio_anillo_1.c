/*

Cada proceso manda su rank al de su derecha
y recibe el rank del de su izquierda

*/
#include <mpi.h>
#include <stdio.h>

//Implementacion 1
int main(int argc, char* argv[]){

    int size, rank, tag, rank_recibido,rank_send;
    tag=0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    rank_send=(rank+1)%size;

    if(rank % 2){
        MPI_Send(&rank,1,MPI_INT,rank_send,tag,MPI_COMM_WORLD);
        MPI_Recv(&rank_recibido,1,MPI_INT,(rank-1+size)%size,tag,MPI_COMM_WORLD,&status);
    }else{
        MPI_Recv(&rank_recibido,1,MPI_INT,(rank-1+size)%size,tag,MPI_COMM_WORLD,&status);
        MPI_Send(&rank,1,MPI_INT,rank_send,tag,MPI_COMM_WORLD);
    }

    printf("Soy el proceso %d, he recibido el rank %d y he enviado mi rank al %d\n\n",rank,rank_recibido,rank_send);

    MPI_Finalize();
    return 0;

}