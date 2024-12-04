#include <stdio.h>
#include "stdlib.h"




int min ( int a, int b){

    return (a < b) ? a : b;
}

int max ( int a, int b){

    return (a > b) ? a : b;
}


typedef struct Arbre Arbre;


struct Arbre{

    Arbre* fg;
    Arbre* fd;
    int eq;
    int elmt;
};


Arbre* CreerArbre(int e){

    Arbre* a = malloc(sizeof (Arbre));
    if(!a){
        exit(EXIT_FAILURE);
    }
    a->elmt = e;
    a->eq = 0;
    a->fd = NULL;
    a->fg= NULL;
    return a;
}


int ExisteFilsDroit(Arbre* a){

    if(a == NULL){
        exit(EXIT_FAILURE);
    }
    else if(a->fd == NULL){
        return 0;
    }
    else{
        return 1;
    }
}

int ExisteFilsGauche(Arbre* a){
    if(a==NULL){
        exit(EXIT_FAILURE);
    }
    else if(a->fg == NULL){
        return 0;
    }
    else{
        return 1;
    }
}



int AjouterFilsDroit(Arbre* a, int e){

    if(a == NULL){
        return 0;
    }
    else if(!ExisteFilsDroit(a)){
        a->fd = CreerArbre(e);
        return 1;
    }
    else{
        return 0;
    }
}


int AjouterFilsGauche(Arbre* a, int e){

    if(a == NULL){
        return 0;
    }
    else if(!ExisteFilsGauche(a)){
        a->fg = CreerArbre(e);
        return 1;
    }
    else{
        return 0;
    }
}



Arbre* recherche(Arbre* a, int e){

    if(a == NULL){
        return NULL;
    }
    else if(a->elmt == e){
        return a;
    }
    else if(e < a->elmt){
        return recherche(a->fg, e);
    }
    else{
        return recherche(a->fd, e);
    }
}



Arbre* RotationGauche(Arbre* a){
    Arbre* pivot = NULL;
    int eq_a, eq_p;
    pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - max ( eq_p, 0) - 1;
    pivot->eq = min ( eq_a-2, min(eq_a+eq_p-2, eq_p-1));
    a = pivot;
    return a;
}

Arbre* RotationDroite(Arbre* a){
    Arbre* pivot = NULL;
    int eq_a, eq_p;

    pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - min ( eq_p, 0) + 1;
    pivot->eq = max ( eq_a+2, min(eq_a+eq_p+2, eq_p+1));
    a = pivot;
    return a;

}


Arbre* DoubleRotationGauche(Arbre* a){
    a->fd = RotationGauche(a->fd);
    return RotationDroite(a);
}

Arbre* DoubleRotationDroite(Arbre*a){
    a->fg = RotationGauche(a->fg);
    return RotationDroite(a);
}


Arbre* equilibrerAVL(Arbre* a){

    if(a->eq >= 2) {
        if (a->fd->eq >= 0) {
            return RotationGauche(a);
        } else {
            return DoubleRotationGauche(a);
        }
    }
    else if(a->eq <= -2){
        if(a->eq <= 0){
            return RotationDroite(a);
        }
        else{
            return DoubleRotationDroite(a);
        }
    }
    return a;
}



Arbre* insertionAVL(Arbre* a, int e, int* h){
    if ( a == NULL){
        *h = 1;
        return CreerArbre(e);
    }
    else if ( e < a->elmt){
        a->fg =  insertionAVL(a->fg, e, h);
        *h = -*h;
    }
    else if ( e > a->elmt){
        a->fd = insertionAVL(a->fd, e, h);
    }
    else{
        *h = 0;
        return a;
    }

    if(*h != 0){
        a->eq = a->eq + *h;
        a = equilibrerAVL(a);
        if(equilibrerAVL(a) == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return a;
}

Arbre* SuppMinAVL(Arbre* a, int* h, int* pe){
    Arbre* tmp;
    if(a->fg == NULL){
        *pe = a->elmt;
        *h = -1;
        tmp = a;
        a = a->fd;
        return a;
    }
    else{
        a->fg = SuppMinAVL(a->fg, h, pe);
        *h = -*h;
    }

    if(*h != 0){
        a->eq = a->eq + *h;
        if(a->eq != 0){
            *h = -1;
        }
        else{
            *h = 0;
        }
    }
    return a;
}

Arbre* SupressionAVL(Arbre* a, int e, int* h){

    Arbre* tmp =NULL;
    if(a == NULL){
        *h = 1;
        return a;
    }
    else if(e > a->elmt){
        a->fd = SupressionAVL(a->fd, e, h);
    }
    else if(e < a->elmt){
        a->fg = SupressionAVL(a->fg, e, h);
        *h = -*h;
    }
    else if (ExisteFilsDroit(a)){
        a->fd = SuppMinAVL(a, h, &a->elmt);
    }
    else{
        tmp = a;
        a = a->fg;
        free(tmp);
        *h = -1;
        return a;
    }

    if(*h != 0){
        a->eq = a->eq + *h;
        if(a->eq == 0){
            *h = -1;
        }
        else{
            *h = 0;
        }
    }
    return a;
}

void traiter(Arbre* a){

    if(!a){
        printf("vide\n");
    }
    else{
        printf("%d-%d ", a->elmt, a->eq);
    }
    printf(" ");
}

void parcoursPrefixe(Arbre* a){

    if(a!= NULL){
        traiter(a);
        parcoursPrefixe(a->fg);
        parcoursPrefixe(a->fd);
    }
}

int main(void) {

    Arbre* a1 = NULL;
    Arbre* a2 = NULL;

    a1 = CreerArbre(1);
    AjouterFilsDroit(a1, 2);
    AjouterFilsDroit(a1->fd, 3);
    a1->eq = -2;
    a1->fd->eq = -1;


    a2 = CreerArbre(3);
    AjouterFilsGauche(a2, 2);
    AjouterFilsGauche(a2->fg, 1);
    a2->eq = 2;
    a2->fg->eq = 1;

    printf("\nparcours a1 : ");
    parcoursPrefixe(a1);

    printf("\nparcours a1 : ");
    parcoursPrefixe(a2);

    a1 = RotationGauche(a1);
    a2 = RotationDroite(a2);

    printf("\nparcours a1 : ");
    parcoursPrefixe(a1);

    printf("\nparcours a1 : ");
    parcoursPrefixe(a2);


    free(a1);
    free(a2);

    return 0;
}

/*

typedef struct Arbre Arbre;
struct Arbre{
    int elmt;
    Arbre* fg;
    Arbre* fm;
    Arbre* fd;
};

typedef struct File File;
struct File{

    Arbre* noad;
    File* next;
};


Arbre* iniialiserNoeud(int e){

    Arbre* a = malloc(sizeof(Arbre ));
    if(!a){
        exit(1);
    }

    a->fd = NULL;
    a->fg =NULL;
    a->fm = NULL;
    a->elmt = e;
    return a;

}



int compterEnfants(Arbre* a){

    int nb = 0;
    if(!a){
        exit(1);
    }


    if(a->fd!= NULL){
        nb++;
    }

    if(a->fg != NULL){
        nb++;
    }

    if(a->fm != NULL){
        nb++;
    }


    return nb;
}

void parcoursPrefixe(Arbre* a){

    if(!a){
        exit(1);
    }


    printf("%d", a->elmt);
    parcoursPrefixe(a->fg);
    parcoursPrefixe(a->fm);
    parcoursPrefixe(a->fd);
}


File* CreerFile(Arbre* a){
    if(!a){
        exit(1);
    }
    File* f = malloc(sizeof (File));
    if(!f){
        exit(1);
    }

    f->next = NULL;
    f->noad = a;
    return f;
}

void enfile(File** f, Arbre* a){

    if(!a){
        exit (1);
    }

    if(f == NULL){
        f = CreerFile(a);
        return;
    }

    File* tmp = *f;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = CreerFile(a);
}

Arbre* defile(File** f){

    if(!f){
        exit(1);
    }
    Arbre* noad = (*f)->noad;
    f = (*f)->next;

    return noad;
}

void parcoursLargeur(Arbre* a){

    if(!a){
        exit(1);
    }

    File* f = NULL;
    enfile(&f, a);


    while(a != NULL){
        Arbre* noad = defile(f);
        printf("%d", noad->elmt);
        if(noad->fg != NULL) enfile(&f, noad->fg);
        if(noad->fm != NULL) enfile(&f, noad->fg);
        if(noad->fd != NULL) enfile(&f, noad->fd);
    }

}




int main(){






}


*/
