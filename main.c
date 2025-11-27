#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "avl_tree.h"
#include "binary_search.h"

static Avl *tree = NULL;
static const char *CSV_PATH = "DIM_Shopping.csv";

#define MAX_PRODUCTS 2500
static Item products[MAX_PRODUCTS];
static int products_count = 0;


void CarregaCsv(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Nao consegui abrir %s\n", path);
        return;
    }

    char line[1024];
    products_count = 0;
    if (!fgets(line, sizeof(line), fp)){  
         fclose(fp);
        return; 
        }

    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *nl = strchr(line, '\n'); if (nl) *nl = '\0';

        Item it;
        it.id = 0; it.item_id[0] = '\0'; it.product[0] = '\0'; it.quantity = 0; it.price = 0;

        char *p = strtok(line, ";");
        int j = 0;
        while (p) {
            switch (j) {
                case 0: it.id = atoi(p); break;
                case 1: strncpy(it.item_id, p, sizeof(it.item_id)-1); it.item_id[sizeof(it.item_id)-1] = '\0'; break;
                case 2: strncpy(it.product, p, sizeof(it.product)-1); it.product[sizeof(it.product)-1] = '\0'; break;
                case 3: it.quantity = atoi(p); break;
                case 4: it.price = atoi(p); break;
            }
            p = strtok(NULL, ";"); j++;
        }
        if(products_count < MAX_PRODUCTS) {
            products[products_count] = it;
            products_count++;
        }
        tree = InsertAvl(tree, it);
        count++;
    }
    fclose(fp);
    printf("Carregado %d registros do CSV.\n", count);
    printf("Produtos armazenados no vetor para busca binaria: %d\n", products_count);
}

void printItem(const Item *it) {
    if (!it) return;
    printf("ID: %d\nProduto: %s\nQuantidade: %d\nPreco: %d\n", it->id, it->product, it->quantity, it->price);
}

void BuscaIdBinaria(){
     if (products_count == 0) {
        printf("Vetor vazio. Carregue o CSV primeiro.\n");
        return;
    }
    char buf[64];
    printf("Digite o ID: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int id = atoi(buf);
    int accesses = 0;
    clock_t t0 = clock();
    Item *element = BinarySearchWithCount(products, products_count, id, &accesses);
    clock_t t1 = clock();
    double elapsed_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    if (element) {
        printf("Vetor: ID %d -> %s\n", element->id, element->product);
    } else {
        printf("Vetor: ID %d -> Nao encontrado\n", id);
    }
    printf("Tempo gasto (busca binaria): %.3f ms\n", elapsed_ms);
    printf("Acessos (vetor): %d\n", accesses);
}

void BuscaId() {
     if (tree == NULL) {
        printf("Arvore vazia. Carregue o CSV primeiro.\n");
        return;
    }

    char buf[64];
    
    printf("Digite o ID: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    
    int id = atoi(buf);
    int accesses = 0;
    
    clock_t t0 = clock();
    Avl *node = SearchAvlWithCount(tree, id, &accesses);
    clock_t t1 = clock();
    
    double elapsed_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    if (!node) printf("AVL: Nao encontrado.\n");
    else {
        printf("AVL: ");
        printItem(&node->Info);
    }
    printf("Tempo gasto (AVL): %.3f ms\n", elapsed_ms);
    printf("Acessos (AVL): %d\n", accesses);
}

void BuscaSequencialBinaria(){
    if (products_count == 0) {
        printf("Vetor vazio. Carregue o CSV primeiro.\n");
        return;
    }
    char buf[64];
    
    printf("Digite o ID inicial: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    
    int start = atoi(buf);
    if (start < 1) start = 1;

    printf("Buscando 100 itens sequencialmente a partir do ID %d...\n", start);
    
    clock_t t0 = clock();
    int found = 0;
    long long total_accesses = 0;
    for (int i = start; i < start + 100; ++i) {
        int accesses = 0;
        Item *element = BinarySearchWithCount(products, products_count, i, &accesses);
        total_accesses += accesses;
        if (element) { printf("%d - %s\n", i, element->product); found++; }
        else printf("%d - Nao encontrado\n", i);
    }
    clock_t t1 = clock();
    
    double total_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("Resultado: %d encontrados de 100. Tempo total (busca binaria): %.3f ms (%.3f ms por busca)\n",
           found, total_ms, total_ms / 100.0);
    printf("Acessos totais (vetor): %lld", total_accesses);
}

void BuscaSequencial() {
    if (tree == NULL) {
        printf("Arvore vazia. Carregue o CSV primeiro.\n");
        return;
    }

    char buf[64];
    printf("Digite o ID inicial: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int start = atoi(buf);
    if (start < 1) start = 1;

    printf("Buscando 100 itens sequencialmente a partir do ID %d...\n", start);
    
    clock_t t0 = clock();
    int found = 0;
    long long total_accesses = 0;
    for (int i = start; i < start + 100; ++i) {
        int accesses = 0;
        Avl *node = SearchAvlWithCount(tree, (int)i, &accesses);
        total_accesses += accesses;
        if (node) { printf("%d - %s\n", i, node->Info.product); found++; }
        else printf("%d - Nao encontrado\n", i);
    }
    clock_t t1 = clock();
    
    double total_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("Resultado: %d encontrados de 100. Tempo total (AVL): %.3f ms (%.3f ms por busca)\n",
           found, total_ms, total_ms / 100.0);
    printf("Acessos totais (AVL): %lld", total_accesses);
}

void BuscaRandomBinaria(int n) {
    if (products_count == 0){
        printf("Vetor vazio. Carregue o CSV primeiro.\n");
        return;
    }
    srand((unsigned)time(NULL));
    
    clock_t t0 = clock();
    int found = 0;
    long long total_accesses = 0;
    for (int i = 0; i < n; ++i) {
        int key = (rand() % 2000) + 1;
        int accesses = 0;
        Item *element = BinarySearchWithCount(products, products_count, key, &accesses);
        total_accesses += accesses;
        if (element) { printf("%d - %s\n", key, element->product); found++; }
        else printf("%d - Nao encontrado\n", key);
    }
    clock_t t1 = clock();
    
    double total_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
    
    printf("Resultado: %d encontrados de %d. Tempo total (busca binaria): %.3f ms (%.3f ms por busca)\n",
           found, n, total_ms, total_ms / (double)n);
    printf("Acessos totais (vetor): %lld", total_accesses);
}

void BuscaRandom(int n) {
    if (tree == NULL){
    printf("Arvore vazia. Carregue o CSV primeiro.\n");
    return;
     }
    srand((unsigned)time(NULL));
    
    clock_t t0 = clock();
    int found = 0;
    long long total_accesses = 0;
    for (int i = 0; i < n; ++i) {
        int key = (rand() % 2000) + 1;
        int accesses = 0;
        Avl *node = SearchAvlWithCount(tree, key, &accesses);
        total_accesses += accesses;
        if (node) { printf("%d - %s\n", key, node->Info.product); found++; }
        else printf("%d - Nao encontrado\n", key);
    }
    clock_t t1 = clock();
    
    double total_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("Resultado: %d encontrados de %d. Tempo total (AVL): %.3f ms (%.3f ms por busca)\n",
           found, n, total_ms, total_ms / (double)n);
    printf("Acessos totais (AVL): %lld", total_accesses);
}   

void menu_loop() {
    char opt[8];
    for (;;) {
        printf("\n=== Menu ===\n");
        printf("1) Carregar CSV e construir AVL\n");
        printf("2) Buscar por ID (AVL)\n");
        printf("3) Buscar por ID (Binaria)\n");
        printf("4) Busca sequencial AVL (100 itens)\n");
        printf("5) Busca sequencial Binaria (100 itens)\n");
        printf("6) Busca aleatoria (AVL)\n");
        printf("7) Busca aleatoria (Binaria)\n");
        printf("8) Sair\n");
        printf("Escolha: ");
        if (!fgets(opt, sizeof(opt), stdin)) break;
        int o = atoi(opt);
        switch (o) {
            case 1: CarregaCsv(CSV_PATH); break;
            case 2: BuscaId(); break;
            case 3: BuscaIdBinaria(); break;
            case 4: BuscaSequencial(); break;
            case 5: BuscaSequencialBinaria(); break;
            case 6: BuscaRandom(100); break;
            case 7: BuscaRandomBinaria(100); break;
            case 8: return;
            default: printf("Opcao invalida.\n"); break;
        }
    }
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    menu_loop();
    return 0;
}
