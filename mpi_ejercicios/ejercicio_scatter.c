#include <stdio.h>
#include <mpi.h>

#define N 10

void imprimir_vector(int* vector,int size);

int main(int argc, char* argv[]){

	int size, rank, i, tam_scatter, VA[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//Inicializo vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}

	//Dar valores al vector si soy el maestro
	if(rank==0)
		for (i=0; i<N; i++)
			VA[i] = i;


	printf("Proceso %d: VA antes de scatter:\n VA= ",rank);
	imprimir_vector(VA,N);
	printf("\n\n");

	tam_scatter=N/size;
	MPI_Scatter(VA,tam_scatter,MPI_INT,VA,tam_scatter,MPI_INT,0,MPI_COMM_WORLD);

	printf("Proceso %d: VA tras scatter\n VA= ",rank);
	imprimir_vector(VA,tam_scatter);
	printf("\n\n");

	MPI_Finalize();
	return 0;
}

void imprimir_vector(int* vector,int size){
	printf("[");
	for (int i=0; i<size; i++)
		printf("%d  ",vector[i]);
	printf("]");
}