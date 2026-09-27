#include "task2.h"

void creereStiva(Stiva **head, FILE *fi)
{
    double nr;
    while(fscanf(fi,"%lf",&nr)==1)      //daca citeste numar il adauga in lista
    {
        nr=(int)(nr*1000)/1000.0;
        if((*head)==NULL)
        {
            (*head)=(Stiva*)malloc(sizeof(Stiva));
            (*head)->valoare=nr;                            //creere head pentru stiva
            (*head)->next=NULL;
        }
        else
        {
            Stiva *new;
            new=(Stiva*)malloc(sizeof(Stiva));          //adaugare element nou in stiva
            new->valoare=nr;
            new->next=(*head);
            (*head)=new;
        }
    }
}

double Arbitraj(double a, double c)
{
    double rez=0.0;
    rez=c-a;                                //calcul diferenta plus trunchere
    rez=(int)(rez*1000)/1000.0;
    return rez;
}

void StergereStiva(Stiva **head)
{
    if((*head)==NULL)
        return;
    StergereStiva(&((*head)->next));                //stergere stiva recursiv
    free((*head));
    (*head)=NULL;
}