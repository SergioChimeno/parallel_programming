#include <mpi.h>
#include <stdio.h>

#define BUFF_SIZE 20

int main(int argc, char* argv[]){

    int  rank, split_key, new_rank;
    MPI_Comm comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    split_key=rank%3;
    MPI_Comm_split(MPI_COMM_WORLD,split_key,rank,&comm);
    MPI_Comm_rank(comm,&new_rank);

    printf("rank en MPI_COMM_WORLD: %3d; rank en el nuevo comunicador: %d\n",rank,new_rank);

    MPI_Finalize();
    return 0;
}