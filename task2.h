#include <stdio.h>
#include <stdlib.h>

struct stiva_piata
{
    double valoare;
    struct stiva_piata *next;
};

typedef struct stiva_piata Stiva;

void creereStiva(Stiva **head, FILE *fi);
double Arbitraj(double a, double c);
void StergereStiva(Stiva **head);
