#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct que representa uma sala da mansão, agora com pista
typedef struct Sala {
    char nome[50];
    char pista[100]; // pista opcional
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Cria uma sala dinamicamente com nome e pista
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala) {
        strcpy(novaSala->nome, nome);
        if (pista)
            strcpy(novaSala->pista, pista);
        else
            novaSala->pista[0] = '\0';
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Insere uma pista na BST de pistas
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // pistas iguais não são inseridas novamente
    return raiz;
}

// Exibe as pistas em ordem alfabética (em ordem)
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Permite ao jogador explorar salas e coletar pistas
void explorarSalasComPistas(Sala* salaAtual, PistaNode** pistasBST) {
    char escolha;
    while (salaAtual) {
        printf("\nVocê está na sala: %s\n", salaAtual->nome);
        if (strlen(salaAtual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", salaAtual->pista);
            *pistasBST = inserirPista(*pistasBST, salaAtual->pista);
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

// Libera memória da BST de pistas
void liberarPistas(PistaNode* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Função principal: monta o mapa e inicia a exploração
int main() {
    // Montagem manual da árvore binária da mansão com pistas
    Sala* hall = criarSala("Hall de Entrada", "Pegada suspeita no tapete.");
    Sala* biblioteca = criarSala("Biblioteca", "Livro antigo fora do lugar.");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida.");
    Sala* sotao = criarSala("Sótão", "Janela aberta.");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra molhada.");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado no chão.");

    // Conectando as salas
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;
    biblioteca->direita = jardim;
    cozinha->direita = salaEstar;

    printf("Bem-vindo à Mansão Detective Quest!\n");

    // Inicializa BST de pistas
    PistaNode* pistasBST = NULL;
    explorarSalasComPistas(hall, &pistasBST);

    printf("\nPistas coletadas (em ordem alfabética):\n");
    exibirPistas(pistasBST);

    // Libera memória alocada
    liberarPistas(pistasBST);
    free(salaEstar);
    free(jardim);
    free(sotao);
    free(cozinha);
    free(biblioteca);
    free(hall);

    return 0;
}

