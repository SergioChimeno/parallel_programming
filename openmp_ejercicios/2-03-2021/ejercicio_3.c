#include <stdio.h>
#include <omp.h>

int main(){

    int nthreads, tid, condicion=1;

    printf("->Hola antes de la region paralela\n");

    #pragma omp parallel num_threads(8) private(tid) if(condicion>0)
    {

        tid=omp_get_thread_num();
        nthreads=omp_get_num_threads();
        printf("Hola desde el thread %d de %d threads\n",tid,nthreads);

    }

    printf("->Hola despues de la region paralela\n");

    return 0;

}