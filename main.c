#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char Linha[1000], *p, *q;
    FILE *fp = fopen("DIM_Shopping.csv", "r");
    if(fp == NULL){
        printf("Nao leu DIM_Shopping.csv!\n");
        exit(1);
    }

    int i = 0, j;
    while(fscanf(fp, " %[^\n]", Linha) != EOF){
        printf("%s\n", Linha);
        p = strtok(Linha, ";");
        j = 0;
        while(p){
            if(j == 0) printf("Item: ");
            if(j == 1) printf("ID: ");
            if(j == 2) printf("Nome: ");
            if(j == 3) printf("Quantidade: ");
            if(j == 4) printf("Preco: ");
            printf("%s\n", p);
            p = strtok(NULL, ";");
            j++;
        }
        i++;
    }



    fclose(fp);
    return 0;
}