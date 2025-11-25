#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "avl_tree.h"
#include "binary_search.h"

static Avl *tree = NULL;
static Item products[2000];
static const char *CSV_PATH = "DIM_Shopping.csv";

void CarregaCsv(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Nao consegui abrir %s\n", path);
        return;
    }

    char line[1024];
    if (!fgets(line, sizeof(line), fp)){  
        fclose(fp);
        return; 
    }

    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *nl = strchr(line, '\n'); 
        if (nl) *nl = '\0';

        char *p = strtok(line, ";");
        int j = 0;
        while (p) {
            switch (j) {
                case 0: 
                    products[count].id = atoi(p);
                    break;
                case 1: 
                    strncpy(products[count].item_id, p, sizeof(products[count].item_id)-1); 
                    products[count].item_id[sizeof(products[count].item_id)-1] = '\0'; 
                    break;
                case 2: 
                    strncpy(products[count].name, p, sizeof(products[count].name)-1); 
                    products[count].name[sizeof(products[count].name)-1] = '\0'; 
                    break;
                case 3: 
                    products[count].quantity = atoi(p);
                    break;
                case 4: 
                    products[count].price = atoi(p); 
                    break;
            }
            p = strtok(NULL, ";"); j++;
        }
        
        tree = InsertAvl(tree, products[count]);
        count++;
    }

    fclose(fp);
    printf("Carregado %d registros do CSV.\n", count);
}

void printItem(const Item *it) {
    if (!it) return;
    printf("ID: %d\nProduto: %s\nQuantidade: %d\nPreco: %d\n", it->id, it->name, it->quantity, it->price);
}

void BuscaId() {
    char buf[64];
    
    printf("Digite o ID: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;

    int id = atoi(buf);

    // Search using AVL Tree
    Avl *node = SearchAvl(tree, id);

    // Search using array and binary search 
    Item *element = BinarySearch(products, sizeof(products)/sizeof(products[0]), id);

    if (!node) 
        printf("Nao encontrado.\n");
    else 
        printItem(&node->Info);
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
    for (int i = start; i < start + 100; ++i) {

        if(i > 2000) {
            printf("\nNão há mais IDs para serem buscados.\n");
            break;
        }

        // Search using AVL Tree
        Avl *node = SearchAvl(tree, (int)i);
        
        // Search using array and binary search
        Item *element = BinarySearch(products, sizeof(products)/sizeof(products[0]), (int)i);
        
        if (node) 
            printf("%d - %s\n", i, node->Info.name);
        else 
            printf("%d - Nao encontrado\n", i);
    }
}

void BuscaRandom(int n) {
    if (tree == NULL){
        printf("Arvore vazia. Carregue o CSV primeiro.\n");
        return;
    }

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; ++i) {
        // Generate key
        int key = (rand() % 2000) + 1;

        // Search using AVL Tree
        Avl *node = SearchAvl(tree, key);

        // Search using array and binary search
        Item *element = BinarySearch(products, sizeof(products)/sizeof(products[0]), key);        

        if (node) 
            printf("%d - %s\n", key, node->Info.name);
        else 
            printf("%d - Nao encontrado\n", key);
    }
}

void menu_loop() {
    char opt[8];
    for (;;) {
        printf("\n=== Menu ===\n");
        printf("1) Carregar CSV e construir AVL\n");
        printf("2) Buscar por ID\n");
        printf("3) Busca sequencial (100 itens)\n");
        printf("4) Busca aleatoria\n");
        printf("5) Sair\n");
        printf("Escolha: ");

        if (!fgets(opt, sizeof(opt), stdin)) break;
        
        int o = atoi(opt);
        switch (o) {
            case 1: CarregaCsv(CSV_PATH); break;
            case 2: BuscaId(); break;
            case 3: BuscaSequencial(); break;
            case 4: BuscaRandom(100); break;
            case 5: return;
            default: printf("Opcao invalida.\n"); break;
        }
    }
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    menu_loop();
    return 0;
}
