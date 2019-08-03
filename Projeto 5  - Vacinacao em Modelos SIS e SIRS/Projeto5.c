/*************************************************************************/
/*PROJETO 5 - MODELAGEM COMPUTACIONAL                                    */
/*Prof. Dr. Marcos Goncalves Quiles                                      */
/*Propagacao de Informacao - Vacinacao nos Modelos SIS e SIRS            */
/*                                                                       */
/*Nomes: Ana Julia de Oliveira Bellini			  RA: 111774             */
/*	   Luiz Filipe Moraes Saldanha de Oliveira    RA: 112229             */
/*	   Willian Dihanster Gomes de Oliveira        RA: 112269             */
/*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define nVertices 1000 /*Numero de vertices*/
#define prob_infec 0.05 /*Prob. de um individuo saudavel ser infectado*/
#define prob_rec 0.35 /*Prob. de um individuo infectado se recuperar*/
#define prob_infec2 0.7 /*Prob. de um individuo recuperado ser infectado*/
#define prob_rec2 0.7 /*Prob. de um individuo recuperado, infectar de novo*/
#define infec_inicial 50 /*Pop. inicial de infectados*/
#define tempo 1000 /*Numero de ciclos*/
#define imunizacao 7 /*Dias que a pessoa fica imune à doença após a recuperação no modelo SIRS*/
#define qtdVacinados 27 /*Multiplo de grupos_ciclo*/ /*Numero de pessoas imunizadas por ciclo*/
#define grupos_ciclo 3 /*1<= grupos_ciclo <= qtdGrupos*/ /* Quantidade de grupos entre os quais a vacinaçao sera distribuida nos metodos 7, 8, 9 e 10*/
#define qtdGrupos 54 /*Quantidade de Grupos/Comunidades*/


struct listaAdj{
    int item;
    int infectado;
    int recuperado;
    int vacinado;
    int grupo;
	struct listaAdj *proxVerticeAdj;
};typedef struct listaAdj tipoListaAdj;

struct grafo{
    tipoListaAdj *vertices[nVertices];
};typedef struct grafo tipoGrafo;

struct Grupo{
    int i;
    int qtd;
    int vetor[1000];
    int completado;
}; typedef struct Grupo tipoGrupo;
tipoGrupo grupo[qtdGrupos];

tipoListaAdj* crialistaAdj(tipoGrafo *grafo, int item, int i){

	tipoListaAdj *listaAdj =  (tipoListaAdj*) malloc(sizeof(tipoListaAdj));
	listaAdj->item = item;
	listaAdj->proxVerticeAdj = NULL;

	return listaAdj;
}

tipoGrafo *criaGrafo(){

	int i;
	tipoGrafo *grafo = (tipoGrafo*) malloc(sizeof(tipoGrafo));
    tipoListaAdj *aux;

	for (i = 0; i < nVertices; i++){
        grafo->vertices[i] = NULL;
    }

	return grafo;
}

void iniciaVertices(tipoGrafo *grafo){
    int i, nRand;
    tipoListaAdj *aux;

    for (i = 0; i < nVertices; i++){
        aux = grafo->vertices[i];
        aux->infectado = 0;
        aux->recuperado = 0;
        aux->grupo = 0;
    }

    for(i = 0; i < infec_inicial; i++){
        nRand = rand() % nVertices;
        aux = grafo->vertices[nRand];
        aux->infectado = 1;
    }

}

void adicionaAresta(tipoGrafo *grafo, int verticeX, int verticeY){

    tipoListaAdj *vertice;
	vertice = crialistaAdj(grafo, verticeY, verticeX);
	vertice->proxVerticeAdj = grafo->vertices[verticeX];
	grafo->vertices[verticeX] = vertice;

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
        while(!feof(arquivo)){
            fscanf(arquivo, "%d", &v1);
            fscanf(arquivo, "%d", &v2);
            adicionaAresta(grafo, v1-1, v2-1);
            tam++;
        }
    }
    fclose(arquivo);
}

void leGrupo(){
    FILE *arquivo;
    arquivo = fopen("community.txt", "r");
    int tam = 0, v1, v2, i;

    for (i = 0; i < qtdGrupos; i++){
        grupo[i].qtd = 0;
        grupo[i].i = i;
        grupo[i].completado = 0;
    }

    if(arquivo == NULL){
        printf("Houve um erro ao abrir o arquivo!");
        exit(1);
    }
    else{
        while(!feof(arquivo)){
            fscanf(arquivo, "%d", &v1); /*Vertice*/
            fscanf(arquivo, "%d", &v2); /*Comunidade que ele pertence*/
            grupo[v2-1].vetor[grupo[v2-1].qtd] = v1-1; /*coloca na posicao atual do vetor*/
            grupo[v2-1].qtd++; /*atualiza contador*/
            tam++;
        }
    }
    fclose(arquivo);
}

int menor[qtdGrupos], maior[qtdGrupos];
void menorMaiorGrupo(){
    int i, j;
    tipoGrupo aux;
    tipoGrupo auxGrupo[qtdGrupos];

    for (i = 0; i < qtdGrupos; i++){
        auxGrupo[i].i = grupo[i].i;
        auxGrupo[i].qtd = grupo[i].qtd;
    }

    for (i = 0; i < qtdGrupos; i++){
        for (j = 0; j < qtdGrupos; j++) {
            if (auxGrupo[j].qtd > auxGrupo[j+1].qtd){
                aux = auxGrupo[j];
                auxGrupo[j] = auxGrupo[j+1];
                auxGrupo[j+1] = aux;
            }
        }
    }

    for (i = 0; i < qtdGrupos; i++){
        menor[i] = auxGrupo[i].i;
        maior[i] = auxGrupo[qtdGrupos - i - 1].i;
    }

}

int calculaInfectados(tipoGrafo *grafo){
    int cont = 0, i;
    tipoListaAdj *aux;

    for (i = 0; i < nVertices; i++){
        aux = grafo->vertices[i];
        if (aux->infectado)
            cont++;
    }

    return cont;

}

void lacoPrincipal1(tipoGrafo *grafo){
    int i, j, cont = 0, contInfectados = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
	srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if (!aux->infectado){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                        aux->infectado = 1;
            }
            else {
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec)
                    aux->infectado = 0;
            }
        }
    }

    fclose(plotPropag);

}

void lacoPrincipal2(tipoGrafo *grafo){
    int i, j, cont = 0, contInfectados = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
	srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    contInfectados = calculaInfectados(grafo);
    fprintf(plotPropag, "%d\n", contInfectados);
    printf("%d\n", contInfectados);
    for (i = 0; i < tempo; i++) {
        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if ((!aux->infectado) && (!aux->recuperado)){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                        aux->infectado = 1;
            }
            else if (aux->recuperado){
                    aux->recuperado++;
                    if (aux->recuperado >= imunizacao)
                        aux->recuperado = 0;

            } else if (aux->infectado){
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec){
                    aux->infectado = 0;
                    aux->recuperado = 1;
                }
            }
        }
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);
    }

    fclose(plotPropag);

}

void lacoPrincipal3(tipoGrafo *grafo) {
    int i, j, k, g, r, cont = 0, contInfectados = 0, contVacinados = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        if (g <= qtdGrupos && (i % 7) == 0) {
            for (k = 0; k < qtdVacinados; k++){
                grafo->vertices[grupo[maior[g]].vetor[contVacinados]]->vacinado = 1;
                grafo->vertices[grupo[maior[g]].vetor[contVacinados]]->infectado = 0;
                contVacinados++;

                if (contVacinados == grupo[maior[g]].qtd){
                    contVacinados = 0;
                    g++;
                    break;
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if (!aux->infectado && !aux->vacinado){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (prob_infec*cont))
                    aux->infectado = 1;
            }
            else {
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec){
                    aux->infectado = 0;
                }
            }
        }
    }

    fclose(plotPropag);
}

void lacoPrincipal4(tipoGrafo *grafo){
    int i, j, k, g, cont = 0, contInfectados = 0, contVacinados = 0, cont2 = 0, cont3 = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        if (g <= qtdGrupos && (i % 7) == 0) {
            for (k = 0; k < qtdVacinados; k++){
                grafo->vertices[grupo[menor[g]].vetor[contVacinados]]->vacinado = 1;
                grafo->vertices[grupo[menor[g]].vetor[contVacinados]]->infectado = 0;
                contVacinados++;

                if (contVacinados == grupo[menor[g]].qtd){
                    contVacinados = 0;
                    g++;
                    break;
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if (!aux->infectado && !aux->vacinado){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                    aux->infectado = 1;
            }
            else {
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec){
                    aux->infectado = 0;
                }
            }
        }
    }

    fclose(plotPropag);

}

void lacoPrincipal5(tipoGrafo *grafo){
    int i, j, k, g, cont = 0, contInfectados = 0, contVacinados = 0, cont2 = 0, cont3 = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        if (g <= qtdGrupos && (i % 7) == 0) {
            for (k = 0; k < qtdVacinados; k++){
                grafo->vertices[grupo[maior[g]].vetor[contVacinados]]->vacinado = 1;
                grafo->vertices[grupo[maior[g]].vetor[contVacinados]]->infectado = 0;
                contVacinados++;

                if (contVacinados == grupo[maior[g]].qtd){
                    contVacinados = 0;
                    g++;
                    break;
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if ((!aux->infectado) && (!aux->recuperado) && (!aux->vacinado)){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                    aux->infectado = 1;
            }
            else if (aux->recuperado){
                aux->recuperado++;
                if (aux->recuperado >= imunizacao)
                    aux->recuperado = 0;
            }
            else if (aux->infectado){
                    nRand = (float) rand()/RAND_MAX;
                    if (nRand <= prob_rec){
                        aux->infectado = 0;
                    }
            }
        }
    }

    fclose(plotPropag);

}

void lacoPrincipal6(tipoGrafo *grafo){
    int i, j, k, g, cont = 0, contInfectados = 0, contVacinados = 0, cont2 = 0, cont3 = 0;
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        if (g <= qtdGrupos && (i % 7) == 0) {
            for (k = 0; k < qtdVacinados; k++){
                grafo->vertices[grupo[menor[g]].vetor[contVacinados]]->vacinado = 1;
                grafo->vertices[grupo[menor[g]].vetor[contVacinados]]->infectado = 0;
                contVacinados++;

                if (contVacinados == grupo[menor[g]].qtd){
                    contVacinados = 0;
                    g++;
                    break;
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if ((!aux->infectado) && (!aux->recuperado) && (!aux->vacinado)){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                    aux->infectado = 1;
            }
            else if (aux->recuperado){
                    aux->recuperado++;
                    if (aux->recuperado >= imunizacao)
                        aux->recuperado = 0;
            }
            else if (aux->infectado){
                    nRand = (float) rand()/RAND_MAX;
                    if (nRand <= prob_rec){
                        aux->infectado = 0;
                        aux->recuperado = 1;
                    }
            }
        }
    }

    fclose(plotPropag);

}

void lacoPrincipal7(tipoGrafo *grafo) {
    int i, j, k, l, m, g, r, sub = 0, cont = 0, contInfectados = 0, qtdVacinados2 = (qtdVacinados/grupos_ciclo), contVacinados[grupos_ciclo], vetor_vacina[grupos_ciclo];
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    for(i = 0; i < grupos_ciclo; i++){
        vetor_vacina[i] = i;
        contVacinados[i] = 0;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        for(j = 0; j < grupos_ciclo; j++){
            g = vetor_vacina[j];
            if (g < qtdGrupos && (i % 7) == 0) {
                for (k = 0; k < qtdVacinados2; k++){
                    grafo->vertices[grupo[maior[g]].vetor[contVacinados[j]]]->vacinado = 1;
                    grafo->vertices[grupo[maior[g]].vetor[contVacinados[j]]]->infectado = 0;
                    contVacinados[j]++;

                    if (contVacinados[j] == grupo[maior[g]].qtd){
                        contVacinados[j] = 0;
                        if(g+grupos_ciclo < qtdGrupos){
                            vetor_vacina[j] = g+grupos_ciclo;
                        }else {
                            vetor_vacina[j] = qtdGrupos + 1;
                        }
                        break;
                    }
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if (!aux->infectado && !aux->vacinado){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (prob_infec*cont))
                    aux->infectado = 1;
            }
            else {
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec){
                    aux->infectado = 0;
                }
            }
        }
    }

    fclose(plotPropag);
}

void lacoPrincipal8(tipoGrafo *grafo) {
    int i, j, k, l, m, g, r, sub = 0, cont = 0, contInfectados = 0, qtdVacinados2 = (qtdVacinados/grupos_ciclo), contVacinados[grupos_ciclo], vetor_vacina[grupos_ciclo];
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }

    for(i = 0; i < grupos_ciclo; i++){
        vetor_vacina[i] = i;
        contVacinados[i] = 0;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        for(j = 0; j < grupos_ciclo; j++){
            g = vetor_vacina[j];
            if (g < qtdGrupos && (i % 7) == 0) {
                for (k = 0; k < qtdVacinados2; k++){
                    grafo->vertices[grupo[menor[g]].vetor[contVacinados[j]]]->vacinado = 1;
                    grafo->vertices[grupo[menor[g]].vetor[contVacinados[j]]]->infectado = 0;
                    contVacinados[j]++;

                    if (contVacinados[j] == grupo[menor[g]].qtd){
                        contVacinados[j] = 0;
                        if(g+grupos_ciclo < qtdGrupos){
                            vetor_vacina[j] = g+grupos_ciclo;
                        }else {
                            vetor_vacina[j] = qtdGrupos + 1;
                        }
                        break;
                    }
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if (!aux->infectado && !aux->vacinado){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (prob_infec*cont))
                    aux->infectado = 1;
            }
            else {
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= prob_rec){
                    aux->infectado = 0;
                }
            }
        }
    }

    fclose(plotPropag);
}

void lacoPrincipal9(tipoGrafo *grafo){
    int i, j, k, l, m, g, r, sub = 0, cont = 0, contInfectados = 0, qtdVacinados2 = (qtdVacinados/grupos_ciclo), contVacinados[grupos_ciclo], vetor_vacina[grupos_ciclo];
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }
    for(i = 0; i < grupos_ciclo; i++){
        vetor_vacina[i] = i;
        contVacinados[i] = 0;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        for(j = 0; j < grupos_ciclo; j++){
            g = vetor_vacina[j];
            if (g < qtdGrupos && (i % 7) == 0) {
                for (k = 0; k < qtdVacinados2; k++){
                    grafo->vertices[grupo[maior[g]].vetor[contVacinados[j]]]->vacinado = 1;
                    grafo->vertices[grupo[maior[g]].vetor[contVacinados[j]]]->infectado = 0;
                    contVacinados[j]++;

                    if (contVacinados[j] == grupo[maior[g]].qtd){
                        contVacinados[j] = 0;
                        if(g+grupos_ciclo < qtdGrupos){
                            vetor_vacina[j] = g+grupos_ciclo;
                        }else {
                            vetor_vacina[j] = qtdGrupos + 1;
                        }
                        break;
                    }
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if ((!aux->infectado) && (!aux->recuperado) && (!aux->vacinado)){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                    aux->infectado = 1;
            }
            else if (aux->recuperado){
                    aux->recuperado++;
                    if (aux->recuperado >= imunizacao)
                        aux->recuperado = 0;
            }
            else if (aux->infectado){
                    nRand = (float) rand()/RAND_MAX;
                    if (nRand <= prob_rec){
                        aux->infectado = 0;
                        aux->recuperado = 1;
                    }
            }
        }
    }

    fclose(plotPropag);

}

void lacoPrincipal10(tipoGrafo *grafo){
    int i, j, k, l, m, g, r, sub = 0, cont = 0, contInfectados = 0, qtdVacinados2 = (qtdVacinados/grupos_ciclo), contVacinados[grupos_ciclo], vetor_vacina[grupos_ciclo];
    float nRand;
    tipoListaAdj *aux, *aux2;
    FILE *plotPropag;
    srand(time(NULL));
    plotPropag = fopen("plotPropag.txt", "w");
    if (plotPropag == NULL){
        printf("Erro ao abrir arquivo");
        return;
    }
    for(i = 0; i < grupos_ciclo; i++){
        vetor_vacina[i] = i;
        contVacinados[i] = 0;
    }

    g = 0;
    for (i = 0; i < tempo; i++) {
        contInfectados = calculaInfectados(grafo);
        fprintf(plotPropag, "%d\n", contInfectados);
        printf("%d\n", contInfectados);

        for(j = 0; j < grupos_ciclo; j++){
            g = vetor_vacina[j];
            if (g < qtdGrupos && (i % 7) == 0) {
                for (k = 0; k < qtdVacinados2; k++){
                    grafo->vertices[grupo[menor[g]].vetor[contVacinados[j]]]->vacinado = 1;
                    grafo->vertices[grupo[menor[g]].vetor[contVacinados[j]]]->infectado = 0;
                    contVacinados[j]++;

                    if (contVacinados[j] == grupo[menor[g]].qtd){
                        contVacinados[j] = 0;
                        if(g+grupos_ciclo < qtdGrupos){
                            vetor_vacina[j] = g+grupos_ciclo;
                        }else {
                            vetor_vacina[j] = qtdGrupos + 1;
                        }
                        break;
                    }
                }
            }
        }

        for (j = 0; j < nVertices; j++) {
            cont = 0;
            aux = grafo->vertices[j];
            if ((!aux->infectado) && (!aux->recuperado) && (!aux->vacinado)){
                while (aux != NULL){
                    aux2 = grafo->vertices[aux->item];
                    if (aux2->infectado)
                        cont++;
                    aux = aux->proxVerticeAdj;
                }
                aux = grafo->vertices[j];
                nRand = (float) rand()/RAND_MAX;
                if (nRand <= (1-(prob_infec*cont)))
                    aux->infectado = 1;
            }
            else if (aux->recuperado){
                    aux->recuperado++;
                    if (aux->recuperado >= imunizacao)
                        aux->recuperado = 0;
            }
            else if (aux->infectado){
                    nRand = (float) rand()/RAND_MAX;
                    if (nRand <= prob_rec){
                        aux->infectado = 0;
                        aux->recuperado = 1;
                    }
            }
        }
    }

    fclose(plotPropag);

}



int main(){
	tipoGrafo *grafo = criaGrafo();
	leGrafoModular(grafo);
    iniciaVertices(grafo);
    leGrupo();
    menorMaiorGrupo();

    //lacoPrincipal1(grafo);
    //lacoPrincipal2(grafo);
    //lacoPrincipal3(grafo);
    //lacoPrincipal4(grafo);
    //lacoPrincipal5(grafo);
    //lacoPrincipal6(grafo);
    lacoPrincipal7(grafo);
    //lacoPrincipal8(grafo);
    //lacoPrincipal9(grafo);
    //lacoPrincipal10(grafo);

    printf("FIM");
	return 0;

}


