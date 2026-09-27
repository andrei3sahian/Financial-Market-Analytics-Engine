#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct portofoliu
{
    double val;
    double randament;
    struct portofoliu *next;
};

typedef struct portofoliu Data;

double volatilitate(Data *head, double rand_mediu, int N);
void StergereLista(Data **head);