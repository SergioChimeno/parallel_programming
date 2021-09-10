#include <mpi.h>
#include <stdio.h>

#define N 40

int main(int argc, char* argv[]){

    int  rank, i, vector[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Datatype uno_si_uno_no;
    MPI_Type_vector(N/2,1,2,MPI_INT,&uno_si_uno_no);
    MPI_Type_commit(&uno_si_uno_no);

    if(rank==0){
        for(i=0;i<N;i++)
            vector[i]=i;
        MPI_Send(vector,1,uno_si_uno_no ,1, 0, MPI_COMM_WORLD);//Pares
        MPI_Send(vector+1,1,uno_si_uno_no ,2, 0, MPI_COMM_WORLD);//Impares
    }else if(rank==1 || rank==2){
        MPI_Recv(vector,1,uno_si_uno_no,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        int sum=0;
        //media
        for(i=0;i<N;i+=2)
            sum+=vector[i];

        float media = (float)sum / ((float)N/2.0f);

        printf("Soy el proceso %d, media: %f\n",rank,media,sum);
    }

    MPI_Finalize();
    return 0;
}