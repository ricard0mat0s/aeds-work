#include "avl_tree.h"

int greater(int x, int y){
    if(x > y) return x;
    else return y;
}

int HeightAvl(Avl *a){
    if(a == NULL) return -1;
    else return a->Altura;
}

int AvlFactorBalance(Avl *a){
    return abs(HeightAvl(a->esq) - HeightAvl(a->dir));
}

Avl* RotationLL(Avl *a){
    Avl *no;

    no = a->esq;
    a->esq = no->dir;
    no->dir = a;

    a->Altura = greater(HeightAvl(a->esq), HeightAvl(a->dir)) + 1;
    no->Altura = greater(HeightAvl(no->esq), a->Altura) + 1;

    return no;
}

Avl* RotationRR(Avl *a){
    Avl *no;

    no = a->dir;
    a->dir = no->esq;
    no->esq = a;

    a->Altura = greater(HeightAvl(a->esq), HeightAvl(a->dir));
    no->Altura = greater(HeightAvl(no->dir), a->Altura) + 1;

    return no;
}

Avl* RotationLR(Avl *a){
    a->esq = RotationRR(a->esq);
    a = RotationLL(a);
    return a;
}

Avl* RotationRL(Avl *a){
    a->dir = RotationLL(a->dir);
    a = RotationRR(a);
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
                a = RotationLL(a);
            else
                a = RotationLR(a);
        }
    }
    else if(v.id > a->Info.id){
        a->dir = InsertAvl(a->dir, v);
        if(AvlFactorBalance(a) > 1){
            if(v.id > a->dir->Info.id)
                a = RotationRR(a);
            else
                a = RotationRL(a);
        }
    }
    else{
        printf("Valor duplicado!\n");
        exit(1);
    }

    current->Altura = greater(HeightAvl(current->esq), HeightAvl(current->dir)) + 1;

    return a;
}

Avl* SearchAvl(Avl *a, int v){
    if(a == NULL) NULL;

    if(v == a->Info.id)
        return a;
    else if(v > a->Info.id)
        return SearchAvl(a->dir, v);
    else
        return SearchAvl(a->esq, v);
}