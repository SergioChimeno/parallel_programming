#include <omp.h>
#include <stdio.h>
#include <math.h>
double funcion(double a);

int main(int argc, char *argv[]){
	int n, i;
	double PI25DT = 3.141592653589793238462643;
	double pi, h,x;
    pi=0;
	n=100000; //Número de intervalos
	h= 1.0 / (double) n;

    omp_set_num_threads(8);
    double start_time = omp_get_wtime();
	//La variable pi es una variable de reducción (+)
    #pragma omp parallel for schedule(static) firstprivate(x) reduction(+:pi)
    for(i=1;i<=n;i++){
        x=h*((double)i - 0.5);
        pi+=funcion(x);
    }

	pi=h*pi;

    double time = omp_get_wtime() - start_time;
	printf("\n Pi es aproximadamente %.16f.El error cometido es %.16f\n", pi, fabs(pi-PI25DT));
	printf("\n Tiempo: %.16f\n",time);
    return 0;
}

double funcion(double a){
	return (4.0/(1.0 + a*a));
}