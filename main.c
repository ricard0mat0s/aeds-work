#include "avl_tree.h"

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

    char Linha[1000], *p, *q;
    Item products[2001];

    FILE *fp = fopen("DIM_Shopping.csv", "r");
    if(fp == NULL){
        printf("Nao leu DIM_Shopping.csv!\n");
        exit(1);
    }
    
    // Remove the first line which contains just the name o each column
    fscanf(fp, " %[^\n]", Linha);

    int i = 1, j;
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

    for(i = 0; i < 2000; i++)
        tree = InsertAvl(tree, products[i]);
    
    fclose(fp);
    return 0;
}