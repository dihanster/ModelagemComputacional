/*
PROJETO 3 - MODELAGEM COMPUTACIONAL
Prof. Dr. Marcos Gonçalves Quiles
Sincronização de Rede de Neurônios Acoplados

Nomes: Ana Júlia de Oliveira Bellini			  RA: 111774
	   Luiz Filipe Moraes Saldanha de Oliveira    RA: 112229
	   Willian Dihanster Gomes de Oliveira        RA: 112269
*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*parâmetros*/
#define n 500
#define contAtivos 250
#define ativo 0.2
#define inativo -0.02
#define deltaT 0.01
#define t 2000
#define x 1
#define y 2
#define I 3
#define Si 4
#define w 0.7
#define alpha 6.0
#define beta 0.1
#define epsilon 0.02
#define theta 0.5
#define k 4

/*inicializar matriz dos neurônios*/
void iniciaNeuronios(float mNeuronios[n][4]) {
    int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 4; j++) {
            mNeuronios[i][j] = 0;
		}
	}
}

/*inicializar grafo*/
void iniciaGrafo(int grafo[n][n]){
    for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
            grafo[i][j] = 0;
		}
	}
}

/*imprimir grafo*/
void imprimeGrafo(int grafo[n][n]){
    for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
            printf("%d ", grafo[i][j]);
		}
        printf("\n");
	}
}

/*inicializar rede aleatória*/
void iniciaGrafoAleatorio(int grafo[n][n]){
    int i, j;
    float r, comp = (float) k/(2*n);
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            if (i != j){
                r = (float) rand()/RAND_MAX;
                if (r <= comp){
                    grafo[i][j] = 1;
                    grafo[j][i] = 1;
                }
            }
        }
    }
}

/*inicializar rede livre de escala*/
void iniciaGrafoBarabas(int grafo[n][n]){
    int i, j, tamLista = 0, lista[(2 * n) + 1];
    grafo[0][1] = grafo[1][0] = 1;
    lista[0] = 0;
    lista[1] = 1;
    tamLista = 2;

    for (i = 2; i < n; i++){
        j = rand() % (tamLista);
        j = lista[j];
        grafo[i][j] = grafo[j][i] = 1;
        lista[tamLista] = i;
        lista[tamLista+1] = j;
        tamLista = tamLista + 2;
    }
}

/*inicializar rede regular*/
void iniciaGrafoRegular(int grafoR[n][n]) {
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            grafoR[i][j] = 0;
        }
    }
	for (j = 1; j <= (k / 2); j++) {
		for (i = 0; i < (n - j); i++) {
			grafoR[i][i + j] = 1;
			grafoR[i + j][i] = 1;
		}
	}

	for (i = (n - (k / 2)); i <= n; i++) {
		for (j = 0; j < (k / 2); j++) {
			if ((i - j) >= (n - (k / 2))) {
				grafoR[i][j] = 1;
				grafoR[j][i] = 1;
			}
		}
	}
}

/*ativação de neurônios*/
void ativaNeuronios(float mNeuronios[n][4]) {
	int i;
    srand(time(NULL));

	for (i = 0; i < contAtivos; i++){
		mNeuronios[i][I] = ativo;
		mNeuronios[i][x] =  (float) (2.0*rand() / RAND_MAX) * pow((-1), rand());
		mNeuronios[i][y] = (float) (4.0*rand() / RAND_MAX);
	}

	for (i = contAtivos; i < n; i++){
		mNeuronios[i][I] = inativo;
		mNeuronios[i][x] = 0;
		mNeuronios[i][y] = 0;
	}

}

/*calcular potencial dos neurônios*/
void calcPotencial(float mNeuronios[n][4], int grafo[][n]) {
    int printou = 0, ciclo, i, j, ka = 0;
    FILE *plotNeuro;
	plotNeuro = fopen("plotneuro.txt", "w");
    float deltaX, deltaY;

    for (ciclo = 0; ciclo <= (t / deltaT); ciclo++) {
		for (i = 0; i < n; i++) {
            mNeuronios[i][Si] = 0;
			for (j = 0; j < n; j++) {
                if(j == i) continue;
				if ((grafo[j][i] == 1) && ((mNeuronios[j][x]) >= 0)){
					mNeuronios[i][Si] = mNeuronios[i][Si] + w;
				}
			}
		}

		for (i = 0; i < n; i++) {
            deltaX = (((3 * mNeuronios[i][x]) - (pow(mNeuronios[i][x], 3)) + 2 - mNeuronios[i][y] + mNeuronios[i][Si]
				+ mNeuronios[i][I]) * deltaT);

            /*if para plotar alguns neurônios deslocados no gráfico, para mostrar melhor o comportamento de cada um*/
            /*if (ciclo % 100 == 0 && i % 50 == 0) {
                fprintf(plotNeuro, "%f,", mNeuronios[i][x] + ka);
                printou = 1;
                ka += 5;
            }*/

            /*if plota todos neurônios juntos, para observar melhor a sincronização*/
            if (ciclo % 100 == 0) {
                fprintf(plotNeuro, "%f,", mNeuronios[i][x]);
                printou = 1;
            }

			deltaY = (epsilon * (alpha * (1 + tanh(mNeuronios[i][x] / beta))- mNeuronios[i][y]) * deltaT);

            mNeuronios[i][x] = mNeuronios[i][x] + deltaX;
            mNeuronios[i][y] = mNeuronios[i][y] + deltaY;

		}

		// ka = 0;

        if (printou){
            fprintf(plotNeuro, "\n");
            printou = 0;
        }
	}
    fclose(plotNeuro);
}

/*função main*/
int main(void) {
	float mNeuronios[n][4];
	int grafo[n][n];
	FILE *plotNeuro;
	plotNeuro = fopen("plotneuro.txt", "w");

	iniciaNeuronios(mNeuronios);
	iniciaGrafo(grafo);
	iniciaGrafoRegular(grafo);
	// iniciaGrafoAleatorio(grafo);
	// iniciaGrafoBarabas(grafo);
	// imprimeGrafo(grafo);
	ativaNeuronios(mNeuronios);
	calcPotencial(mNeuronios, grafo);

	fclose(plotNeuro);
	return 0;
}
