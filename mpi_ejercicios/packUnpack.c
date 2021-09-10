#include <mpi.h>
#include <stdio.h>

#define BUFF_SIZE 200

int main(int argc, char* argv[]){

    float num_reales[2];
    int num_entero, rank, position=0;
    char buff[BUFF_SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank==0){
        printf("Introduce un número real: ");
        scanf("%f",num_reales);
        printf("\nIntroduce otro número real: ");
        scanf("%f",num_reales+1);
        printf("\nIntroduce un número entero: ");
        scanf("%d",&num_entero);

        // Valores de prueba
        // num_reales[0]=1.2;
        // num_reales[1]=2.3;
        // num_entero=12;

        MPI_Pack(num_reales,2,MPI_FLOAT,buff,BUFF_SIZE,&position,MPI_COMM_WORLD);
        MPI_Pack(&num_entero,1,MPI_INT,buff,BUFF_SIZE,&position,MPI_COMM_WORLD);
        MPI_Bcast(buff,position,MPI_PACKED,0,MPI_COMM_WORLD);

    }else{
        MPI_Bcast(buff,BUFF_SIZE,MPI_PACKED,0,MPI_COMM_WORLD);
        MPI_Unpack(buff,BUFF_SIZE,&position,num_reales,2,MPI_FLOAT,MPI_COMM_WORLD);
        MPI_Unpack(buff,BUFF_SIZE,&position,&num_entero,1,MPI_INT,MPI_COMM_WORLD);

        printf("\n\nSoy el hijo: %d", rank);
        printf("\n Me han llegado los número reales: %f y %f", num_reales[0], num_reales[1]);
        printf("\n Me ha llegado el número entero: %d", num_entero);
    }


    MPI_Finalize();
    return 0;
}