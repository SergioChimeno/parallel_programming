#include <stdio.h>
#include <mpi.h>

#define N 10

int main(int argc, char* argv[]){

	int size, rank, i, VA[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//Inicializo vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}

	//Añadir el código para:
	//  - Si soy maestro, inicializo el vector almacenando en cada elemento del vector, su índice i
	//	- Tanto si soy maestro como si soy esclavo, imprimir el vector VA
	//  - Enviar el VA a TODOS los procesos usando Broadcast (los esclavos lo recibirán)
	//  - Tanto si soy maestro como si soy esclavo, imprimir el vector VA

	if(rank==0)
		for (i=0; i<N; i++)
			VA[i] = i;


	printf("Proceso %d: VA antes de broadcast:\n VA= [",rank);
	for (i=0; i<N; i++)
		printf("%d  ",VA[i]);
	printf("]\n\n");

	MPI_Bcast(VA,N,MPI_INT,0,MPI_COMM_WORLD);

	printf("Proceso %d: VA tras broadcast\n VA= [",rank);
	for (i=0; i<N; i++)
		printf("%d  ",VA[i]);
	printf("]\n\n");

	MPI_Finalize();
	return 0;
}

