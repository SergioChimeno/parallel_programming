#include <omp.h>
#include <stdio.h>
void sumaparcial(double *a, int n);
double sumatotal(double *a, int n, int THREADS);
double suma(double *a, int n, int THREADS);

void multparcial(double *a, int n);
double multtotal(double *a, int n, int THREADS);
double producto(double *a, int n, int THREADS);

int main(){
	int nthreads, tid;
	double vector[100],resultadosuma,resultadoproducto;
	int i,tam=100;
	srand(time(NULL));
	for (i=0; i<tam; i++)
		vector[i]=rand()%5;


    #pragma omp parallel sections private(tid) firstprivate(vector) num_threads(2)
    {
        #pragma omp section
        {
            //Qué thread soy?
            tid=omp_get_thread_num();
            //Cuántos threads hay?
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 1\n", tid,nthreads);
            resultadosuma=suma(vector,tam,4);
            printf("El resultado de la suma es %f\n",resultadosuma);
        }

        #pragma omp section
        {
            //Qué thread soy?
            tid=omp_get_thread_num();
            //Cuántos threads hay?
            nthreads=omp_get_num_threads();
            printf("El thread %d, de %d, calcula la sección 2\n", tid, nthreads);
            resultadoproducto=producto(vector,tam,4);
            printf("El resultado del producto es %f\n",resultadoproducto);
        }
    }
}


void sumaparcial(double *a, int n){
	int i;
	double s=0;
	for(i=0;i<n;i++)
		s+=a[i];
	a[0]=s;
}
double sumatotal(double *a, int n, int THREADS){
	int i;
	double s=0;
	for(i=0;i<n;i+=n/THREADS)
		s+=a[i];
	return s;
}
double suma(double *a, int n, int THREADS){
	int i;
    #pragma omp parallel for schedule(static) num_threads(THREADS)
	for (i=0;i<THREADS;i++){
        sumaparcial(&a[i*n/THREADS],n/THREADS);
        printf("Soy el thread %d\n", omp_get_thread_num());
	}
	return sumatotal(a,n,THREADS);
}



void multparcial(double *a, int n){
	int i;
	double s=1;
	for(i=0;i<n;i++)
		s*=a[i];
	a[0]=s;
}
double multtotal(double *a, int n, int THREADS){
	int i;
	double s=1;
	for(i=0;i<n;i+=n/THREADS)
		s*=a[i];
	return s;
}
double producto(double *a, int n, int THREADS){
	int i;
    #pragma omp parallel for schedule(static) num_threads(THREADS)
	for (i=0;i<THREADS;i++){
        multparcial(&a[i*n/THREADS],n/THREADS);
        printf("Soy el thread %d\n", omp_get_thread_num());
	}
	return multtotal(a,n,THREADS);
}

