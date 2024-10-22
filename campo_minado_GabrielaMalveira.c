//Trabalho Prático 1 - Gabriela Silva Malveira

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOMBA -1

// função para criar uma matriz `tam x tam`
int** fazMatriz(int tam) {
    int** m = malloc(tam * sizeof(int*));
    for (int i = 0; i < tam; i++) {
        m[i] = malloc(tam * sizeof(int));
    }
    return m;
}

// função para colocar bombas aleatoriamente na matriz
void colocaBombas(int** campo, int tam, int bombas) {
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            campo[i][j] = 0;
    srand(time(NULL));
    int b = 0;

    // coloca bombas até atingir o número desejado
    while (b < bombas) {
        int x = rand() % tam;
        int y = rand() % tam;
        if (campo[x][y] != BOMBA) {
            campo[x][y] = BOMBA;
            b++;
        }
    }
}

// função para contar as bombas ao redor das coordenadas
int contaBombas(int** campo, int tam, int x, int y) {
    int bombas = 0;
    
    // percorre a vizinhança 3x3
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i, ny = y + j;  // calcula as coordenadas vizinhas

            // ve se a célula vizinha está dentro dos limites e se tem uma bomba
            if (nx >= 0 && nx < tam && ny >= 0 && ny < tam && campo[nx][ny] == BOMBA) {
                bombas++;
            }
        }
    }
    return bombas;
}

// função para exibir o campo ao usuário
void mostraCampo(char** campo, int tam) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (campo[i][j] == '-') {
                printf("-1 ");
            } else {
                printf("%c  ", campo[i][j]);
            }
        }
        printf("\n");
    }
}

// libera a memória alocada para a matriz
void limpaMatriz(int** m, int tam) {
    for (int i = 0; i < tam; i++) {
        free(m[i]);
    }
    free(m);
}

// função principal do jogo
void jogo(int tam, int bombas) {
    int** campo = fazMatriz(tam);  // cria a matriz de bombas
    char** campoVisto = malloc(tam * sizeof(char*));
    for (int i = 0; i < tam; i++) {
        campoVisto[i] = malloc(tam * sizeof(char));
        for (int j = 0; j < tam; j++) {
            campoVisto[i][j] = 'x';  // inicializa todas as células como não reveladas
        }
    }

    colocaBombas(campo, tam, bombas);

    int jogando = 1;  // variável de controle para o loop do jogo
    while (jogando) {
        mostraCampo(campoVisto, tam);
        int x, y;
        printf("qual coordenada? (x,y)");
        scanf("%d,%d", &x, &y);
        x--; y--;

        // encontrou uma bomba
        if (campo[x][y] == BOMBA) {
            printf("game over\n");  // fim de jogo mostrando tudo
            for (int i = 0; i < tam; i++) {
                for (int j = 0; j < tam; j++) {
                    if (campo[i][j] == BOMBA) {
                        campoVisto[i][j] = '-'; 
                    } else {
                        int vizinhos = contaBombas(campo, tam, i, j);
                        campoVisto[i][j] = vizinhos + '0';
                    }
                }
            }
            mostraCampo(campoVisto, tam);  // exibe o campo final
            break;
        } else {
            int vizinhos = contaBombas(campo, tam, x, y);
            campoVisto[x][y] = vizinhos + '0';  // atualiza a célula com o número de bombas
        }

        // ve se o jogador ganhou
        int acertos = 0;
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < tam; j++) {
                if (campoVisto[i][j] != 'x') {
                    acertos++;  // conta células reveladas
                }
            }
        }
        // vencedor
        if (acertos == (tam * tam - bombas)) {
            printf("parabens, vc eh fera\n");
            mostraCampo(campoVisto, tam);
            break;
        }

        // ve se a jogada é válida
        if (x < 0 || x >= tam || y < 0 || y >= tam) {
            printf("invalida\n");
            continue;
        }

    }

    limpaMatriz(campo, tam);
    for (int i = 0; i < tam; i++) {
        free(campoVisto[i]);
    }
    free(campoVisto);
} //tudo liberado!

// função principal do programa
int main(void) {
    char opcao;
    int valido = 0;

    while (!valido) {
        printf("Escolhe ai:\nf para facil (10x10)\nm para medio (20x20)\nd para dificil (30x30)\n");
        
        scanf("%c", &opcao);
        switch(opcao) {
            case 'f':
                jogo(10, 10);
                valido = 1;
                break;
            case 'm':
                jogo(20, 40);
                valido = 1;
                break;
            case 'd':
                jogo(30, 90);
                valido = 1;
                break;
            default:
                printf("invalida\n");
        }
    }

    return 0;
}
