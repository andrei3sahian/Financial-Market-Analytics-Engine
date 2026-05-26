#include "task3.h"

void creere_lista(list **cap, FILE *fi, int *n)

{
    list *new, *temp=NULL;
    int i, j;
    for(i=0;i<10;i++)           //se retin numele actiunilor
    {
        if((*cap)==NULL)
        {
            *cap=(list*)malloc(sizeof(list));
            fgets((*cap)->nume,5,fi);
            fseek(fi,1,1);          //se trece peste virgula din fisierul de input
            (*cap)->next=NULL;
            temp=(*cap);
        }
        else
        {
            new=(list*)malloc(sizeof(list));
            fgets(new->nume,5,fi);
            if(i!=9)
                fseek(fi,1,1);          //se trece peste \n de la final de rand
            new->next=NULL;
            temp->next=new;
            temp=new;
        }
    }
    i=1;
    j=0;
    temp=(*cap);
    while(fscanf(fi,"%lf,", &temp->pret[j])==1)         //se retin preturile pana cand se termina fisierul
    {
        if(i%10==0)
        {
            fseek(fi,1,1);
            j++;
            i++;                    //se trece peste virgula dintre numere
            temp=(*cap);
        }
        else
        {
            temp=temp->next;        //se continua popularea listei cu numere
            i++;
        }
    }
    (*n)=(i-1)/10;          //se calculeaza numarul de zile
}

void divizare_lista(list **caps, list **capd, list *head, int h, int n)
{
    int j;
    list *prevs=NULL, *prevd=NULL, *new;
    for( ;head!=NULL;head=head->next)
    {                                                               //functia divizeaza lista mare in doua liste mai mici
        if(head->pret[h]>head->pret[h+1])                           //in functie de pret daca scade sau creste
        {                                                           //se compara pretul de la ziua h cu ziua urmatoare
            if((*caps)==NULL)
            {
                (*caps)=(list*)malloc(sizeof(list));
                strcpy((*caps)->nume,head->nume);
                for(j=0;j<n;j++)
                    (*caps)->pret[j]=head->pret[j];                 //se retine vectorul de preturi pentru fiecare actiune
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
    divizare_lista(&cap1,&cap2,head->stock,head->inaltime,n+head->inaltime);            //se da in functie numarul de zile ca n+inaltime
    head->left=(arbore*)malloc(sizeof(arbore));                                         //si se da inaltimea si lista principala
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
    stergere_arbore(node->left);            //stergere arbore recursiv postorder
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
    {                               //se sterg listele de pe fiecare nod 
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
        {                                                                   //se parcurge recursiv arborele cu 2 pointeri(unul stanga si unul dreapta)
            if(strcmp(p->nume,nume1)==0)                                    //daca gaseste primul nume intr-o lista din frunza
            {
                list *q=dreapta->stock;
                while(q!=NULL)                                              //il verifica pe al doilea daca se gaseste in oglinda si daca da se opreste functia
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