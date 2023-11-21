#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *arquivo;
    char nomeArquivo[100];

    // Solicita o nome do arquivo ao usuário
    printf("Digite o nome do arquivo .txt: ");
    scanf("%s", nomeArquivo);

    // Abre o arquivo em modo leitura
    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        // Se o arquivo não puder ser aberto, exibe uma mensagem de erro
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê a matriz do arquivo
    int valor;
    int linhas = 1;

    while (fscanf(arquivo, "%d", &valor) == 1) {
        // Supõe que os elementos da matriz são inteiros
        printf("%d ", valor);

        // Verifica se chegou ao final da linha
        char caractere;
        if (fscanf(arquivo, "%c", &caractere) == 1 && caractere == '\n') {
            // Se sim, incrementa o contador de linhas
            linhas++;
            printf("\n");
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Exibe o número de linhas
    printf("O arquivo tem %d linhas.\n", linhas);

    return 0;
}
