#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define V 4

typedef struct {
    int origem, destino, peso;
} Aresta;

Aresta* arestasNaoMST(Aresta* listaTodasArestas, int tamanhoTodasArestas, Aresta* listaArestasMST, int tamanhoArestasMST, int* tamanhoArestasNaoMST) {
    // Inicializa um ponteiro para armazenar a lista de arestas que não estão na MST
    Aresta* listaArestasNaoMST = NULL;

    // Calcula o tamanho máximo possível para a lista de arestas que não estão na MST
    int tamanhoMaxArestasNaoMST = tamanhoTodasArestas - tamanhoArestasMST;

    // Aloca memória para a lista de arestas que não estão na MST
    listaArestasNaoMST = (Aresta*)calloc(tamanhoMaxArestasNaoMST, sizeof(Aresta));

    if (listaArestasNaoMST == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    int i, j;
    int contador = 0;

    // Percorre todas as arestas do grafo
    for (i = 0; i < tamanhoTodasArestas; i++) {
        int estaNaMST = 0;

        // Verifica se a aresta está na MST
        for (j = 0; j < tamanhoArestasMST; j++) {

            if(listaTodasArestas[i].origem == listaArestasMST[j].destino && listaArestasMST[j].peso == listaTodasArestas[i].peso){
                estaNaMST = 1;
                break;  
            }
            

            if (listaTodasArestas[i].origem == listaArestasMST[j].origem &&
                listaTodasArestas[i].destino == listaArestasMST[j].destino &&
                listaTodasArestas[i].peso == listaArestasMST[j].peso
                ) {
                estaNaMST = 1;
                break;
            }
        }

        // Se a aresta não está na MST, adiciona à lista de arestas que não estão na MST
        if (!estaNaMST && 
            listaTodasArestas[i].peso != 0 &&
            listaArestasMST[j].peso != 0) {

            listaArestasNaoMST[contador].origem = listaTodasArestas[i].origem;
            listaArestasNaoMST[contador].destino = listaTodasArestas[i].destino;
            listaArestasNaoMST[contador].peso = listaTodasArestas[i].peso;
            contador++;
        }
    }

    // Atualiza o tamanho real da lista de arestas que não estão na MST
    *tamanhoArestasNaoMST = contador;

    return listaArestasNaoMST;
}

void printArestaLista(Aresta *listaArestas, int tamanho, const char *titulo) {
    printf("%s\n", titulo);
    printf("Edge \tWeight\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d - %d \t%d \n", listaArestas[i].origem, listaArestas[i].destino, listaArestas[i].peso);
    }
}

int minKey(int key[], int mstSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (mstSet[v] == 0 && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

void printMST(int parent[], int graph[V][V], Aresta *listaArestasMST) {
    int arestaCount = 0;

    for (int i = 1; i < V; i++) {
        listaArestasMST[arestaCount].origem = parent[i];
        listaArestasMST[arestaCount].destino = i;
        listaArestasMST[arestaCount].peso = graph[i][parent[i]];
        arestaCount++;
    }

    printArestaLista(listaArestasMST, arestaCount, "Arestas na Arvore Geradora Minima:");
}

void primMST(int graph[V][V], Aresta *listaTodasArestas, Aresta *listaArestasMST) {
    int parent[V];
    int key[V];
    int mstSet[V];

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printMST(parent, graph, listaArestasMST);

    // Preenche a listaTodasArestas com todas as arestas do grafo
    int arestaCount = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0) {
                listaTodasArestas[arestaCount].origem = i;
                listaTodasArestas[arestaCount].destino = j;
                listaTodasArestas[arestaCount].peso = graph[i][j];
                arestaCount++;
            }
        }
    }

    printArestaLista(listaTodasArestas, arestaCount, "\nTodas as Arestas do Grafo:");
}

int main() {
    int cont = 0, raiz;
    char nomeArquivo[100];  // Ajuste o tamanho conforme necessário
    int continuar = 1;

    while (continuar) {
        printf("Digite o nome do arquivo .txt: ");
        scanf("%s", nomeArquivo);

        FILE *file = fopen(nomeArquivo, "r");

        if (file == NULL) {
            perror("Erro ao abrir o arquivo");
            return -1;
        }

        int graph[V][V];

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                fscanf(file, "%d", &graph[i][j]);
                cont++;
            }
        }

        raiz = sqrt(cont);

        fclose(file);

    // Aloca espaço para as listas de arestas
    Aresta *listaTodasArestas = (Aresta *)calloc(V * (V - 1) / 2, sizeof(Aresta));
    Aresta *listaArestasMST = (Aresta *)calloc(V - 1, sizeof(Aresta));

    if (listaTodasArestas == NULL || listaArestasMST == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    // Chama a função para imprimir as arestas da MST e todas as arestas do grafo
    primMST(graph, listaTodasArestas, listaArestasMST);

    int tamanhoArestasNaoMST;
    Aresta* listaArestasNaoMST = arestasNaoMST(listaTodasArestas, V * (V - 1) / 2, listaArestasMST, V - 1, &tamanhoArestasNaoMST);

    printArestaLista(listaArestasNaoMST, tamanhoArestasNaoMST, "\nArestas que nao estao na Arvore Geradora Minima:");

    // Libera a memória alocada
    free(listaTodasArestas);
    free(listaArestasMST);
    free(listaArestasNaoMST);

        printf("Deseja continuar? (1 para sim, 0 para sair): ");
        scanf("%d", &continuar);
    }

    return 0;
}
