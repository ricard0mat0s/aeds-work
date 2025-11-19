#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Item{
    int id;
    char item_id[7];
    char product[50];
    int quantity;
    int price;
}Item;

typedef struct avl{
    Item Info;
    int Altura;
    struct avl *esq, *dir;
}Avl;

int alturaAvl(Avl *a){
    if(a == NULL) return -1;
    else return a->Altura;
}

int maior(int x, int y){
    if(x > y) return x;
    else return y;
}

int AvlFactorBalance(Avl *a){
    return abs(alturaAvl(a->esq) - alturaAvl(a->dir));
}

Avl* RotacaoLL(Avl *a){
    Avl *no;

    no = a->esq;
    a->esq = no->dir;
    no->dir = a;

    a->Altura = maior(alturaAvl(a->esq), alturaAvl(a->dir)) + 1;
    no->Altura = maior(alturaAvl(no->esq), a->Altura) + 1;

    return no;
}

Avl* RotacaoRR(Avl *a){
    Avl *no;

    no = a->dir;
    a->dir = no->esq;
    no->esq = a;

    a->Altura = maior(alturaAvl(a->esq), alturaAvl(a->dir));
    no->Altura = maior(alturaAvl(no->dir), a->Altura) + 1;

    return no;
}

Avl* RotacaoLR(Avl *a){
    a->esq = RotacaoRR(a->esq);
    a = RotacaoLL(a);
    return a;
}

Avl* RotacaoRL(Avl *a){
    a->dir = RotacaoLL(a->dir);
    a = RotacaoRR(a);
    return a;
}

Avl* InsertAvl(Avl *a, Item v){

    // Create the node of the AVL tree with value v
    if(a == NULL){
        Avl *Novo = (Avl*)malloc(sizeof(Avl));
        Novo->Info = v;
        Novo->Altura = 0;
        Novo->esq = Novo->dir = NULL;
        return Novo;
    }

    Avl *current = a;

    if(v.id < a->Info.id){
        a->esq = InsertAvl(a->esq, v);
        if(AvlFactorBalance(a) > 1){
            if(v.id < a->esq->Info.id)
                a = RotacaoLL(a);
            else
                a = RotacaoLR(a);
        }
    }
    else if(v.id > a->Info.id){
        a->dir = InsertAvl(a->dir, v);
        if(AvlFactorBalance(a) > 1){
            if(v.id > a->dir->Info.id)
                a = RotacaoRR(a);
            else
                a = RotacaoRL(a);
        }
    }
    else{
        printf("Valor duplicado!\n");
        exit(1);
    }

    current->Altura = maior(alturaAvl(current->esq), alturaAvl(current->dir)) + 1;

    return a;
}

void ShowAvl(Avl *a){
    if(a == NULL) return;

    printf("%d Altura %d\n", a->Info.id, a->Altura);
    ShowAvl(a->esq);
    ShowAvl(a->dir);
}

int string_to_int(char *number){

    // Find the number of elements into string pointed by number pointer
    int elements = strlen(number);

    int n = 0;

    // Sum the less to most significant number into n integer
    int exp = 0;
    for(int i = elements - 1; i >= 0; i--)
        if(number[i] >= '0'){
            n += (int)(number[i] - '0') * (int)pow(10, exp);
            exp++;
        }
    return n;
}

int main(){
    
    Avl* tree = NULL;

    Item x = {50, "U00001", "Samsung", 5, 5000};
    tree = InsertAvl(tree, x);

    Item y = {45, "U00001", "Samsung", 5, 5000};
    tree = InsertAvl(tree, y);

    Item z = {60, "U00001", "Samsung", 5, 5000};
    tree = InsertAvl(tree, z);

    Item a = {35, "U00001", "Samsung", 5, 5000};
    tree = InsertAvl(tree, a);

    Item b = {25, "U00001", "Samsung", 5, 5000};
    tree = InsertAvl(tree, b);

    ShowAvl(tree);

    char Linha[1000], *p, *q;
    Item products[2000];

    FILE *fp = fopen("DIM_Shopping.csv", "r");
    if(fp == NULL){
        printf("Nao leu DIM_Shopping.csv!\n");
        exit(1);
    }
    
    // Remove the first line which contains just the name o each column
    fscanf(fp, " %[^\n]", Linha);

    int i = 0, j;
    while(fscanf(fp, " %[^\n]", Linha) != EOF){
        p = strtok(Linha, ";");
        j = 0;

        while(p){
            if(j == 0) products[i].id = string_to_int(p);
            if(j == 1) strcpy(products[i].item_id, p);
            if(j == 2) strcpy(products[i].product, p);
            if(j == 3) products[i].quantity = string_to_int(p);
            if(j == 4) products[i].price = string_to_int(p);
            
            p = strtok(NULL, ";");
            j++;
        }

        i++;
    }

    //for(i = 0; i < 1000; i++)
        //printf("%d - %s\n", products[i].id, products[i].product);

    fclose(fp);
    return 0;
}