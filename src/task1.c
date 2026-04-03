#include "task1.h"

double volatilitate(Data *head, double rand_mediu, int N)
{
    double volat=0.0;
    while(head !=NULL)
    {
        volat+=(head->randament-rand_mediu)*(head->randament-rand_mediu);            
        head=head->next;
    }
    return volat=sqrt(volat/(N-1)); 
}

void StergereLista(Data **head)
{
    Data *q;
    while((*head)!=NULL)
    {
        q=(*head);
        (*head)=(*head)->next;
       free(q);
    }
}