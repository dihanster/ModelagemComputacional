/*
PROJETO 4 - MODELAGEM COMPUTACIONAL
Prof. Dr. Marcos Gonçalves Quiles
Propagação de Informação - Consenso

Nomes: Ana Júlia de Oliveira Bellini			  RA: 111774
	   Luiz Filipe Moraes Saldanha de Oliveira    RA: 112229
	   Willian Dihanster Gomes de Oliveira        RA: 112269
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*parâmetros*/
#define n 1000          //Numero de Vertices
#define opcoes 10       //Numero de Opcoes
#define min 900        //Numero minimo de opinioes iguais
#define t 10000         //Numero maximo de ciclos

/*Estrutura do Vértice*/
struct vertice {
    int opcao; //Opcao atual do vertice
    int nAdjacentes; //Quantidade de vertices adjacentes do vertice
    int oc; //Variavel para verificar se o vertice ja foi escolhido no ciclo principal
    int* vetAdjacentes; //Lista com vertices adjcaentes
    int prob; //Influencia do vertice no debate
}; typedef struct vertice tipoV;

/*Vetores e Matrizes Globais*/
tipoV vetorVertices[n];
int grafo[n][n]; //matriz de adjacência

/*Inicializar*/
void inicia() {
    srand(time(0));

    for (int i = 0; i < n; i++) {
        vetorVertices[i].nAdjacentes = 0;
        vetorVertices[i].opcao = (rand() % opcoes) + 1; //Sorteia opiniao inicial do vertice i
        //printf("%d ", vetorVertices[i].opcao);
        vetorVertices[i].prob = (rand()%opcoes) + 1; //Sorteia a força do vertice i no debate
		for (int j = 0; j < n; j++) { //Zerar matriz adjacencia
            grafo[i][j] = 0;
		}
	}
}

/*Inicia os adj */
void iniciaAdj() {
    int i, j, cont = 0;

    for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
            if (grafo[i][j] == 1) {
                vetorVertices[i].nAdjacentes = vetorVertices[i].nAdjacentes + 1; //Verifica a quantidade de vertices adjacentes ao vertice i
            }
		}
        vetorVertices[i].vetAdjacentes = (int *) calloc(vetorVertices[i].nAdjacentes, sizeof(int)); //Inicia lista com vertices adjacentes
	}

	for(i = 0; i < n; i++){
        cont = 0;
        for(j = 0; j < n; j++){
            if(grafo[i][j] == 1){
                vetorVertices[i].vetAdjacentes[cont] = j; //Se i e j são adjacentes, então j é incluído na lista de adjacentes de i
                cont++;
            }
        }
	}
}

/*Inicializar Rede Aleatória*/
void iniciaGrafoAleatorio(int k) {
    srand(time(0));

    int i, j;
    float r, comp = (float) k/(2*n);
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            if (i != j){
                r = (float) rand() / RAND_MAX;
                if (r <= comp){
                    grafo[i][j] = 1;
                    grafo[j][i] = 1;
                }
            }
        }
    }
}

/*inicia Rede Barabasi-Albert (Livre de Escala)*/
void iniciaGrafoBarabas() {
    int i, j, tamLista = 0, lista[(2 * n) + 1];
    grafo[0][1] = grafo[1][0] = 1;
    lista[0] = 0;
    lista[1] = 1;
    tamLista = 2;

    srand(time(0));

    for (i = 2; i < n; i++){
        j = rand() % (tamLista);
        j = lista[j];
        grafo[i][j] = grafo[j][i] = 1;
        lista[tamLista] = i;
        lista[tamLista+1] = j;
        tamLista = tamLista + 2;
    }
}

/*Inicia Rede Regular*/
void iniciaGrafoRegular(int k) {
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            grafo[i][j] = 0;
        }
    }
	for (j = 1; j <= (k / 2); j++) {
		for (i = 0; i < (n - j); i++) {
			grafo[i][i + j] = 1;
			grafo[i + j][i] = 1;
		}
	}

	for (i = (n - (k / 2)); i < n; i++) {
		for (j = 0; j < (k / 2); j++) {
			if ((i - j) >= (n - (k / 2))) {
				grafo[i][j] = 1;
				grafo[j][i] = 1;
			}
		}
	}
}

/*Faz a leitura da Rede Modular do arquivo fornecido*/
void leGrafoModular(){
    FILE *arquivo;
    arquivo = fopen("network.txt", "r");
    int tam = 0, v1, v2;

    if(arquivo == NULL){
        printf("Houve um erro ao abrir o arquivo!");
        exit(1);
    }
    else{
        while(!feof(arquivo) && tam < 4954){
            fscanf(arquivo, "%d", &v1);
            fscanf(arquivo, "%d", &v2);
            grafo[v1-1][v2-1] = grafo[v2-1][v1-1] = 1; //Pq indice eh de 0 a 999, e vertices do txt sao de 1 a 1000
            //printf("vertice %d eh adj a %d\n", v1, v2);
            tam++;
        }
    }
    fclose(arquivo);
}

/*Calcula propagação de informações*/
void calculaPropagacao() { //Ciclo principal
    int cont = 0, cond = 0, para = 0, ciclo = 0, ciclo2 = 0, i, j, r;
    int contOpinioes[opcoes];
    double r_d;

    FILE *plotPropag;
	plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    srand(time(0));
    //while (1) {
    while (para == 0 && ciclo2 <= t) {
    //for(int ciclo2 = 0; ciclo2 < 10000; ciclo2++){
        ciclo2++;
        for (i = 0; i < n; i++)
            vetorVertices[i].oc = 0; //vetorVertices[i].oc deve ser 0 no inicio de cada ciclo

         for (i = 0; i < opcoes; i++)
            contOpinioes[i] = 0; //Vetor com a quantidade de de vertices que escolhem a opcao i


        for (i = 0; i < n; i++) {
            cont = 0;
            if (vetorVertices[i].oc == 0 && vetorVertices[i].nAdjacentes > 0) {
                vetorVertices[i].oc = 1; //Se um vertice i for escolhido, ele nao deve entrar em debate novamente
                for (j = 0; j < vetorVertices[i].nAdjacentes; j++) {
                    if (vetorVertices[vetorVertices[i].vetAdjacentes[j]].oc == 1) //Verifica quantos  vertices adjacente ao vertice i ja entraram em debate
                        cont++;
                }

                if (cont != vetorVertices[i].nAdjacentes){ //O vertice entra em debate apenas se houver um vertice adjacente disponivel
                    r = rand() % vetorVertices[i].nAdjacentes; //Sorteia um vertice adjacente ao vertice i
                    while (vetorVertices[vetorVertices[i].vetAdjacentes[r]].oc == 1){ //Verifica se o vertice sorteado esta disponivel
                        r = rand() % vetorVertices[i].nAdjacentes;
                    }
                    j = vetorVertices[i].vetAdjacentes[r]; //j é o vertice que entra em debate com o vertice i
                    vetorVertices[j].oc = 1; //j deve estar indisponivel para os proximos debates neste ciclo

                    int soma = vetorVertices[i].prob + vetorVertices[i].opcao +
                               vetorVertices[j].prob + vetorVertices[j].opcao; //Soma das probabilidades de i e j para o srotei no debate
                    int nRand = (rand() % soma) + 1; //Sorteia um numero menor ou igual a soma das probabilidades de i e j

                    if (nRand <= (vetorVertices[i].prob + vetorVertices[i].opcao)){ //Se o valor sorteado esta entre 0 e vetorVertices[i].prob + vetorVertices[i].opcao
                        vetorVertices[j].opcao = vetorVertices[i].opcao;               //o vertice i ganha o debate
                    }
                    else {                                                       //Se o valor sorteado estiver entre vetorVertices[i].prob + vetorVertices[i].opcao e1
                        vetorVertices[i].opcao = vetorVertices[j].opcao;               //o vertice j ganha o debate
                    }
                }
            }
        }
        for(i = 0; i < n; i++) //Verifica quantos vertices na rede escolhem a opcao i
            contOpinioes[vetorVertices[i].opcao - 1] = contOpinioes[vetorVertices[i].opcao - 1] + 1;

        for(i = 0; i < opcoes; i++){
            if(i == opcoes-1) fprintf(plotPropag, "%d\n", contOpinioes[i]);

            else fprintf(plotPropag, "%d,", contOpinioes[i]);
            //Se alguma opcao for escolhida por mais que min vertices, o programa acaba
            if (contOpinioes[i] >= min){ //convergiu :D
                printf("\nNumero de iteracoes para convergir foram %d \n", ciclo2);
                para++;
            }
        }
        ciclo++;
    }
    fclose(plotPropag);
}

/*função main*/
int main(void) {
    int op, grau;
    int tmili, inicio, fim;
    srand(time(0));

	inicia();

    printf("Iniciando... \nEscolha a rede desejada:\n");
    printf("1- Rede Regular\n2- Rede Aleatoria\n3- Rede Livre de Escala\n4- Rede Modularizada\n");
    scanf("\n%d", &op);

    switch (op){
    case 1:
        printf("\nDigite o grau medio da rede\nOpcoes: 4 ou 8\n");
        scanf("\n%d", &grau);
        iniciaGrafoRegular(grau);
        break;
    case 2:
        printf("\nDigite o grau medio da rede\nOpcoes: 4 ou 8\n");
        scanf("\n%d", &grau);
        iniciaGrafoAleatorio(grau);
        break;
    case 3:
        iniciaGrafoBarabas();
        break;
    case 4:
        leGrafoModular();
        break;
    default:
        break;
    }

    iniciaAdj();

    //inicio = GetTickCount();

    calculaPropagacao();

    //fim = GetTickCount();

    //tmili = fim - inicio; //Conta o tempo que o programa leva para rodar
    //printf("Concluido a propagacao!\nForam gastos %d microsegundos", tmili);

	return 0;
}
