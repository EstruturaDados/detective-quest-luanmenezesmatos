#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct para sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    char suspeito[50]; // suspeito associado à pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Nó da lista encadeada para hash (colisão)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// Tabela hash de pistas para suspeitos
#define HASH_SIZE 13
typedef struct {
    HashNode* buckets[HASH_SIZE];
} HashTable;

// Função de hash simples: soma dos valores ASCII da pista
int hashFunc(const char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % HASH_SIZE;
}

// Cria uma sala dinamicamente com nome, pista e suspeito
Sala* criarSala(const char* nome, const char* pista, const char* suspeito) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala) {
        strcpy(novaSala->nome, nome);
        if (pista)
            strcpy(novaSala->pista, pista);
        else
            novaSala->pista[0] = '\0';
        if (suspeito)
            strcpy(novaSala->suspeito, suspeito);
        else
            novaSala->suspeito[0] = '\0';
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
    return raiz;
}

// Exibe as pistas em ordem alfabética
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Inicializa a tabela hash
void inicializarHash(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++)
        ht->buckets[i] = NULL;
}

// Insere associação pista/suspeito na tabela hash
void inserirNaHash(HashTable* ht, const char* pista, const char* suspeito) {
    int idx = hashFunc(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = ht->buckets[idx];
    ht->buckets[idx] = novo;
}

// Busca suspeito correspondente à pista
const char* encontrarSuspeito(HashTable* ht, const char* pista) {
    int idx = hashFunc(pista);
    HashNode* atual = ht->buckets[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Conta quantas pistas apontam para o suspeito acusado
int contarPistasPorSuspeito(HashTable* ht, const char* suspeito) {
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = ht->buckets[i];
        while (atual) {
            if (strcmp(atual->suspeito, suspeito) == 0)
                count++;
            atual = atual->prox;
        }
    }
    return count;
}

// Permite ao jogador explorar salas, coletar pistas e associar suspeitos
void explorarSalas(Sala* salaAtual, PistaNode** pistasBST, HashTable* ht) {
    char escolha;
    while (salaAtual) {
        printf("\nVocê está na sala: %s\n", salaAtual->nome);
        if (strlen(salaAtual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", salaAtual->pista);
            printf("Suspeito relacionado: %s\n", salaAtual->suspeito);
            *pistasBST = inserirPista(*pistasBST, salaAtual->pista);
            inserirNaHash(ht, salaAtual->pista, salaAtual->suspeito);
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

// Libera memória da tabela hash
void liberarHash(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = ht->buckets[i];
        while (atual) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// Fase de julgamento final
void verificarSuspeitoFinal(HashTable* ht) {
    char acusacao[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusacao);
    int qtd = contarPistasPorSuspeito(ht, acusacao);
    if (qtd >= 2) {
        printf("Acusação aceita! %d pistas apontam para %s. Você solucionou o mistério!\n", qtd, acusacao);
    } else if (qtd == 1) {
        printf("Apenas uma pista aponta para %s. Evidências insuficientes!\n", acusacao);
    } else {
        printf("Nenhuma pista aponta para %s. Acusação rejeitada!\n", acusacao);
    }
}

// Função principal: monta o mapa e inicia a exploração
int main() {
    // Montagem manual da árvore binária da mansão com pistas e suspeitos
    Sala* hall = criarSala("Hall de Entrada", "Pegada suspeita no tapete.", "Sr. Green");
    Sala* biblioteca = criarSala("Biblioteca", "Livro antigo fora do lugar.", "Sra. White");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida.", "Srta. Scarlet");
    Sala* sotao = criarSala("Sótão", "Janela aberta.", "Sr. Green");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra molhada.", "Srta. Scarlet");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado no chão.", "Sra. White");

    // Conectando as salas
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;
    biblioteca->direita = jardim;
    cozinha->direita = salaEstar;

    printf("Bem-vindo à Mansão Detective Quest!\n");

    // Inicializa BST de pistas e tabela hash
    PistaNode* pistasBST = NULL;
    HashTable hashTable;
    inicializarHash(&hashTable);

    explorarSalas(hall, &pistasBST, &hashTable);

    printf("\nPistas coletadas (em ordem alfabética):\n");
    exibirPistas(pistasBST);

    verificarSuspeitoFinal(&hashTable);

    // Libera memória alocada
    liberarHash(&hashTable);
    liberarPistas(pistasBST);
    free(salaEstar);
    free(jardim);
    free(sotao);
    free(cozinha);
    free(biblioteca);
    free(hall);

    return 0;
}

