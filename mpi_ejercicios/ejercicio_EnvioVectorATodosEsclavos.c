#include <stdio.h>
#include <mpi.h>

#define N 10

int main(int argc, char* argv[]){

	int size, rank, i, from, to, ndat, part, tag, VA[N];
	MPI_Status info;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//Inicializo el vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}
	tag=0;

	//Si soy maestro
	if (rank == 0){
		for (i=1; i<N; i++) {
			VA[i] = i;
		}


		int n_esclavos=size-1;
		if(n_esclavos>N) n_esclavos=N; //Si hay mas esclavos que posiciones en el array no se usaran

		part=N/n_esclavos;
		for(to=1;to<n_esclavos;to++)
			MPI_Send(VA+(to-1)*part,part,MPI_INT,to,tag,MPI_COMM_WORLD);

		MPI_Send(VA+(to-1)*part,part+N%n_esclavos,MPI_INT,to,tag,MPI_COMM_WORLD);

		//Añadir el código necesario para:

		//1. Calcular cuántos datos se envían a cada esclavo (por ejemplo, repartir 100 datos entre 4 esclavos-> 25 a cada uno)
		//2. Para todos los esclavos...
		//		2.1 Identificar destino y tag
		//      2.2. Distinguir entre los esclavos y el último esclavo
		//		2.3. Si no es el último esclavo, se le envía el número de datos que se calculó antes
		//		2.3. Si es el último esclavo, se le envían los datos restantes

	}

	//si soy esclavo...
	else if(rank<=N){
		printf("Proceso %d: VA antes de recibir datos: \n",rank);


		// Añadir el código necesario para:

		// 1. Imprimir el vector antes de recibir nada
		printf("[");
		for (i=1; i<N; i++) {
			printf("%d  ",VA[i]);
		}
		printf("]\n");
		// 2. Recibir los datos del maestro
		MPI_Recv(VA,N,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
		// 3. Calcular cuántos datos se han recibido del maestro
		MPI_Get_count(&info,MPI_INT,&ndat);

		printf("Proceso %d recibe VA de %d: tag %d, ndat %d; \nVA = ", rank, info.MPI_SOURCE, info.MPI_TAG, ndat);

		printf("[");
		for (i=0; i<ndat; i++) {
			printf("%d  ",VA[i]);
		}
		printf("]");
		printf("\n\n");
	}
	else{//No quedan posiciones de array para estos
		printf("Proceso %d: No recibe nada : \n",rank);
	}

	MPI_Finalize();
	return 0;
}


// compilar:
// salloc -p formacion -q formacion --mem-per-cpu=150 --time=00:15:00
// mpicc 02ejercicioEnvioVectorATodosEsclavos.c -o 02ejercicioEnvioVectorATodosEsclavos
// exit
