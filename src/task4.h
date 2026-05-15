#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct  Queue
{
    int q;
    int numarator, numitor;
    struct Queue *next;
} Queue;

typedef struct probabilitati
{
    int numitor, numarator;
} probabil;

typedef struct Noduri
{
    int inceput, final, nr_legaturi;
    probabil prob;
} Nodes;

typedef struct Graph
{
   int **a;
   Nodes *nod[20]; 
} graph;



int creere_mat_adiacenta(int d, FILE *fi, int N, graph *g);
void probabilitati(graph *g, int K, int inceput, int final, FILE *fo, int nr_intervale);
void elibereaza_graf(graph *g, int nr_intervale);