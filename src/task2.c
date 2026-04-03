#include "task2.h"

void creereStiva(Stiva **head, FILE *fi)
{
    double nr;
    Stiva *new;
    while(fscanf(fi,"%lf",&nr)!=0)
    {
        if((*head)==NULL)
        {
            (*head)=(Stiva*)malloc(sizeof(Stiva));
            (*head)->valoare=nr;
            (*head)->next=NULL;
        }
        else
        {
            new=(Stiva*)malloc(sizeof(Stiva));
            new->valoare=nr;
            new->next=(*head);
            (*head)=new;
        }
    }
}

double Arbitraj(double a, double b, double c)
{
    double rez=0.0;
    rez=c-a;
    rez=(int)(rez*1000)/1000.0;
    return rez;
}

void StergereStiva(Stiva **head)
{
    Stiva *q;
    q=(*head);
    while((*head)!=NULL)
    {
        (*head)=(*head)->next;
        free(q);
        q=(*head);
    }
}