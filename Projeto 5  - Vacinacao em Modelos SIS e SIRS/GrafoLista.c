#include <stdio.h>
#include <stdlib.h>

#define MAX_TAM 1000

struct listaAdj{
	int item;
	struct listaAdj *proxVerticeAdj;
};typedef struct listaAdj tipoListaAdj;

struct grafo{
	tipoListaAdj *vertices[MAX_TAM];
};typedef struct grafo tipoGrafo;

tipoGrafo *criaGrafo(){

	tipoGrafo *grafo = (tipoGrafo*) malloc(sizeof(tipoGrafo));
	int i;

	for (i = 0; i < MAX_TAM; i++){
		grafo->vertices[i] = NULL;
	}

	return grafo;
}

tipoListaAdj* crialistaAdj(int item){

	tipoListaAdj *listaAdj = (tipoListaAdj*) malloc(sizeof(tipoListaAdj));
	listaAdj->item = item;
	listaAdj->proxVerticeAdj = NULL;

	return listaAdj;
}

void adicionaAresta(tipoGrafo *grafo, int verticeX, int verticeY){

	tipoListaAdj *vertice;
	vertice = crialistaAdj(verticeY);
	vertice->proxVerticeAdj = grafo->vertices[verticeX];
	grafo->vertices[verticeX] = vertice;

    //Para grafos nao direcionados:
	/*vertice = crialistaAdj(verticeX);
	vertice->proxVerticeAdj = grafo->vertices[verticeY];
	grafo->vertices[verticeY] = vertice;
    */
}

void imprimeGrafo(tipoGrafo *grafo){
    int i;
    tipoListaAdj *aux;

    for (i = 0; i < MAX_TAM; i++){
        aux = grafo->vertices[i];
        while(aux != NULL){
            printf("Vertice %d eh adj a %d \n", i, aux->item);
            aux = aux->proxVerticeAdj;
        }
    }

}

void leGrafoModular(tipoGrafo *grafo){
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
            adicionaAresta(grafo, v1-1, v2-1);
            //adicionaAresta(grafo, v2-1, v1-1);
            tam++;
        }
    }
    fclose(arquivo);
}

int main(){
	tipoGrafo *grafo = criaGrafo();

	leGrafoModular(grafo);
	//imprimeGrafo(grafo);

	tipoListaAdj *aux;
	aux = grafo->vertices[999];
    tipoListaAdj *aux2;
    int i;
	printf("Vertice 999 eh adj a:\n");
	while(aux != NULL){
        printf("%d \n", aux->item);
        //aux2 = aux;
        i = aux->item;
        aux = aux->proxVerticeAdj;
	}
    printf("acabou");
    aux2 = grafo->vertices[i];
    printf("Vertice %d eh adj a:\n", i);

	while(aux2 != NULL){
        printf("%d \n", aux2->item);
        aux2 = aux2->proxVerticeAdj;
	}

    //imprimeGrafo(grafo);

	return 0;

}
