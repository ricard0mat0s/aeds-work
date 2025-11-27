#ifndef AVL_TREE_H
#define AVL_TREE_H

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

int greater(int x, int y);
int HeightAvl(Avl *a);
int AvlFactorBalance(Avl *a);
Avl* RotationLL(Avl *a);
Avl* RotationRR(Avl *a);
Avl* RotationLR(Avl *a);
Avl* RotationRL(Avl *a);
Avl* InsertAvl(Avl *a, Item v);
Avl* SearchAvl(Avl *a, int v);
// Search that also counts node accesses. If 'accesses' is not NULL,
// it will be incremented for each node visited during the search.
Avl* SearchAvlWithCount(Avl *a, int v, int *accesses);

#endif