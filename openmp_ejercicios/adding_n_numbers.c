#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    int n=10000000;//cantidad de numeros a sumar
    srand(time(NULL));
    int* random_numbers=malloc(n*sizeof(int));

    //inicializar numeros
    for(int i=0;i<n;i++)
        random_numbers[i]=rand();

    int suma_total=0;
    omp_set_num_threads(4);

    printf("num threads: %d\n",omp_get_max_threads());

    //timepo inicial
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(static) reduction(+: suma_total)
    for(int i=0;i<n;i++){
        suma_total+=random_numbers[i];
    }

    printf("El resultado de la suma es %d",suma_total);

    //tiempo total
    double tiempo = omp_get_wtime() - start_time;
	printf("\n Tiempo: %.16f\n",tiempo);

    free(random_numbers);

    return 0;
}