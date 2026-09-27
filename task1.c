#include "task1.h"

double volatilitate(Data *head, double rand_mediu, int N)
{
    double volat=0.0;
    while(head !=NULL)
    {
        volat+=(head->randament-rand_mediu)*(head->randament-rand_mediu);     //trecere prin lista si calculare volatilitate       
        head=head->next;
    }
    return volat=sqrt(volat/(N-1)); 
}

void StergereLista(Data **head)
{
    if((*head)==NULL)
        return;
    StergereLista(&((*head)->next));                //stergere lista
    free((*head));
    (*head)=NULL;
}