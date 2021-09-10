#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

void mul_matrices_un_bucle(float** matriz_A, float** matriz_B,float** resultado, int tamano){

    int fila,columna;
    double suma=0.;
    float* resultadop = resultado[0];
    float* matriz_Ap = matriz_A[0];
    float* matriz_Bp = matriz_B[0];

    #pragma omp parallel for schedule(runtime) private(fila,columna,suma)
    for(int it=0;it<tamano*tamano;it++){
        fila=it/(tamano);
        columna=it%tamano;
        suma=0.;
        for(int desplazamiento=0;desplazamiento<tamano;desplazamiento++)
            suma+=matriz_Ap[fila*tamano+desplazamiento]*matriz_Bp[desplazamiento*tamano+columna];

        resultadop[it]=suma;
    }

    // #pragma omp parallel for schedule(static) private(fila,desplazamiento,columna)
    // for(int it=0;it<tamano*tamano*tamano;it++){
    //     fila=it/(tamano*tamano);
    //     desplazamiento=(it/tamano)%tamano;
    //     columna=it%(tamano);

    //     resultado[fila][columna]+=matriz_A[fila][desplazamiento]*matriz_B[desplazamiento][columna];
    // }
}

void suma_matrices_un_bucle(float** matriz_A, float** matriz_B,float** resultado, int tamano){

    float* matriz_Ap=matriz_A[0];
    float* matriz_Bp=matriz_B[0];
    float* resultadop=resultado[0];

    #pragma omp parallel for schedule(runtime)
    for(int it=0;it<tamano*tamano;it++)
        resultadop[it]=matriz_Ap[it]+matriz_Bp[it];
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
    //Imprimir numero de threads disponibles
    printf("OpenMP-parallel with %1d threads\n", omp_get_max_threads());

    int TAMANO_MATRICES=500;

    //Reservar matrices
    float** matriz_A=reserva_memoria_matriz(TAMANO_MATRICES);
    float** matriz_B=reserva_memoria_matriz(TAMANO_MATRICES);
    float** matriz_resultado_mult=reserva_memoria_matriz(TAMANO_MATRICES);
    float** matriz_resultado_suma=reserva_memoria_matriz(TAMANO_MATRICES);

    //Inicializar cada celda con el numero de la fila, y la matriz de resultados a 0
    #pragma omp parallel for schedule(static)
    for(int i=0;i<TAMANO_MATRICES;i++)
        for(int j=0;j<TAMANO_MATRICES;j++){
            matriz_A[i][j]=1;
            matriz_B[i][j]=1;
            matriz_resultado_suma[i][j]=0.0;
            matriz_resultado_mult[i][j]=0.0;
        }

    //multiplicar,sumar y tomar el tiempo
    double start_time = omp_get_wtime();
    #pragma omp parallel sections
    {

        #pragma omp section
        {
            mul_matrices_un_bucle(matriz_A,matriz_B,matriz_resultado_mult,TAMANO_MATRICES);
        }

        #pragma omp section
        {
            suma_matrices_un_bucle(matriz_A,matriz_B,matriz_resultado_suma,TAMANO_MATRICES);
        }
    }

    double tiempo = omp_get_wtime() - start_time;

    //Imprimir las matrices
    printf("Matriz A:\n");
    imprimir_matriz(matriz_A,TAMANO_MATRICES);
    printf("\n");
    printf("Matriz B:\n");
    imprimir_matriz(matriz_B,TAMANO_MATRICES);
    printf("\n");
    printf("Resultado multiplicación:\n");
    imprimir_matriz(matriz_resultado_mult,TAMANO_MATRICES);
    printf("Resultado suma:\n");
    imprimir_matriz(matriz_resultado_suma,TAMANO_MATRICES);

    //Liberar espacio
    free(matriz_A[0]);
    free(matriz_B[0]);
    free(matriz_resultado_mult[0]);
    free(matriz_resultado_suma[0]);
    free(matriz_A);
    free(matriz_B);
    free(matriz_resultado_mult);
    free(matriz_resultado_suma);

    printf("\nTiempo que ha tardado la multiplicacion y la suma: %lf\n",tiempo);

    return 0;
}