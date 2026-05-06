#include "task3.h"

void creere_lista(list **cap, FILE *fi, int *n)

{
    list *new, *temp=NULL;
    int i, j;
    for(i=0;i<10;i++)
    {
        if((*cap)==NULL)
        {
            *cap=(list*)malloc(sizeof(list));
            fgets((*cap)->nume,5,fi);
            fseek(fi,1,1);
            (*cap)->next=NULL;
            temp=(*cap);
        }
        else
        {
            new=(list*)malloc(sizeof(list));
            fgets(new->nume,5,fi);
            if(i!=9)
                fseek(fi,1,1);
            new->next=NULL;
            temp->next=new;
            temp=new;
        }
    }
    i=1;
    j=0;
    temp=(*cap);
    while(fscanf(fi,"%lf,", &temp->pret[j])==1)
    {
        if(i%10==0)
        {
            fseek(fi,1,1);
            j++;
            i++;
            temp=(*cap);
        }
        else
        {
            temp=temp->next;
            i++;
        }
    }
    (*n)=(i-1)/10;
}

void divizare_lista(list **caps, list **capd, list *head, int h, int n)
{
    int j;
    list *prevs=NULL, *prevd=NULL, *new;
    for( ;head!=NULL;head=head->next)
    {
        if(head->pret[h]>head->pret[h+1])
        {
            printf("[NIVEL %d] %s: %.2f > %.2f -> STINGA\n", h, head->nume, head->pret[h], head->pret[h+1]);
            if((*caps)==NULL)
            {
                (*caps)=(list*)malloc(sizeof(list));
                strcpy((*caps)->nume,head->nume);
                for(j=0;j<n;j++)
                    (*caps)->pret[j]=head->pret[j];
                (*caps)->next=NULL;
                prevs=(*caps);
            }
            else
            {
                new=(list*)malloc(sizeof(list));
                strcpy(new->nume,head->nume);
                for(j=0;j<n;j++)
                    new->pret[j]=head->pret[j];
                new->next=NULL;
                prevs->next=new;
                prevs=new;
            }
        }
        else
        {
            if((*capd)==NULL)
            {
                (*capd)=(list*)malloc(sizeof(list));
                strcpy((*capd)->nume,head->nume);
                for(j=0;j<n;j++)
                    (*capd)->pret[j]=head->pret[j];
                (*capd)->next=NULL;
                prevd=(*capd);
            }
            else
            {
                new=(list*)malloc(sizeof(list));
                strcpy(new->nume,head->nume);
                for(j=0;j<n;j++)
                    new->pret[j]=head->pret[j];
                new->next=NULL;
                prevd->next=new;
                prevd=new;
            }
        }
    }
}

void creere_arbore(arbore *head, int n)
{
    if(n<=1)
        return ;
    list *cap1=NULL, *cap2=NULL; 
    divizare_lista(&cap1,&cap2,head->stock,head->inaltime,n+head->inaltime);
    head->left=(arbore*)malloc(sizeof(arbore));
    head->left->stock=cap1;
    head->left->inaltime=head->inaltime+1;
    head->left->left=NULL;
    head->left->right=NULL;

    head->right=(arbore*)malloc(sizeof(arbore));
    head->right->stock=cap2;         
    head->right->inaltime=head->inaltime+1;
    head->right->left=NULL;
    head->right->right=NULL;

    creere_arbore(head->left, n-1);
    creere_arbore(head->right, n-1);
}

void stergere_arbore(arbore *node) 
{
    if (node == NULL) 
    {
        return;
    }
    stergere_arbore(node->left);
    stergere_arbore(node->right);
    if (node->stock != NULL) 
    {
        stergere_lista(node->stock);
    }
    free(node);
}

void stergere_lista(list *node)
{
    list *q=NULL;
    while(node!=NULL)
    {
        q=node;
        node=node->next;
        free(q);
    }
}


void parcurgere(char nume1[], char nume2[], arbore *stanga, arbore *dreapta, int *ok)
{
    if(stanga==NULL || (*ok)==1 || dreapta==NULL)
        return ;
    parcurgere(nume1,nume2,stanga->left,dreapta->right,&(*ok));
    parcurgere(nume1,nume2,stanga->right,dreapta->left,&(*ok));
    if(stanga->left==NULL && stanga->right==NULL)
    {
        list *p=stanga->stock;
        while(p!=NULL)
        {
            if(strcmp(p->nume,nume1)==0)
            {
                list *q=dreapta->stock;
                while(q!=NULL)
                {
                    if(strcmp(q->nume,nume2)==0)
                    {
                        (*ok)=1;
                        break;
                    }
                    q=q->next;
                }
            }
            if((*ok)==1)
                break;
            p=p->next;
        }
    }
}