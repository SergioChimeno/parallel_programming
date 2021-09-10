#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a,b) (a<b?b:a)
#define N_esclavo 100     //Tamaño del vector en el esclavo
#define N_maestro 100000  //Tamaño del vector en el maestro
#define RECEPCIONES_ESPERADAS (N_maestro/N_esclavo) //Recepciones que espera recibir el maestro

int calcular_maximo(int* vector,int tam){
    int maximo=0;
    for(int i=0;i<tam;i++)
        if(vector[i]>maximo)
            maximo=vector[i];
    return maximo;
}

int main(int argc, char* argv[]){
    int rank, nprocs, i, *vector,tag_datos = 0,tag_finalizar = 1, maximo;

    MPI_Status status_Send,status_Recv;
    MPI_Request request_Send,request_Recv;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank == 0){
        double tiempo_inicial, tiempo_total;
        int esclavo, maximo_total=0, maximo_secuencial=0;

        tiempo_inicial=MPI_Wtime();
        //Inicializar el vector
        srand ( time ( NULL));
        vector = malloc(N_maestro*sizeof(int));
        for(i=0;i<N_maestro;i++)
            vector[i]=rand()%100001;

        int* puntero_send=vector;
        //Dar 100 elementos a cada esclavo
        MPI_Request* requests_slaves=malloc(nprocs*sizeof(MPI_Request));
        for(i=1;i<nprocs;i++,puntero_send+=N_esclavo)
            MPI_Isend(puntero_send,N_esclavo,MPI_INT,i,tag_datos,MPI_COMM_WORLD,&requests_slaves[i]);

        maximo_secuencial=calcular_maximo(vector,N_esclavo*(nprocs-1));//mientras envia, calcula el maximo

        for(i=0;i<RECEPCIONES_ESPERADAS;i++){
            //Espera a que alguno acabe
            MPI_Probe(MPI_ANY_SOURCE,tag_datos,MPI_COMM_WORLD,&status_Recv);
            esclavo = status_Recv.MPI_SOURCE;

            MPI_Irecv(&maximo,1,MPI_INT,esclavo,tag_datos,MPI_COMM_WORLD,&request_Recv);//Obtener el máximo
            //le envia 100 elementos mas al que acabo
            if(puntero_send < vector+N_maestro){
                MPI_Isend(puntero_send,N_esclavo,MPI_INT,esclavo,tag_datos,MPI_COMM_WORLD,&requests_slaves[esclavo]);
                maximo_secuencial=MAX(maximo_secuencial,calcular_maximo(puntero_send,N_esclavo));//mientras envia, calcula el maximo
                puntero_send+=N_esclavo;
            }

            //espera a recibir el maximo del esclavo
            MPI_Wait(&request_Recv,&status_Recv);
            printf("-Esclavo: %02d - Máximo calculado: %06d\n",esclavo,maximo);

            //El maestro calcula el maximo
            maximo_total = MAX(maximo,maximo_total);
        }

        //Avisar a todos los esclavos de que he acabado
        for(i=1;i<nprocs;i++)
            MPI_Isend(NULL,0,MPI_INT,i,tag_finalizar,MPI_COMM_WORLD,&requests_slaves[i]);

        tiempo_total=MPI_Wtime()- tiempo_inicial;
        printf("\n->Maestro - Máximo calculado: %06d\n",maximo_total);
        printf("->Maestro - Máximo calculado secuencialmente: %06d\n",maximo_secuencial);
        printf("\n->El tiempo que ha tomado es de: %lf, usando %d procesos\n",tiempo_total,nprocs);

        for(i=1;i<nprocs;i++) //esperar que los esclavos hayan recibido antes de salir del proceso maestro
            MPI_Wait(&requests_slaves[i],&status_Send);
        free(requests_slaves);
    }else{
        //Inicializar el vector
        vector = malloc(N_esclavo*sizeof(int));
        for(i=0;i<N_esclavo;i++)
            vector[i]=0;

        //Recibe el primer vector
        MPI_Irecv(vector,N_esclavo,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&request_Recv);
        MPI_Wait(&request_Recv,&status_Recv);

        while(status_Recv.MPI_TAG != tag_finalizar){
            maximo=calcular_maximo(vector,N_esclavo);//calcular maximo
            MPI_Isend(&maximo,1,MPI_INT,0,tag_datos,MPI_COMM_WORLD,&request_Send);//Enviar el maximo al maestro
            MPI_Irecv(vector,N_esclavo,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&request_Recv);
            MPI_Wait(&request_Recv,&status_Recv);
            MPI_Wait(&request_Send,&status_Send);
        }
    }

    free(vector);
    MPI_Finalize();
    return 0;
}
