#include <mpi.h>
#include <stdio.h>

//Implementacion 2
int main(int argc, char* argv[]){

    int size, rank, tag, rank_recibido,rank_send;
    tag=0;
    MPI_Status status_Send,status_Recv;
    MPI_Request request_Send,request_Recv;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    rank_send=(rank+1)%size;

    MPI_Isend(&rank,1,MPI_INT,rank_send,tag,MPI_COMM_WORLD,&request_Send);
    MPI_Irecv(&rank_recibido,1,MPI_INT,(rank-1+size)%size,tag,MPI_COMM_WORLD,&request_Recv);
    MPI_Wait(&request_Send,&status_Send);
    MPI_Wait(&request_Recv,&status_Recv);

    printf("Soy el proceso %d, he recibido el rank %d y he enviado mi rank al %d\n\n",rank,rank_recibido,rank_send);

    MPI_Finalize();
    return 0;

}