#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria uma sala dinamicamente com o nome fornecido
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Permite ao jogador explorar as salas da mansão
void explorarSalas(Sala* salaAtual) {
    char escolha;
    while (salaAtual) {
        printf("\nVocê está na sala: %s\n", salaAtual->nome);
        if (!salaAtual->esquerda && !salaAtual->direita) {
            printf("Esta sala não possui mais caminhos. Fim da exploração!\n");
            break;
        }
        printf("Escolha o caminho: [e] Esquerda | [d] Direita | [s] Sair: ");
        scanf(" %c", &escolha);
        if (escolha == 'e' && salaAtual->esquerda) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

// Função principal: monta o mapa e inicia a exploração
int main() {
    // Montagem manual da árvore binária da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");
    Sala* jardim = criarSala("Jardim");
    Sala* salaEstar = criarSala("Sala de Estar");

    // Conectando as salas
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;
    biblioteca->direita = jardim;
    cozinha->direita = salaEstar;

    printf("Bem-vindo à Mansão Detective Quest!\n");
    explorarSalas(hall);

    // Libera memória alocada
    free(salaEstar);
    free(jardim);
    free(sotao);
    free(cozinha);
    free(biblioteca);
    free(hall);

    return 0;
}

