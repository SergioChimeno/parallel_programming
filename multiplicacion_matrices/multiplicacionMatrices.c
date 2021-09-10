#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double wallclock(void)
{
	struct timeval tv;
	double t;
	
	gettimeofday(&tv, NULL);
	
	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;
	
	return t;
}

void mul_matrices(float** matriz_A, float** matriz_B,float** resultado, int tamano){
    for(int fila=0;fila<tamano;fila++){
        for(int columna=0;columna<tamano;columna++){
            for(int desplazamiento=0;desplazamiento<tamano;desplazamiento++){
                resultado[fila][columna]+=matriz_A[fila][desplazamiento]*matriz_B[desplazamiento][columna];
            }
        }
    }
}

void imprimir_matriz(float** matriz, int tamano){
    printf("[");
    for(int i=0;i<tamano && i<5;i++){
        if(i!=0){printf(" ");}
        printf("[");
        for(int j=0;j<tamano && j<8;j++){
            printf("%f",matriz[i][j]);
            printf(", ");
        }
        if(tamano>=8){printf("...");};
        printf("]");
        printf(",\n");
    }
    if(tamano>=5){
        printf("  ...\n");
    }
    printf("]\n"); 
}

float** reserva_memoria_matriz(int tamano_matriz){
    //Reservar memoria
    float* tmp=(float*)malloc(tamano_matriz*tamano_matriz*sizeof(float));
    float** matriz = malloc(tamano_matriz*sizeof(float*));

    //Rellenar el array de indices
    for(int i=0;i<tamano_matriz;i++){
        matriz[i]=tmp+i*tamano_matriz;
    }

    return matriz;
}

//Requisito: solo funciona con matrices cuadradas
int main(){
    int tamano_matrices=1100;

    //Reservar matrices
    float** matriz_A=reserva_memoria_matriz(tamano_matrices);
    float** matriz_B=reserva_memoria_matriz(tamano_matrices);
    float** matriz_resultado=reserva_memoria_matriz(tamano_matrices);

    //Inicializar cada celda con el numero de la fila, y la matriz de resultados a 0
    for(int i=0;i<tamano_matrices;i++)
        for(int j=0;j<tamano_matrices;j++){
            matriz_A[i][j]=i;
            matriz_B[i][j]=i;
            matriz_resultado[i][j]=0.0;
        }

    //multiplicar y tomar el tiempo
    double tiempo_inicial=wallclock();
    mul_matrices(matriz_A,matriz_B,matriz_resultado,tamano_matrices);
    double tiempo_final=wallclock();

    double tiempo_total=tiempo_final-tiempo_inicial;

    //Imprimir las matrices
    printf("Matriz A:\n");
    imprimir_matriz(matriz_A,tamano_matrices);
    printf("\n");
    printf("Matriz B:\n");
    imprimir_matriz(matriz_B,tamano_matrices);
    printf("\n");
    printf("Resultado:\n");
    imprimir_matriz(matriz_resultado,tamano_matrices);

    //Liberar espacio
    free(matriz_A[0]);
    free(matriz_B[0]);
    free(matriz_resultado[0]);
    free(matriz_A);
    free(matriz_B);
    free(matriz_resultado);

    printf("\nTiempo que ha tardado la multiplicacion: %lf\n",tiempo_total);
    
    return 0;
}
