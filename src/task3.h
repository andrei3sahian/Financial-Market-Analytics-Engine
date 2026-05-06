#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StockList
{
    char nume[5];
    double pret[7];
    struct StockList *next;
};

typedef struct StockList list;

struct arbore
{
    list *stock;
    int inaltime;
    struct arbore *right, *left;
};

typedef struct arbore arbore;

void creere_lista(list **cap, FILE *fi, int *n);
void divizare_lista(list **caps, list **capd, list *head, int h, int n);
void creere_arbore(arbore *head, int n);
void stergere_arbore(arbore *node);
void stergere_lista(list *node);
void parcurgere(char nume1[], char nume2[], arbore *stanga, arbore *dreapta, int *ok);