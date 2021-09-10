#include <omp.h>
#include <stdio.h>
int main(){
	int nthreads, tid;
	//La variable tid es privada a cada thread
    omp_set_num_threads(8);

	#pragma omp parallel sections private(tid)
	{

        //Another section
        #pragma omp section
        {
            //Get ID of thread
            tid=omp_get_thread_num();
            sleep(4);
            //Get number of threads
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 1\n", tid, nthreads);
        }

        //Another section
        #pragma omp section
        {
            //Get ID of thread
            tid=omp_get_thread_num();
            sleep(1);
            //Get number of threads
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 2\n", tid, nthreads);
        }

        //Another section
        #pragma omp section
        {
            //Get ID of thread
            tid=omp_get_thread_num();
            //Get number of threads
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 3\n", tid, nthreads);
        }

        //Another section
        #pragma omp section
        {
            //Get ID of thread
            tid=omp_get_thread_num();
            //Get number of threads
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 4\n", tid, nthreads);
        }

		 //Fin de las sections
	} //Fin de la parallel section
}